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
		int rank = GET_RANK(the_board->en_passant);
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
 * @return
 *
 */
char *print_square(enum square sq)
{

    static char square_text[3];

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
    for (enum square sq = 0; sq < NUM_SQUARES; sq++) {
		enum piece pce = get_piece_at_square(brd, sq);
		if (pce != NO_PIECE) {
			if (pce == W_KING) {
				assert(sq == brd->king_squares[WHITE]);
			} else if (pce == B_KING) {
				assert(sq == brd->king_squares[BLACK]);
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

    // check number of pieces on board
    // -------------------------------
    U8 pce_num[NUM_PIECES] = { 0 };
    for (enum square sq = 0; sq < NUM_SQUARES; sq++) {
		enum piece pce = get_piece_at_square(brd, sq);
		if (pce != NO_PIECE) {
			pce_num[pce]++;
		}
    }
    for (int i = 0; i < NUM_PIECES; i++) {
		assert(pce_num[i] == brd->pce_num[i]);
    }
    for (int i = 0; i < NUM_PIECES; i++) {
		assert(pce_num[i] == CNT(brd->bitboards[i]));
    }

    assert(brd->en_passant == NO_SQUARE
	   || (GET_RANK(brd->en_passant) == RANK_6
	       && brd->side_to_move == WHITE)
	   || (GET_RANK(brd->en_passant) == RANK_3
	       && brd->side_to_move == BLACK));

    // check on big, major and minor piece count
    U8 big_pieces[NUM_COLOURS] = { 0 };
    U8 major_pieces[NUM_COLOURS] = { 0 };
    U8 minor_pieces[NUM_COLOURS] = { 0 };
    for (enum square sq = 0; sq < NUM_SQUARES; sq++) {
		enum piece pce = get_piece_at_square(brd, sq);
		if (pce != NO_PIECE) {
			enum colour col = get_colour(pce);
			if (is_big_piece(pce)) {
				big_pieces[col] += 1;
			}
			if (is_major_piece(pce)) {
				major_pieces[col] += 1;
			}
			if (is_minor_piece(pce)) {
				minor_pieces[col] += 1;
			}
		}
    }
    assert(big_pieces[WHITE] == brd->big_pieces[WHITE]);
    assert(big_pieces[BLACK] == brd->big_pieces[BLACK]);
    assert(major_pieces[WHITE] == brd->major_pieces[WHITE]);
    assert(major_pieces[BLACK] == brd->major_pieces[BLACK]);
    assert(minor_pieces[WHITE] == brd->minor_pieces[WHITE]);
    assert(minor_pieces[BLACK] == brd->minor_pieces[BLACK]);

    // calc and verify the material count
    U16 material[NUM_COLOURS] = { 0 };
    for (enum square sq = 0; sq < NUM_SQUARES; sq++) {
		enum piece pce = get_piece_at_square(brd, sq);
		if (pce != NO_PIECE) {
			enum colour col = get_colour(pce);
			material[col] += piece_values[pce];
		}
    }
    assert(material[WHITE] == brd->material[WHITE]);
    assert(material[BLACK] == brd->material[BLACK]);

    // check on position key
    assert(brd->board_hash == get_position_hashkey(brd));

    return true;

}
