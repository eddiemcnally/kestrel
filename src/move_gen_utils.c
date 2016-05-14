/*
 * move_gen_utils.c
 *
 * ---------------------------------------------------------------------
 * DESCRIPTION: utility functions to support the move generation code.
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
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include "types.h"
#include "pieces.h"
#include "board.h"
#include "board_utils.h"
#include "move_gen_utils.h"

#define R2(n)     n,     n + 2*64,     n + 1*64,     n + 3*64
#define R4(n) R2(n), R2(n + 2*16), R2(n + 1*16), R2(n + 3*16)
#define R6(n) R4(n), R4(n + 2*4 ), R4(n + 1*4 ), R4(n + 3*4 )


static const unsigned char BitReverseTable256[256] = {
    R6(0), R6(2), R6(1), R6(3)
};




inline uint32_t get_score(mv_bitmap mv)
{
    return (uint32_t)(mv & MV_MASK_SCORE);
}

inline mv_bitmap get_move(mv_bitmap mv)
{
    return (mv_bitmap)(mv & MV_MASK_MOVE);
}

inline void add_to_score(mv_bitmap *mv, uint32_t to_add)
{
    uint32_t score = get_score(*mv);

    score += to_add;

    *mv |= ((uint32_t)(score) & MV_MASK_SCORE);
}



/*
 * Clears the LSB of the board, and returns the bit # that was cleared.
 * name: pop_1st_bit
 * @param	ptr to uint64_t
 * @return	index of bit cleared.
 *
 * uses gcc built-in function (see https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html)
 *
 */
inline uint8_t pop_1st_bit(uint64_t * bb)
{
    uint8_t bit = (uint8_t) __builtin_ctzll(*bb);

    // clear the bit
    *bb = *bb & (uint64_t) (~(0x01ull << bit));
    return bit;
}






/* Reverse the bits in a word
 *
 * name: reverse_bits
 * @param
 * @return
 *
 */
uint64_t reverse_bits(uint64_t word)
{
    uint64_t retval = 0;

    uint8_t *p_in = (uint8_t *) & word;
    uint8_t *p_out = (uint8_t *) & retval;
    // reverse the bits in each byte
    for (int i = 0; i < 8; i++) {
        *p_out = (uint8_t)BitReverseTable256[*p_in];
        p_out++;
        p_in++;
    }

    // now reverse the bytes
    return __builtin_bswap64(retval);
}



inline bool is_move_in_list(struct move_list *mvl, mv_bitmap mv)
{
    mv_bitmap mv_no_score = mv >> MV_MASK_OFF_FROM_SQ;

    for (int i = 0; i < mvl->move_count; i++) {
        mv_bitmap temp = mvl->moves[i] >> MV_MASK_OFF_FROM_SQ;

        if (temp == mv_no_score) {
            return true;
        }
    }
    return false;
}



void validate_move_list(struct move_list *mvl)
{
    assert(mvl->move_count < MAX_POSITION_MOVES);

    for (int i = 0; i < mvl->move_count; i++) {
        mv_bitmap m = mvl->moves[i];

        enum square from = FROMSQ(m);
        enum square to = TOSQ(m);
        enum piece capt = CAPTURED_PCE(m);
        enum piece promote = PROMOTED_PCE(m);

        assert(is_valid_piece(capt));
        assert(is_valid_piece(promote));

        assert(from >= a1 && from <= h8);
        assert(to >= a1 && to <= h8);

        assert(get_score(m) == 0);
    }
}


/*
 * Prints out the algebraic notation of a move (eg, a2a4)
 * name: print_move
 * @param
 * @return
 *
 */
char *print_move(mv_bitmap move_bitmap)
{
    static char move_string[6];

    int from_file = GET_FILE(FROMSQ(move_bitmap));
    int from_rank = GET_RANK(FROMSQ(move_bitmap));

    int to_file = GET_FILE(TOSQ(move_bitmap));
    int to_rank = GET_RANK(TOSQ(move_bitmap));

    enum piece promoted_pce = PROMOTED_PCE(move_bitmap);

    if (promoted_pce != NO_PIECE) {
        char pchar = 'q';
        if (IS_KNIGHT(promoted_pce)) {
            pchar = 'n';
        } else if (IS_ROOK(promoted_pce)) {
            pchar = 'r';
        } else if (IS_BISHOP(promoted_pce)) {
            pchar = 'b';
        }
        sprintf(move_string, "%c%c%c%c%c", ('a' + from_file),
                ('1' + from_rank), ('a' + to_file), ('1' + to_rank),
                pchar);
    } else {
        sprintf(move_string, "%c%c%c%c", ('a' + from_file),
                ('1' + from_rank), ('a' + to_file), ('1' + to_rank));
    }

    return move_string;
}


void print_board_and_move(struct board *brd, mv_bitmap move_bitmap)
{
	print_board(brd);
	print_move_details(move_bitmap);	
}

void print_move_details(mv_bitmap move_bitmap)
{
    int from_file = GET_FILE(FROMSQ(move_bitmap));
    int from_rank = GET_RANK(FROMSQ(move_bitmap));

    int to_file = GET_FILE(TOSQ(move_bitmap));
    int to_rank = GET_RANK(TOSQ(move_bitmap));

    enum piece captured = CAPTURED_PCE(move_bitmap);
    enum piece promoted = PROMOTED_PCE(move_bitmap);

    char c_capt = get_piece_label(captured);
    char c_promoted = get_piece_label(promoted);

    uint32_t score = get_score(move_bitmap);

    printf("%c%c%c%c, captured '%c' promote '%c' score %d IsCapt %d \n",
           ('a' + from_file), ('1' + from_rank), ('a' + to_file),
           ('1' + to_rank), c_capt, c_promoted, score, IS_CAPTURE_MOVE(move_bitmap));

}

/*
 * Prints out the move list details
 * name: print_move_list_details
 * @param
 * @return
 *
 */


void print_move_list_details(const struct move_list *list)
{
    printf("MoveList Details: (%d)\n", list->move_count);

    for (int i = 0; i < list->move_count; i++) {
        mv_bitmap mv = list->moves[i];
        print_move_details(mv);
    }
    printf("MoveList Total %d Moves:\n\n", list->move_count);
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
        mv_bitmap move = list->moves[i];

        printf("%s\n", print_move(move));
    }
    //printf("MoveList Total %d Moves:\n\n", list->move_count);
}





