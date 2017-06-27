/*
 * search.c
 *
 * ---------------------------------------------------------------------
 * DESCRIPTION : Contains code for searching for moves and determining
 * the best ones
 * ---------------------------------------------------------------------
 *
 *
 * Copyright (C) 2015 Eddie McNally <emcn@gmx.com>
 *
 * kestrel is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * kestrel is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "kestrel.h"
#include "search.h"
#include "attack.h"
#include "evaluate.h"
#include "board.h"
#include "pieces.h"
#include "tt.h"
#include "board_utils.h"
#include "move_gen.h"
#include "move_gen_utils.h"
#include "uci_protocol.h"
#include "utils.h"


static int32_t quiescence(struct position *pos, struct search_info *si, int32_t alpha, int32_t beta);
static int32_t alpha_beta(struct position *pos, struct search_info *si, int32_t alpha, int32_t beta, uint8_t depth);
static void init_search(struct position *pos);
static inline void check_search_time_limit(struct search_info *sinfo);


// keep as power of 2
#define	EXPIRY_NODE_COUNT	1024


void init_search_struct(struct search_info *si)
{
    memset(si, 0, sizeof(struct search_info));
}





void search_positions(struct position *pos, struct search_info *si, uint32_t tt_size_in_bytes)
{

    si->search_start_time = get_time_of_day_in_millis();

    //assert(ASSERT_BOARD_OK(pos) == true);

    init_search(pos);

    create_tt_table(tt_size_in_bytes);

    mv_bitmap best_move = NO_MOVE;
    int32_t score = 0;
    uint8_t num_moves = 0;
    // use iterative deepening
    for(uint8_t current_depth = 1; current_depth <= si->depth; current_depth++) {
        score = alpha_beta(pos, si, -INFINITE, INFINITE, current_depth);

        if (si->search_stopped == true) {
            break;
        }
        num_moves = populate_pv_line(pos, current_depth);

        best_move = get_best_pvline(pos);

        uci_print_info_score(score, current_depth, si->num_nodes,
                             (get_time_of_day_in_millis() - si->search_start_time),
                             num_moves, &best_move);
    }

    // populate the best moves
    for(int i = 0; i < MAX_SEARCH_DEPTH; i++) {
        if (i < num_moves) {
            continue;
        }

        set_pvline(pos, (uint8_t)i, NO_MOVE);
    }
    // update search stats
    uint32_t elapsed_time_in_millis = (uint32_t)(get_time_of_day_in_millis() - si->search_start_time);
    si->nodes_per_second = (si->num_nodes * 1000) / elapsed_time_in_millis;

    uci_print_bestmove(best_move);
}



static void init_search(struct position *pos)
{

    for(int i = 0; i < MAX_SEARCH_DEPTH; i++) {
        set_pvline(pos, (uint8_t)i, NO_MOVE);
    }

	init_search_history(pos);

	init_search_killers(pos);

    set_ply(pos, 0);

}


static int32_t alpha_beta(struct position *pos, struct search_info *si, int32_t alpha, int32_t beta, uint8_t depth)
{
    if(depth <= 0) {
        return quiescence(pos, si, alpha, beta);
    }

    // only check every so many nodes
    if ((si->num_nodes & (EXPIRY_NODE_COUNT - 1)) == 0) {
        check_search_time_limit(si);
    }

    si->num_nodes++;

    if (is_repetition(pos)) {
        si->repetition++;
        return 0; // a draw
    }

    if (get_fifty_move_counter(pos) >= 100) {
        si->fifty_move_rule++;
        return 0; // a draw
    }

    if (get_ply(pos) > MAX_SEARCH_DEPTH - 1) {
        si->max_depth_reached++;
        return evaluate_position(pos);
    }

    mv_bitmap best_move = NO_MOVE;
    int32_t old_alpha = alpha;

    struct move_list mvl = {
        .moves = {0},
        .move_count = 0
    };

    generate_all_moves(pos, &mvl);

    // check is position already in PV table
    mv_bitmap pv_move = probe_tt(get_board_hash(pos));
    if (pv_move != NO_MOVE) {
        // prioritise
        for(uint16_t i = 0; i < mvl.move_count; i++) {
            if (mvl.moves[i] == pv_move) {
                add_to_score(&mvl.moves[i], MOVE_ORDER_WEIGHT_PV_MOVE);
                si->move_ordering_pv_move++;
                break;
            }
        }
    }

    uint16_t num_moves = mvl.move_count;

    uint8_t legal_move_cnt = 0;
    for(uint16_t i = 0; i < num_moves; i++) {
        bring_best_move_to_top(i, &mvl);

        // incr search stats
        si->num_nodes++;

        mv_bitmap mv = mvl.moves[i];
        bool valid_move = make_move(pos, mv);
        if (valid_move) {
            legal_move_cnt++;

            // note: alpha/beta are swapped, and sign is reversed
            int32_t score = -alpha_beta(pos, si, -beta, -alpha, (uint8_t)(depth - 1));
            take_move(pos);

            if (si->search_stopped == true) {
                // timed out
                return 0;
            }

            if (score > alpha) {
                if (score >= beta) {
                    if (legal_move_cnt == 1) {
                        si->fail_high_first++;
                    }
                    si->fail_high++;

                    // killer move....beta cutoff, no capture
                    if (IS_CAPTURE_MOVE(mv) == false) {
                        si->killer_moves++;
                        // shuffle down killers
                        shuffle_search_killers(pos, mv);
                    }

                    return beta;
                }
                alpha = score;
                best_move = mv;


                // search history....alpha cutoff, no capture
                if (IS_CAPTURE_MOVE(mv) == false) {
                    si->search_history++;
                    enum square from_sq = FROMSQ(best_move);
                    enum square to_sq = TOSQ(best_move);

                    enum piece pce = get_piece_on_square(pos, from_sq);
                    add_to_search_history(pos, pce, to_sq, depth);
				}


            }
        } else {
            si->invalid_moves_made++;
        }
    }

    if(legal_move_cnt == 0) {
        si->zero_legal_moves++;
        //printf("***no legal moves left\n");
        // no legal moves....must be mate or draw
        enum colour side_to_move = get_side_to_move(pos);

        enum square king_sq = get_king_square(pos, side_to_move);
        enum colour opposite_side = GET_OPPOSITE_SIDE(side_to_move);

        if (is_sq_attacked(pos, king_sq, opposite_side)) {
            si->mates_detected++;
            return -MATE + get_ply(pos);
        } else {
            // draw
            return 0;
        }
    }

    if (alpha != old_alpha) {
        // improved alpha, so add to tt
        uint64_t board_hash = get_board_hash(pos);
        add_to_tt(board_hash, best_move, depth);

        // search stats
        si->added_to_tt++;
    }

    return alpha;
}



static int32_t quiescence(struct position *pos, struct search_info *si, int32_t alpha, int32_t beta)
{

    // only check every so many nodes
    if ((si->num_nodes & (EXPIRY_NODE_COUNT - 1)) == 0) {
        check_search_time_limit(si);
    }
    si->num_nodes++;

    if (is_repetition(pos) || get_fifty_move_counter(pos) > 100) {
        // draw
        return 0;
    }

    if (get_ply(pos) > MAX_SEARCH_DEPTH - 1) {
        return evaluate_position(pos);
    }

    // stand pat
    int32_t stand_pat_score = evaluate_position(pos);
    if (stand_pat_score >= beta) {
        si->stand_pat_cutoff++;
        return beta;
    }
    if (stand_pat_score > alpha) {
        si->stand_pat_improvement++;
        alpha = stand_pat_score;
    }


    struct move_list mvl = {
        .moves = {0},
        .move_count = 0
    };

    // only the capture moves
    generate_all_capture_moves(pos, &mvl);

    uint16_t num_moves = mvl.move_count;

    for(uint16_t i = 0; i < num_moves; i++) {
        bring_best_move_to_top(i, &mvl);

        mv_bitmap mv = mvl.moves[i];
        bool valid_move = make_move(pos, mv);
        if (valid_move) {

            // note: alpha/beta are swapped, and sign is reversed
            int32_t score = -quiescence(pos, si, -beta, -alpha);
            take_move(pos);

            if (si->search_stopped == true) {
                // timed out
                return 0;
            }

            if (score > alpha) {
                if (score >= beta) {
                    return beta;
                }

                alpha = score;
            }
        }
    }
    return alpha;
}



static inline void check_search_time_limit(struct search_info *sinfo)
{
    if(sinfo->search_time_set == true) {
        uint64_t curr_time_of_day = get_time_of_day_in_millis();
        if (curr_time_of_day >= sinfo->search_expiry_time) {
            // search timed out
            sinfo->search_stopped = true;
        }
    }
}



inline void bring_best_move_to_top(uint16_t move_num, struct move_list *mvl)
{

    uint32_t best_score = 0;
    uint16_t best_move_num = move_num;

    for(uint16_t i = move_num; i < mvl->move_count; i++) {
        uint32_t score = get_score(mvl->moves[i]);
        if (score > best_score) {
            best_score = score;
            best_move_num = i;
        }
    }

    mv_bitmap temp_mv = mvl->moves[move_num];
    mvl->moves[move_num] = mvl->moves[best_move_num];
    mvl->moves[best_move_num] = temp_mv;
}

void dump_search_info(struct search_info *si)
{
    printf("Search Stats :\n");
    printf("\tSearch Depth..............%d\n", si->depth);
    printf("\t#nodes....................%d\n", si->num_nodes);
    printf("\t#nodes/sec................%d\n", si->nodes_per_second);
    printf("\t#add to TT................%d\n", si->added_to_tt);
    printf("\t#invalid moves............%d\n", si->invalid_moves_made);
    printf("\t#zero legal moves.........%d\n", si->zero_legal_moves);
    printf("\t#repetitions..............%d\n", si->repetition);
    printf("\t#mate moves detected......%d\n", si->mates_detected);
    printf("\t#50-move rules............%d\n", si->fifty_move_rule);
    printf("\t#max depth reached........%d\n", si->max_depth_reached);
    printf("\tmove ordering : PV Move...%d\n", si->move_ordering_pv_move);
    printf("\tmove ordering : Killers...%d\n", si->killer_moves);
    printf("\tmove ordering : History...%d\n", si->search_history);
    printf("\tfhf/fh....................%.2f\n", ((float)si->fail_high_first/(float)si->fail_high));
    printf("\tstand-pat beta cutoff.....%d\n", si->stand_pat_cutoff);
    printf("\tstand-pat improvement.....%d\n", si->stand_pat_improvement);
}
