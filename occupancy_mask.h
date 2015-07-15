/*
 * occupancy_mask.h
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

#ifndef _OCCUPANCY_MASK_H_
#define _OCCUPANCY_MASK_H_

#include "types.h"

U64 get_occ_mask(enum piece pce, enum square sq);
void print_occupancy_masks(enum piece pce);
void print_occupancy_mask_as_board(const U64 * mask, enum piece pce,
				   enum square square);
void print_out_masks(const U64 * masks);

// elem [0] is a1, [63] is h8
static const U64 black_pawn_occupancy_masks[NUM_SQUARES] = {
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000002,
	0x0000000000000005,
	0x000000000000000a,
	0x0000000000000014,
	0x0000000000000028,
	0x0000000000000050,
	0x00000000000000a0,
	0x0000000000000040,
	0x0000000000000200,
	0x0000000000000500,
	0x0000000000000a00,
	0x0000000000001400,
	0x0000000000002800,
	0x0000000000005000,
	0x000000000000a000,
	0x0000000000004000,
	0x0000000000020000,
	0x0000000000050000,
	0x00000000000a0000,
	0x0000000000140000,
	0x0000000000280000,
	0x0000000000500000,
	0x0000000000a00000,
	0x0000000000400000,
	0x0000000002000000,
	0x0000000005000000,
	0x000000000a000000,
	0x0000000014000000,
	0x0000000028000000,
	0x0000000050000000,
	0x00000000a0000000,
	0x0000000040000000,
	0x0000000200000000,
	0x0000000500000000,
	0x0000000a00000000,
	0x0000001400000000,
	0x0000002800000000,
	0x0000005000000000,
	0x000000a000000000,
	0x0000004000000000,
	0x0000020000000000,
	0x0000050000000000,
	0x00000a0000000000,
	0x0000140000000000,
	0x0000280000000000,
	0x0000500000000000,
	0x0000a00000000000,
	0x0000400000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000
};

// elem [0] is a1, [63] is h8
static const U64 white_pawn_occupancy_masks[NUM_SQUARES] = {
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000020000,
	0x0000000000050000,
	0x00000000000a0000,
	0x0000000000140000,
	0x0000000000280000,
	0x0000000000500000,
	0x0000000000a00000,
	0x0000000000400000,
	0x0000000002000000,
	0x0000000005000000,
	0x000000000a000000,
	0x0000000014000000,
	0x0000000028000000,
	0x0000000050000000,
	0x00000000a0000000,
	0x0000000040000000,
	0x0000000200000000,
	0x0000000500000000,
	0x0000000a00000000,
	0x0000001400000000,
	0x0000002800000000,
	0x0000005000000000,
	0x000000a000000000,
	0x0000004000000000,
	0x0000020000000000,
	0x0000050000000000,
	0x00000a0000000000,
	0x0000140000000000,
	0x0000280000000000,
	0x0000500000000000,
	0x0000a00000000000,
	0x0000400000000000,
	0x0002000000000000,
	0x0005000000000000,
	0x000a000000000000,
	0x0014000000000000,
	0x0028000000000000,
	0x0050000000000000,
	0x00a0000000000000,
	0x0040000000000000,
	0x0200000000000000,
	0x0500000000000000,
	0x0a00000000000000,
	0x1400000000000000,
	0x2800000000000000,
	0x5000000000000000,
	0xa000000000000000,
	0x4000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000000
};

// elem [0] is a1, [63] is h8
static const U64 knight_occupancy_masks[NUM_SQUARES] = {
	0x0000000000020400,
	0x0000000000050800,
	0x00000000000a1100,
	0x0000000000142200,
	0x0000000000284400,
	0x0000000000508800,
	0x0000000000a01000,
	0x0000000000402000,
	0x0000000002040004,
	0x0000000005080008,
	0x000000000a110011,
	0x0000000014220022,
	0x0000000028440044,
	0x0000000050880088,
	0x00000000a0100010,
	0x0000000040200020,
	0x0000000204000402,
	0x0000000508000805,
	0x0000000a1100110a,
	0x0000001422002214,
	0x0000002844004428,
	0x0000005088008850,
	0x000000a0100010a0,
	0x0000004020002040,
	0x0000020400040200,
	0x0000050800080500,
	0x00000a1100110a00,
	0x0000142200221400,
	0x0000284400442800,
	0x0000508800885000,
	0x0000a0100010a000,
	0x0000402000204000,
	0x0002040004020000,
	0x0005080008050000,
	0x000a1100110a0000,
	0x0014220022140000,
	0x0028440044280000,
	0x0050880088500000,
	0x00a0100010a00000,
	0x0040200020400000,
	0x0204000402000000,
	0x0508000805000000,
	0x0a1100110a000000,
	0x1422002214000000,
	0x2844004428000000,
	0x5088008850000000,
	0xa0100010a0000000,
	0x4020002040000000,
	0x0400040200000000,
	0x0800080500000000,
	0x1100110a00000000,
	0x2200221400000000,
	0x4400442800000000,
	0x8800885000000000,
	0x100010a000000000,
	0x2000204000000000,
	0x0004020000000000,
	0x0008050000000000,
	0x00110a0000000000,
	0x0022140000000000,
	0x0044280000000000,
	0x0088500000000000,
	0x0010a00000000000,
	0x0020400000000000
};

static const U64 bishop_occupancy_masks[NUM_SQUARES] = {
	0x8040201008040200,
	0x0080402010080500,
	0x0000804020110a00,
	0x0000008041221400,
	0x0000000182442800,
	0x0000010204885000,
	0x000102040810a000,
	0x0102040810204000,
	0x4020100804020002,
	0x8040201008050005,
	0x00804020110a000a,
	0x0000804122140014,
	0x0000018244280028,
	0x0001020488500050,
	0x0102040810a000a0,
	0x0204081020400040,
	0x2010080402000204,
	0x4020100805000508,
	0x804020110a000a11,
	0x0080412214001422,
	0x0001824428002844,
	0x0102048850005088,
	0x02040810a000a010,
	0x0408102040004020,
	0x1008040200020408,
	0x2010080500050810,
	0x4020110a000a1120,
	0x8041221400142241,
	0x0182442800284482,
	0x0204885000508804,
	0x040810a000a01008,
	0x0810204000402010,
	0x0804020002040810,
	0x1008050005081020,
	0x20110a000a112040,
	0x4122140014224180,
	0x8244280028448201,
	0x0488500050880402,
	0x0810a000a0100804,
	0x1020400040201008,
	0x0402000204081020,
	0x0805000508102040,
	0x110a000a11204080,
	0x2214001422418000,
	0x4428002844820100,
	0x8850005088040201,
	0x10a000a010080402,
	0x2040004020100804,
	0x0200020408102040,
	0x0500050810204080,
	0x0a000a1120408000,
	0x1400142241800000,
	0x2800284482010000,
	0x5000508804020100,
	0xa000a01008040201,
	0x4000402010080402,
	0x0002040810204080,
	0x0005081020408000,
	0x000a112040800000,
	0x0014224180000000,
	0x0028448201000000,
	0x0050880402010000,
	0x00a0100804020100,
	0x0040201008040201
};

static const U64 queen_occupancy_masks[NUM_SQUARES] = {
	0x81412111090503fe,
	0x02824222120a07fd,
	0x0404844424150efb,
	0x08080888492a1cf7,
	0x10101011925438ef,
	0x2020212224a870df,
	0x404142444850e0bf,
	0x8182848890a0c07f,
	0x412111090503fe03,
	0x824222120a07fd07,
	0x04844424150efb0e,
	0x080888492a1cf71c,
	0x101011925438ef38,
	0x20212224a870df70,
	0x4142444850e0bfe0,
	0x82848890a0c07fc0,
	0x2111090503fe0305,
	0x4222120a07fd070a,
	0x844424150efb0e15,
	0x0888492a1cf71c2a,
	0x1011925438ef3854,
	0x212224a870df70a8,
	0x42444850e0bfe050,
	0x848890a0c07fc0a0,
	0x11090503fe030509,
	0x22120a07fd070a12,
	0x4424150efb0e1524,
	0x88492a1cf71c2a49,
	0x11925438ef385492,
	0x2224a870df70a824,
	0x444850e0bfe05048,
	0x8890a0c07fc0a090,
	0x090503fe03050911,
	0x120a07fd070a1222,
	0x24150efb0e152444,
	0x492a1cf71c2a4988,
	0x925438ef38549211,
	0x24a870df70a82422,
	0x4850e0bfe0504844,
	0x90a0c07fc0a09088,
	0x0503fe0305091121,
	0x0a07fd070a122242,
	0x150efb0e15244484,
	0x2a1cf71c2a498808,
	0x5438ef3854921110,
	0xa870df70a8242221,
	0x50e0bfe050484442,
	0xa0c07fc0a0908884,
	0x03fe030509112141,
	0x07fd070a12224282,
	0x0efb0e1524448404,
	0x1cf71c2a49880808,
	0x38ef385492111010,
	0x70df70a824222120,
	0xe0bfe05048444241,
	0xc07fc0a090888482,
	0xfe03050911214181,
	0xfd070a1222428202,
	0xfb0e152444840404,
	0xf71c2a4988080808,
	0xef38549211101010,
	0xdf70a82422212020,
	0xbfe0504844424140,
	0x7fc0a09088848281
};

// elem [0] is a1, [63] is h8
static const U64 king_occupancy_masks[NUM_SQUARES] = {

	0x0000000000000302,
	0x0000000000000705,
	0x0000000000000e0a,
	0x0000000000001c14,
	0x0000000000003828,
	0x0000000000007050,
	0x000000000000e0a0,
	0x000000000000c040,
	0x0000000000030203,
	0x0000000000070507,
	0x00000000000e0a0e,
	0x00000000001c141c,
	0x0000000000382838,
	0x0000000000705070,
	0x0000000000e0a0e0,
	0x0000000000c040c0,
	0x0000000003020300,
	0x0000000007050700,
	0x000000000e0a0e00,
	0x000000001c141c00,
	0x0000000038283800,
	0x0000000070507000,
	0x00000000e0a0e000,
	0x00000000c040c000,
	0x0000000302030000,
	0x0000000705070000,
	0x0000000e0a0e0000,
	0x0000001c141c0000,
	0x0000003828380000,
	0x0000007050700000,
	0x000000e0a0e00000,
	0x000000c040c00000,
	0x0000030203000000,
	0x0000070507000000,
	0x00000e0a0e000000,
	0x00001c141c000000,
	0x0000382838000000,
	0x0000705070000000,
	0x0000e0a0e0000000,
	0x0000c040c0000000,
	0x0003020300000000,
	0x0007050700000000,
	0x000e0a0e00000000,
	0x001c141c00000000,
	0x0038283800000000,
	0x0070507000000000,
	0x00e0a0e000000000,
	0x00c040c000000000,
	0x0302030000000000,
	0x0705070000000000,
	0x0e0a0e0000000000,
	0x1c141c0000000000,
	0x3828380000000000,
	0x7050700000000000,
	0xe0a0e00000000000,
	0xc040c00000000000,
	0x0203000000000000,
	0x0507000000000000,
	0x0a0e000000000000,
	0x141c000000000000,
	0x2838000000000000,
	0x5070000000000000,
	0xa0e0000000000000,
	0x40c0000000000000
};

static const U64 rook_occupancy_masks[NUM_SQUARES] = {
	0x01010101010101fe,
	0x02020202020202fd,
	0x04040404040404fb,
	0x08080808080808f7,
	0x10101010101010ef,
	0x20202020202020df,
	0x40404040404040bf,
	0x808080808080807f,
	0x010101010101fe01,
	0x020202020202fd02,
	0x040404040404fb04,
	0x080808080808f708,
	0x101010101010ef10,
	0x202020202020df20,
	0x404040404040bf40,
	0x8080808080807f80,
	0x0101010101fe0101,
	0x0202020202fd0202,
	0x0404040404fb0404,
	0x0808080808f70808,
	0x1010101010ef1010,
	0x2020202020df2020,
	0x4040404040bf4040,
	0x80808080807f8080,
	0x01010101fe010101,
	0x02020202fd020202,
	0x04040404fb040404,
	0x08080808f7080808,
	0x10101010ef101010,
	0x20202020df202020,
	0x40404040bf404040,
	0x808080807f808080,
	0x010101fe01010101,
	0x020202fd02020202,
	0x040404fb04040404,
	0x080808f708080808,
	0x101010ef10101010,
	0x202020df20202020,
	0x404040bf40404040,
	0x8080807f80808080,
	0x0101fe0101010101,
	0x0202fd0202020202,
	0x0404fb0404040404,
	0x0808f70808080808,
	0x1010ef1010101010,
	0x2020df2020202020,
	0x4040bf4040404040,
	0x80807f8080808080,
	0x01fe010101010101,
	0x02fd020202020202,
	0x04fb040404040404,
	0x08f7080808080808,
	0x10ef101010101010,
	0x20df202020202020,
	0x40bf404040404040,
	0x807f808080808080,
	0xfe01010101010101,
	0xfd02020202020202,
	0xfb04040404040404,
	0xf708080808080808,
	0xef10101010101010,
	0xdf20202020202020,
	0xbf40404040404040,
	0x7f80808080808080
};

#define GET_KNIGHT_OCC_MASK(square)		(knight_occupancy_masks[(square)])
#define GET_BISHOP_OCC_MASK(square)		(bishop_occupancy_masks[(square)])
#define GET_KING_OCC_MASK(square)		(king_occupancy_masks[(square)])
#define GET_QUEEN_OCC_MASK(square)		(queen_occupancy_masks[(square)])
#define GET_ROOK_OCC_MASK(square)		(rook_occupancy_masks[(square)])
#define GET_WHITE_PAWN_OCC_MASK(square)		(white_pawn_occupancy_masks[(square)])
#define GET_BLACK_PAWN_OCC_MASK(square)		(black_pawn_occupancy_masks[(square)])

#endif
