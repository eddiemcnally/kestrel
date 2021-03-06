/*
 * fen.c
 *
 * ---------------------------------------------------------------------
 * Description : Code to parse a FEN string and convert it into a
 * populated board struct.
 * ---------------------------------------------------------------------
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
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "kestrel.h"
#include "pieces.h"
#include "board.h"
#include "hashkeys.h"
#include "board_utils.h"
#include "fen/fen.h"

/*
 *
 * name: Takes a char array as FEN notation, and returns a populated
 * board
 * @param 	char * representing the FEN string
 * 			board * representing the bpard tp be populated
 * @return 	0 = OK, -1 Error
 *
 * Thanks for BlueFever Software for his youtube videos and this code
 */

void consume_fen_notation(const char *fen_string, struct position *pos)
{

    int rank = RANK_8;
    int file = FILE_A;
    int count = 0;

    while ((rank >= RANK_1) && *fen_string) {
        enum piece piece_to_add = NO_PIECE;
        count = 1;

        switch (*fen_string) {
        case 'p':
            piece_to_add = B_PAWN;
            break;
        case 'r':
            piece_to_add = B_ROOK;
            break;
        case 'n':
            piece_to_add = B_KNIGHT;
            break;
        case 'b':
            piece_to_add = B_BISHOP;
            break;
        case 'q':
            piece_to_add = B_QUEEN;
            break;
        case 'k':
            piece_to_add = B_KING;
            break;
        case 'P':
            piece_to_add = W_PAWN;
            break;
        case 'R':
            piece_to_add = W_ROOK;
            break;
        case 'N':
            piece_to_add = W_KNIGHT;
            break;
        case 'B':
            piece_to_add = W_BISHOP;
            break;
        case 'Q':
            piece_to_add = W_QUEEN;
            break;
        case 'K':
            piece_to_add = W_KING;
            break;

        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
            count = (*fen_string) - '0';
            break;

        case '/':
        case ' ':
            rank--;
            file = FILE_A;
            fen_string++;
            continue;

        default:
            printf("FEN error \n");
            return;
        }

        for (int i = 0; i < count; i++) {
            if (piece_to_add != NO_PIECE) {
                enum square sq = get_square((enum rank)rank, (enum file)file);
                add_piece_to_board(pos, piece_to_add, sq);
            }
            file++;
        }
        fen_string++;
    }

    assert((*fen_string == 'w') || (*fen_string == 'b'));
    if (*fen_string == 'w') {
		set_side_to_move(pos, WHITE);
    } else {
		set_side_to_move(pos, BLACK);
    }

    // skip 'w' or 'b', and the next space
    fen_string += 2;

    for (int i = 0; i < 4; i++) {
        if (*fen_string == ' ') {
            break;
        }

        switch (*fen_string) {
        case 'K':
			set_castle_permission(pos, WKCA);
            break;
        case 'Q':
			set_castle_permission(pos, WQCA);
            break;
        case 'k':
			set_castle_permission(pos, BKCA);
            break;
        case 'q':
			set_castle_permission(pos, BQCA);
            break;
        default:
            break;
        }
        fen_string++;
    }

    fen_string++;

    if (*fen_string != '-') {
        // en passant square present
        file = fen_string[0] - 'a';
        rank = fen_string[1] - '1';

        assert(IS_VALID_FILE(file));
        assert(IS_VALID_RANK(rank));

		set_en_passant_sq(pos, get_square((enum rank)rank, (enum file)file));
    } else {
		set_en_passant_sq(pos, NO_SQUARE);
    }

	update_board_hash(pos);

}
