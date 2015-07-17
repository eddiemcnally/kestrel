/*
 * move.c
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
#include <stdlib.h>
#include "types.h"
#include "board.h"
#include "pieces.h"
#include "move.h"

static void add_white_pawn_capture_move(const struct board *brd,enum square from, enum square to,
					enum piece capture,	struct move_list *mvl);
static void add_white_pawn_move(const struct board *brd, enum square from,
				enum square to, enum piece capture, struct move_list *mvl);
static void add_quiet_move(const struct board *brd, int move_bitmap, struct move_list *mvlist);
static void add_capture_move(const struct board *brd, int move_bitmap, struct move_list *mvlist);
static void add_en_passent_move(const struct board *brd, int move_bitmap, struct move_list *mvlist);


/* man function for taking a board and returning a populated
 * move list for all pieces
 *
 * name: generate_all_moves
 * @param
 * @return
 *
 */
void generate_all_moves(const struct board *brd, struct move_list *mvl)
{
    if (brd->side_to_move == WHITE) {
		generate_white_pawn_moves(brd, mvl);
    }
}

static void add_quiet_move(const struct board *brd, int move_bitmap,
		    struct move_list *mvlist)
{
    mvlist->moves[mvlist->move_count].move_bitmap = move_bitmap;
    mvlist->moves[mvlist->move_count].score = 0;
    mvlist->move_count++;
}

static void add_capture_move(const struct board *brd, int move_bitmap,
		      struct move_list *mvlist)
{
	mvlist->moves[mvlist->move_count].move_bitmap = move_bitmap;
    mvlist->moves[mvlist->move_count].score = 0;
    mvlist->move_count++;
}

static void add_en_passent_move(const struct board *brd, int move_bitmap,
			 struct move_list *mvlist)
{
    mvlist->moves[mvlist->move_count].move_bitmap = move_bitmap;
    mvlist->moves[mvlist->move_count].score = 0;
    mvlist->move_count++;
}

static void add_white_pawn_capture_move(const struct board *brd,
					enum square from, enum square to,
					enum piece capture,
					struct move_list *mvl)
{
    if (GET_RANK(from) == RANK_7) {
		// pawn can promote to 4 pieces
		add_capture_move(brd, MOVE(from, to, capture, W_QUEEN, 0), mvl);
		add_capture_move(brd, MOVE(from, to, capture, W_ROOK, 0), mvl);
		add_capture_move(brd, MOVE(from, to, capture, W_BISHOP, 0), mvl);
		add_capture_move(brd, MOVE(from, to, capture, W_KNIGHT, 0), mvl);
    } else {
		add_capture_move(brd, MOVE(from, to, capture, NO_PIECE, 0), mvl);
    }
}

static void add_white_pawn_move(const struct board *brd, enum square from,
				enum square to, enum piece capture,
				struct move_list *mvl)
{
	if (GET_RANK(from) == RANK_7) {
		// pawn can promote to 4 pieces
		add_capture_move(brd, MOVE(from, to, NO_PIECE, W_QUEEN, 0), mvl);
		add_capture_move(brd, MOVE(from, to, NO_PIECE, W_ROOK, 0), mvl);
		add_capture_move(brd, MOVE(from, to, NO_PIECE, W_BISHOP, 0), mvl);
		add_capture_move(brd, MOVE(from, to, NO_PIECE, W_KNIGHT, 0), mvl);
	} else {
		add_capture_move(brd, MOVE(from, to, NO_PIECE, NO_PIECE, 0), mvl);
    }
}

void generate_white_pawn_moves(const struct board *brd, struct move_list *mvl)
{
    // get the bitboard representing all WHITE pawns
    // on the board
    U64 bbPawn = brd->bitboards[W_PAWN];

    while (bbPawn != 0) {

		printf("bbPawn:\t0x%016llx\n", bbPawn);

		enum square pawn_sq = POP(&bbPawn);

		//int pawn_rank = GET_RANK(pawn_sq);
		int pawn_file = GET_FILE(pawn_sq);
		int pawn_rank = GET_RANK(pawn_sq);
		enum square next_sq_1 = pawn_sq + 8;

		if (is_square_occupied(brd->board, next_sq_1) == false) {
			add_white_pawn_move(brd, pawn_sq, next_sq_1, NO_PIECE, mvl);

			if (pawn_rank == RANK_2) {
				enum square next_sq_2 = pawn_sq + 16;
				bool sq_2_occupied = is_square_occupied(brd->board, next_sq_2);
				if (sq_2_occupied == false) {
					add_quiet_move(brd,
					   MOVE(pawn_sq, next_sq_2,
						NO_PIECE, NO_PIECE,
						MFLAG_PAWN_START), mvl);
				}
			}
		}

		// check for capture left
		if (pawn_file >= FILE_B) {
			enum square cap_sq = pawn_sq + 7;
			enum piece pce = get_piece_at_square(brd, cap_sq);

			if ((pce != NO_PIECE) && (get_colour(pce) == BLACK)) {
				add_white_pawn_capture_move(brd, pawn_sq, cap_sq, pce, mvl);
			}

			if (cap_sq == brd->en_passant) {
				add_white_pawn_capture_move(brd, pawn_sq, cap_sq, pce, mvl);
			}
		}

		// check for capture right
		if (pawn_file <= FILE_G) {
			enum square cap_sq = pawn_sq + 9;
			enum piece pce = get_piece_at_square(brd, cap_sq);

			if ((pce != NO_PIECE) && (get_colour(pce) == BLACK)) {
				add_white_pawn_capture_move(brd, pawn_sq, cap_sq, pce, mvl);
			}

			if (cap_sq == brd->en_passant) {
				add_white_pawn_capture_move(brd, pawn_sq, cap_sq, pce, mvl);
			}
		}
    }
}

/*
 * Prints out the algebraic notatio of a move (eg, a2a4)
 * name: print_move
 * @param
 * @return
 *
 */
char *print_move(U32 move_bitmap)
{

    static char move_string[6];

    int from_file = GET_FILE(FROMSQ(move_bitmap));
    int from_rank = GET_RANK(FROMSQ(move_bitmap));

    int to_file = GET_FILE(TOSQ(move_bitmap));
    int to_rank = GET_RANK(TOSQ(move_bitmap));

    enum piece promoted_pce = PROMOTED(move_bitmap);

    if (promoted_pce != NO_PIECE) {
		char pchar = 'q';
		if (isKn(promoted_pce)) {
			pchar = 'n';
		} else if (isR(promoted_pce)) {
			pchar = 'r';
		} else if (isB(promoted_pce)) {
			pchar = 'b';
		}
		sprintf(move_string, "%c%c%c%c%c", ('a' + from_file),
				('1' + from_rank), ('a' + to_file), ('1' + to_rank), pchar);
	} else {
		sprintf(move_string, "%c%c%c%c", ('a' + from_file),
				('1' + from_rank), ('a' + to_file), ('1' + to_rank));
	}

    return move_string;
}

/*
 * Prints out the move list
 * name: print_move_list
 * @param
 * @return
 *
 */

void print_move_list(const struct move_list *list)
{
    printf("MoveList:\n");

    for (int i = 0; i < list->move_count; i++) {
		U32 move = list->moves[i].move_bitmap;
		U32 score = list->moves[i].score;

		printf("Move:%d > %s (score:%d)\n", (i + 1), print_move(move), score);
    }
    printf("MoveList Total %d Moves:\n\n", list->move_count);
}
