/*
 * pieces.c
 * Copyright (C) 2015 Eddie McNally <emcn@gmx.com>
 * 
 * Chess is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Chess is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "board.h"
#include "pieces.h"

/**
 * Takes a typed piece and returns a human-readable representation
 */
char get_piece_label(piece_id_t piece) {

    switch (piece) {

        case W_PAWN:
            return 'P';
        case W_ROOK:
            return 'R';
        case W_KNIGHT:
            return 'N';
        case W_BISHOP:
            return 'B';
        case W_QUEEN:
            return 'Q';
        case W_KING:
            return 'K';
        case B_PAWN:
            return 'p';
        case B_ROOK:
            return 'r';
        case B_KNIGHT:
            return 'n';
        case B_BISHOP:
            return 'b';
        case B_QUEEN:
            return 'q';
        case B_KING:
            return 'k';
        default:
            return 'x';
    }
}



