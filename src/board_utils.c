/*
 * board_utils.c
 *
 * ---------------------------------------------------------------------
 * Description : Misc utility functions associated with the board
 * ---------------------------------------------------------------------
 *
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
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include "types.h"
#include "move_gen.h"
#include "move_gen_utils.h"
#include "hashkeys.h"
#include "board.h"
#include "pieces.h"
#include "utils.h"
#include "board_utils.h"

// char arrays to suport printing
static const char ranks[] = "12345678";
static const char files[] = "abcdefgh";

/*
 * Pretty-prints the board
 *
 * name: print_board
 * @param: a board
 * @return : void
 *
 *
 *
 * Thanks again to Bluefever Software for this code
 */
void print_board(const struct board *the_board)
{
    printf("\nGame Board:\n\n");

    for (int r = RANK_8; r >= RANK_1; r--) {
        printf("%d  ", r + 1);	// enum is zero-based
        for (int f = FILE_A; f <= FILE_H; f++) {
            enum square sq = get_square(r, f);
            
            enum piece pce = get_piece_on_square(the_board, sq);
            if (pce != NO_PIECE) {
                char c = get_piece_label(pce);
                printf("%3c", c);
            } else {
                printf("  .");
            }
        }
        printf("\n");
    }

    printf("\n   ");
    for (int file = FILE_A; file <= FILE_H; file++) {
        printf("%3c", 'a' + file);
    }
    printf("\n\n");
    char side;
    if (get_side_to_move(the_board) == WHITE) {
        side = 'w';
    } else {
        side = 'b';
    }
    printf("side:\t%c\n", side);

	enum square enp_sq = get_en_passant_sq(the_board);
    if (enp_sq == NO_SQUARE) {
        printf("enPas:\t-\n");
    } else {
        uint8_t rank = get_rank(enp_sq);
        uint8_t file = get_file(enp_sq);
        printf("enPas:\t%c%c\n", files[file], ranks[rank]);
    }

	enum castle_perm perm = get_castle_permissions(the_board);
    printf("castle:\t%c%c%c%c\n",
           (perm & WKCA) ? 'K' : '-',
           (perm & WQCA) ? 'Q' : '-',
           (perm & BKCA) ? 'k' : '-',
           (perm & BQCA) ? 'q' : '-');

    printf("PosKey:\t%jx\n", get_board_hash(the_board));

    /*
    	printf("Move history\n");
    	for(int i = 0; i < the_board->history_ply; i++){
    		printf("Move #%d\n", i);
    		printf("\t");
    		print_move_details(the_board->history[i].move, 0);
    		if (the_board->history[i].en_passant != NO_SQUARE){
    			printf("\ten passant : %s\n", print_square(the_board->history[i].en_passant));
    		} else{
    			printf("\ten passant : -\n");
    		}
    	}
    */
    printf("\n");

}

/*
 * Prints out a square as file/rank, eg a2
 * name: print_square
 * @param
 * @return a malloc'ed buffer that needs to be free'd
 *
 */
char *print_square(enum square sq)
{
    char *square_text = (char *)malloc(3 * sizeof(char));

    int file = get_file(sq);
    int rank = get_rank(sq);

    sprintf(square_text, "%c%c", ('a' + file), ('1' + rank));

    return square_text;
}

void print_compressed_board(const struct board *brd)
{
    for(enum square sq = a1; sq <=h8; sq++) {
        enum piece pce = get_piece_on_square(brd, sq);
        //printf("***pce = %d\n", pce);
        assert(IS_VALID_PIECE_OR_NO_PIECE(pce));

        if (pce == NO_PIECE) {
            printf(".");
        } else {
            printf("%c", get_piece_label(pce));
        }
    }

}

/*
 * Validates the contents of a board struct.
 *
 * name: ASSERT_BOARD_OK
 * @param
 * @return
 *
 */

