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

	for (int rank = RANK_8; rank >= RANK_1; rank--) {
		printf("%d  ", rank + 1);	// enum is zero-based
		for (int file = FILE_A; file <= FILE_H; file++) {
			enum square sq = GET_SQUARE(rank, file);
			enum piece pce = the_board->pieces[sq];
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
	if (the_board->side_to_move == WHITE) {
		side = 'w';
	} else {
		side = 'b';
	}
	printf("side:\t%c\n", side);

	if (the_board->en_passant == NO_SQUARE) {
		printf("enPas:\t-\n");
	} else {
		uint32_t rank = GET_RANK(the_board->en_passant);
		int file = GET_FILE(the_board->en_passant);
		printf("enPas:\t%c%c\n", files[file], ranks[rank]);
	}

	printf("castle:\t%c%c%c%c\n",
	       (the_board->castle_perm & WKCA) ? 'K' : '-',
	       (the_board->castle_perm & WQCA) ? 'Q' : '-',
	       (the_board->castle_perm & BKCA) ? 'k' : '-',
	       (the_board->castle_perm & BQCA) ? 'q' : '-');

	printf("PosKey:\t%jx\n", the_board->board_hash);

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

	int file = GET_FILE(sq);
	int rank = GET_RANK(sq);

	sprintf(square_text, "%c%c", ('a' + file), ('1' + rank));

	return square_text;
}

void print_compressed_board(const struct board *brd)
{
	for(enum square sq = a1; sq <=h8; sq++){
		enum piece pce = brd->pieces[sq];
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
		conflated |= brd->bitboards[i];
	}
	assert(conflated == brd->board);

	uint64_t wking_bb = brd->bitboards[W_KING];
	assert(count_bits(wking_bb) == 1);
	uint64_t bking_bb = brd->bitboards[B_KING];
	assert(count_bits(bking_bb) == 1);

	// check where Kings are
	for (enum square sq = a1; sq <= h8; sq++) {
		enum piece pce = brd->pieces[sq];
		if (pce != NO_PIECE) {
			if (pce == W_KING) {

				uint64_t bb_wk = brd->bitboards[W_KING];
				enum square wk_sq = pop_1st_bit(&bb_wk);

				assert(sq == wk_sq);
				assert(brd->king_sq[WHITE] == wk_sq);
			} else if (pce == B_KING) {

				uint64_t bb_bk = brd->bitboards[B_KING];
				enum square bk_sq = pop_1st_bit(&bb_bk);

				assert(sq == bk_sq);
				assert(brd->king_sq[BLACK] == bk_sq);
			}
		}
	}

	// check verbose representation of board
	for (enum square sq = 0; sq < NUM_SQUARES; sq++) {
		enum piece pce = brd->pieces[sq];
		if (pce != NO_PIECE) {
			assert(pce == brd->pieces[sq]);
		}
	}

	assert(IS_VALID_SQUARE(brd->en_passant)
	       || (brd->en_passant == NO_SQUARE));

	// calc and verify the material count
	uint32_t local_material[NUM_COLOURS] = { 0 };
	for (enum square sq = 0; sq <= h8; sq++) {
		enum piece pce = brd->pieces[sq];
		if (pce != NO_PIECE) {
			enum colour col = GET_COLOUR(pce);
			local_material[col] += get_piece_value(pce);
		}
	}

	assert(local_material[WHITE] == brd->material[WHITE]);
	assert(local_material[BLACK] == brd->material[BLACK]);

	// check on position key
	assert(brd->board_hash == get_position_hash(brd));

	return true;

}

/* Compares two boards and checks for equality
 *
 * name: assert_boards_are_equal
 * @param
 * @return
 *
 */
void assert_boards_are_equal(const struct board *brd1, const struct board *brd2)
{

	for (int i = 0; i < NUM_PIECES; i++) {
		assert(brd1->bitboards[i] == brd2->bitboards[i]);
	}

	assert(brd1->board == brd2->board);

	assert(brd1->side_to_move == brd2->side_to_move);

	assert(brd1->en_passant == brd2->en_passant);

	assert(brd1->fifty_move_counter == brd2->fifty_move_counter);

	assert(brd1->ply == brd2->ply);
	assert(brd1->history_ply == brd2->history_ply);

	assert(brd1->material[WHITE] == brd2->material[WHITE]);
	assert(brd1->material[BLACK] == brd2->material[BLACK]);

	for (int i = 0; i < NUM_SQUARES; i++) {
		assert(brd1->pieces[i] == brd2->pieces[i]);
	}

	assert(brd1->castle_perm == brd2->castle_perm);

	// already verified that brd1->history_ply == brd2_history_ply
	for (int i = 0; i < brd1->history_ply; i++) {
		assert(brd1->history[i].move == brd2->history[i].move);
		assert(brd1->history[i].fifty_move_counter ==
		       brd2->history[i].fifty_move_counter);
		assert(brd1->history[i].castle_perm ==
		       brd2->history[i].castle_perm);
		assert(brd1->history[i].board_hash ==
		       brd2->history[i].board_hash);
		assert(brd1->history[i].en_passant ==
		       brd2->history[i].en_passant);
	}

	assert(brd1->board_hash == brd2->board_hash);

}

/*
 * Clones the given board. Returns malloc'ed memory that needs to be free'd
 * name: clone_board
 * @param
 * @return
 *
 */
void clone_board(const struct board *board_to_clone, struct board *cloned)
{
	memcpy(cloned, board_to_clone, sizeof(struct board));
}

// parses and validates a user-entered move
// ip_move -> "a3b4" or "d7d8r"
mv_bitmap parse_move(char *ip_move, struct board * brd)
{
	if (ip_move[1] > '8' || ip_move[1] < '1')
		return NO_MOVE;
	if (ip_move[3] > '8' || ip_move[3] < '1')
		return NO_MOVE;
	if (ip_move[0] > 'h' || ip_move[0] < 'a')
		return NO_MOVE;
	if (ip_move[2] > 'h' || ip_move[2] < 'a')
		return NO_MOVE;

	uint8_t from_file = (uint8_t) (ip_move[0] - 'a');
	uint8_t from_rank = (uint8_t) (ip_move[1] - '1');
	uint8_t to_file = (uint8_t) (ip_move[2] - 'a');
	uint8_t to_rank = (uint8_t) (ip_move[3] - '1');

	enum square from = GET_SQUARE(from_rank, from_file);
	enum square to = GET_SQUARE(to_rank, to_file);

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
