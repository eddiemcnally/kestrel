/*
 * board_utils.c
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
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "types.h"
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

    //int sq,piece;

    printf("\nGame Board:\n\n");

    for (int rank = RANK_8; rank >= RANK_1; rank--) {
		printf("%d  ", rank + 1);	// enum is zero-based
		for (int file = FILE_A; file <= FILE_H; file++) {
			enum square sq = GET_SQUARE(rank, file);
			enum piece pce = get_piece_at_square(the_board, sq);
			if (pce != NO_PIECE) {
				char c = get_piece_label(pce);
				printf("%3c", c);
			} else {
				printf("  -");
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
		U32 rank = GET_RANK(the_board->en_passant);
		int file = GET_FILE(the_board->en_passant);
		printf("enPas:\t%c%c\n", files[file], ranks[rank]);
    }

    printf("castle:\t%c%c%c%c\n",
	   (the_board->castle_perm & WKCA) ? 'K' : '-',
	   (the_board->castle_perm & WQCA) ? 'Q' : '-',
	   (the_board->castle_perm & BKCA) ? 'k' : '-',
	   (the_board->castle_perm & BQCA) ? 'q' : '-');

    printf("PosKey:\t0x%016llx\n", the_board->board_hash);

    printf("\n\n");

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
	char * square_text = (char *)malloc(3 * sizeof(char));

    int file = GET_FILE(sq);
    int rank = GET_RANK(sq);

    sprintf(square_text, "%c%c", ('a' + file), ('1' + rank));

    return square_text;
}





/*
 * Validates the contents of a board struct.
 *
 * name: ASSERT_BOARD_OK
 * @param
 * @return
 *
 */

bool ASSERT_BOARD_OK(const struct board * brd)
{

    // check bit boards
    U64 conflated = 0;

    for (int i = 0; i < NUM_PIECES; i++) {
		conflated |= brd->bitboards[i];
    }
    assert(conflated == brd->board);

    // check where Kings are
    for (enum square sq = a1; sq <= h8; sq++) {
		enum piece pce = get_piece_at_square(brd, sq);
		if (pce != NO_PIECE) {
			if (pce == W_KING) {

				U64 bb_wk = brd->bitboards[W_KING];
				enum square wk_sq = POP(&bb_wk);

				assert(sq == wk_sq);
			} else if (pce == B_KING) {

				U64 bb_bk = brd->bitboards[B_KING];
				enum square bk_sq = POP(&bb_bk);

				assert(sq == bk_sq);
			}
		}
    }

    // check verbose representation of board
    for (enum square sq = 0; sq < NUM_SQUARES; sq++) {
		enum piece pce = get_piece_at_square(brd, sq);
		if (pce != NO_PIECE) {
			assert(pce == brd->pieces[sq]);
		}
    }


    assert(brd->en_passant == NO_SQUARE
	   || (GET_RANK(brd->en_passant) == RANK_6
	       && brd->side_to_move == WHITE)
	   || (GET_RANK(brd->en_passant) == RANK_3
	       && brd->side_to_move == BLACK));


    // calc and verify the material count
    U32 local_material[NUM_COLOURS] = { 0 };
    for (enum square sq = 0; sq <= h8; sq++) {
		enum piece pce = get_piece_at_square(brd, sq);
		if (pce != NO_PIECE) {
			enum colour col = GET_COLOUR(pce);
			local_material[col] += get_piece_value(pce);
		}
    }

	if (local_material[WHITE] != brd->material[WHITE]){
		printf("--------------------------------------\n");
		print_board(brd);
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
void assert_boards_are_equal(const struct board * brd1, const struct board * brd2){

	for(int i = 0; i < NUM_PIECES; i++){
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


	for(int i = 0; i < NUM_SQUARES; i++){
		assert(brd1->pieces[i] == brd2->pieces[i]);
	}

	assert(brd1->castle_perm == brd2->castle_perm);

	// already verified that brd1->history_ply == brd2_history_ply
	for(int i = 0; i < brd1->history_ply; i++){
		assert(brd1->history[i].move == brd2->history[i].move);
		assert(brd1->history[i].fifty_move_counter == brd2->history[i].fifty_move_counter);
		assert(brd1->history[i].castle_perm == brd2->history[i].castle_perm);
		assert(brd1->history[i].board_hash == brd2->history[i].board_hash);
		assert(brd1->history[i].en_passant == brd2->history[i].en_passant);
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
struct board * clone_board(const struct board * board_to_clone){
	
	U16 size = sizeof(struct board);
	
    struct board *brd = malloc(size);

	memcpy(brd, board_to_clone, size);

	return brd;
}




