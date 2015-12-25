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






// function to map move attributes to a bitmapped field
// see typdef for mv_bitmap for a description
inline mv_bitmap MOVE(enum square from, enum square to, enum piece capture,
		      enum piece promote, uint64_t flags, uint32_t score)
{
	return (  ((uint64_t)from 		<< MV_MASK_OFF_FROM_SQ) 
			| ((uint64_t)to 		<< MV_MASK_OFF_TO_SQ) 	 
			| ((uint64_t)capture 	<< MV_MASK_OFF_CAPTURED_PCE)  
			| ((uint64_t)promote 	<< MV_MASK_OFF_PROMOTED_PCE) 
			| flags
			+ score
	  );
}



inline uint32_t get_score(mv_bitmap mv){
	return (uint32_t)(mv & MV_MASK_SCORE);
}

inline uint64_t get_move(mv_bitmap mv){
	return (uint64_t)(mv & MV_MASK_MOVE);
}

inline void add_to_score(uint64_t *score, uint32_t to_add){
	*score = *score + to_add;
}


inline void Set

/*
 * Returns the piece type on the given square
 *
 * name: 	get_piece_at_square
 * @param	the board container and the square to test
 * @return	the piece or NO_PIECE
 *
 */

inline enum piece get_piece_at_square(const struct board *the_board,
				      enum square sq)
{
	return the_board->pieces[sq];
}

inline struct move_list *get_empty_move_list()
{
	struct move_list *list = malloc(sizeof(struct move_list));
	memset(list, 0, sizeof(struct move_list));
	return list;
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
	clear_bit(bb, bit);
	return bit;
}



/* Reverse the bits in a word
 *
 * name: reverse_bits
 * @param
 * @return
 *
 */
inline uint64_t reverse_bits(uint64_t word)
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
		struct move m = mvl->moves[i];

		enum square from = FROMSQ(m.move_bitmap);
		enum square to = TOSQ(m.move_bitmap);
		enum piece capt = CAPTURED_PCE(m.move_bitmap);
		enum piece promote = PROMOTED_PCE(m.move_bitmap);

		assert(is_valid_piece(capt));
		assert(is_valid_piece(promote));

		assert(from >= a1 && from <= h8);
		assert(to >= a1 && to <= h8);

		assert(m.score == 0);
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

void print_move_details(mv_bitmap move_bitmap, uint32_t score)
{
	int from_file = GET_FILE(FROMSQ(move_bitmap));
	int from_rank = GET_RANK(FROMSQ(move_bitmap));

	int to_file = GET_FILE(TOSQ(move_bitmap));
	int to_rank = GET_RANK(TOSQ(move_bitmap));

	enum piece captured = CAPTURED_PCE(move_bitmap);
	enum piece promoted = PROMOTED_PCE(move_bitmap);

	char c_capt = get_piece_label(captured);
	char c_promoted = get_piece_label(promoted);

	printf("%c%c%c%c, captured '%c' promote '%c' score %d\n",
	       ('a' + from_file), ('1' + from_rank), ('a' + to_file),
	       ('1' + to_rank), c_capt, c_promoted, score);

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
		mv_bitmap mv = list->moves[i].move_bitmap;
		uint32_t score = list->moves[i].score;

		print_move_details(mv, score);
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
		uint32_t move = list->moves[i].move_bitmap;
		//uint32_t score = list->moves[i].score;

		printf("%s\n", print_move(move));
	}
	//printf("MoveList Total %d Moves:\n\n", list->move_count);
}