bool ASSERT_BOARD_OK(const struct board *brd)
{
    // check bit boards
    uint64_t conflated = 0;

    for (int i = 0; i < NUM_PIECES; i++) {
        conflated |= get_bitboard(brd, (enum piece)i);
    }

    assert(conflated == get_bitboard_all_pieces(brd));
    
    uint64_t wking_bb = get_bitboard_for_king(brd, WHITE);
    assert(count_bits(wking_bb) == 1);
    uint64_t bking_bb = get_bitboard_for_king(brd, BLACK);
    assert(count_bits(bking_bb) == 1);

    // check where Kings are
    for (enum square sq = a1; sq <= h8; sq++) {
        enum piece pce = get_piece_on_square(brd, sq);
        if (pce != NO_PIECE) {
            if (pce == W_KING) {

                uint64_t bb_wk = get_bitboard_for_king(brd, WHITE);
                enum square wk_sq = pop_1st_bit(&bb_wk);

                assert(sq == wk_sq);
                
                assert(get_king_square(brd, WHITE) == wk_sq);
            } else if (pce == B_KING) {

                uint64_t bb_bk = get_bitboard(brd, B_KING);
                enum square bk_sq = pop_1st_bit(&bb_bk);

                assert(sq == bk_sq);
                assert(get_king_square(brd, BLACK) == bk_sq);
            }
        }
    }

    // check verbose representation of board
    uint64_t black_bb = get_bitboard_for_colour(brd, BLACK);
    uint64_t white_bb = get_bitboard_for_colour(brd, WHITE);

	assert(conflated == (black_bb | white_bb));


    for (enum square sq = 0; sq < NUM_SQUARES; sq++) {
        enum piece pce = get_piece_on_square(brd, sq);
        if (pce != NO_PIECE) {
            assert(is_square_occupied(get_bitboard_all_pieces(brd), sq) != 0);

            if (GET_COLOUR(pce) == WHITE) {
                assert(is_square_occupied(white_bb, sq) != 0);
            } else {
                assert(is_square_occupied(black_bb, sq) != 0);
            }

            uint64_t pce_bb = get_bitboard(brd, pce);
            assert(is_square_occupied(pce_bb, sq) != 0);
        }
    }

	enum square enp_sq = get_en_passant_sq(brd);
    assert(IS_VALID_SQUARE(enp_sq) || (enp_sq == NO_SQUARE));


    assert_material_correct(brd);


    // check on position key
    assert(get_board_hash(brd) == get_position_hash(brd));

    return true;

}


inline uint64_t get_bitboard_for_colour(const struct board *brd, enum colour col)
{
    uint64_t retval = 0;
    switch(col) {
    case (WHITE):
        retval |= get_bitboard(brd, W_PAWN);
        retval |= get_bitboard(brd, W_BISHOP);
        retval |= get_bitboard(brd, W_ROOK);
        retval |= get_bitboard(brd, W_KNIGHT);
        retval |= get_bitboard(brd, W_QUEEN);
        retval |= get_bitboard(brd, W_KING);
        return retval;
    case (BLACK):
        retval |= get_bitboard(brd, B_PAWN);
        retval |= get_bitboard(brd, B_BISHOP);
        retval |= get_bitboard(brd, B_ROOK);
        retval |= get_bitboard(brd, B_KNIGHT);
        retval |= get_bitboard(brd, B_QUEEN);
        retval |= get_bitboard(brd, B_KING);
        return retval;
    default:
        assert(false);
        print_stacktrace();
        exit(-1);
    }
}


void assert_material_correct(const struct board *brd)
{

#ifdef ENABLE_ASSERTS

    // calc and verify the material count
    int32_t local_material[NUM_COLOURS] = {0, 0};
    
    for (enum square sq = a1; sq <= h8; sq++) {
        enum piece pce = get_piece_on_square(brd, sq);
        if (pce != NO_PIECE) {
            enum colour col = GET_COLOUR(pce);
            local_material[col] += GET_PIECE_VALUE(pce);
        }
    }
    assert(local_material[WHITE] == get_material_value(brd, WHITE));

    assert(local_material[BLACK] == get_material_value(brd, BLACK));
#endif
}



// parses and validates a user-entered move
// ip_move -> "a3b4" or "d7d8r"
mv_bitmap parse_move(char *ip_move, struct board * brd)
{
    if (ip_move[1] > '8' || ip_move[1] < '1') {
        return NO_MOVE;
    }
    if (ip_move[3] > '8' || ip_move[3] < '1') {
        return NO_MOVE;
    }
    if (ip_move[0] > 'h' || ip_move[0] < 'a') {
        return NO_MOVE;
    }
    if (ip_move[2] > 'h' || ip_move[2] < 'a') {
        return NO_MOVE;
    }

    uint8_t from_file = (uint8_t) (ip_move[0] - 'a');
    uint8_t from_rank = (uint8_t) (ip_move[1] - '1');
    uint8_t to_file = (uint8_t) (ip_move[2] - 'a');
    uint8_t to_rank = (uint8_t) (ip_move[3] - '1');

    enum square from = get_square(from_rank, from_file);
    enum square to = get_square(to_rank, to_file);

    // create ampty move list
    struct move_list mv_list = {
        .moves = {0},
        .move_count = 0
    };

    generate_all_moves(brd, &mv_list);

    mv_bitmap move = NO_MOVE;
    enum piece promoted = NO_PIECE;

    for (int move_num = 0; move_num < mv_list.move_count; move_num++) {
        move = mv_list.moves[move_num];

        if ((FROMSQ(move) == from) && (TOSQ(move) == to)) {
            promoted = PROMOTED_PCE(move);
            if (promoted != NO_PIECE) {
                if (IS_ROOK(promoted) && ip_move[4] == 'r') {
                    return move;
                } else if (IS_BISHOP(promoted)
                           && ip_move[4] == 'b') {
                    return move;
                } else if (IS_QUEEN(promoted)
                           && ip_move[4] == 'q') {
                    return move;
                } else if (IS_KNIGHT(promoted)
                           && ip_move[4] == 'n') {
                    return move;
                }
                continue;
            }
            return move;
        }
    }
    return NO_MOVE;
}
