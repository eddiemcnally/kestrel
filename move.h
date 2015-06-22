/*
 * move.h
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
void generate_king_occupancy_masks(board_t *occ_mask_array);
void generate_knight_occupancy_masks(board_t *occ_mask_array);
void generate_rook_occupancy_masks(board_t *occ_mask_array);
void print_knight_occupancy_masks();
void print_king_occupancy_masks();


// elem [0] is a1, [63] is h8
static const board_t knight_occupancy_masks[NUM_SQUARES] = {
			0x0000000000028440,			
			0x0000000000050880,			
			0x00000000000a1100,
			0x0000000000142200,
			0x0000000000284400,
			0x0000000000508800,
			0x0000000000a11001,
			0x0000000001422002,
			0x0000000002844004,		
			0x0000000005088008,			
			0x000000000a110011,
			0x0000000014220022,
			0x0000000028440044,
			0x0000000050880088,
			0x00000000a1100110,
			0x0000000142200221,
			0x0000000284400442,
			0x0000000508800885,
			0x0000000a1100110a,
			0x0000001422002214,
			0x0000002844004428,
			0x0000005088008850,
			0x000000a1100110a0,
			0x0000014220022140,
			0x0000028440044280,
			0x0000050880088500,
			0x00000a1100110a00,
			0x0000142200221400,
			0x0000284400442800,
			0x0000508800885000,
			0x0000a1100110a000,
			0x0001422002214000,
			0x0002844004428000,
			0x0005088008850000,
			0x000a1100110a0000,
			0x0014220022140000,
			0x0028440044280000,
			0x0050880088500000,
			0x00a1100110a00000,
			0x0142200221400000,
			0x0284400442800000,
			0x0508800885000000,
			0x0a1100110a000000,
			0x1422002214000000,
			0x2844004428000000,
			0x5088008850000000,
			0xa1100110a0000000,
			0x4220022140000000,
			0x8440044280000000,
			0x0880088500000000,
			0x1100110a00000000,
			0x2200221400000000,
			0x4400442800000000,
			0x8800885000000000,
			0x100110a000000000,
			0x2002214000000000,
			0x4004428000000000,
			0x8008850000000000,
			0x00110a0000000000,
			0x0022140000000000,
			0x0044280000000000,
			0x0088500000000000,
			0x0110a00000000000,
			0x0221400000000000
};


// elem [0] is a1, [63] is h8
static const board_t king_occupancy_masks[NUM_SQUARES] = {
			0x0000000000000382,
			0x0000000000000705,
			0x0000000000000e0a,
			0x0000000000001c14,
			0x0000000000003828,
			0x0000000000007050,
			0x000000000000e0a0,
			0x000000000001c141,
			0x0000000000038283,
			0x0000000000070507,
			0x00000000000e0a0e,
			0x00000000001c141c,
			0x0000000000382838,
			0x0000000000705070,
			0x0000000000e0a0e0,
			0x0000000001c141c0,
			0x0000000003828380,
			0x0000000007050700,
			0x000000000e0a0e00,
			0x000000001c141c00,
			0x0000000038283800,
			0x0000000070507000,
			0x00000000e0a0e000,
			0x00000001c141c000,
			0x0000000382838000,
			0x0000000705070000,
			0x0000000e0a0e0000,
			0x0000001c141c0000,
			0x0000003828380000,
			0x0000007050700000,
			0x000000e0a0e00000,
			0x000001c141c00000,
			0x0000038283800000,
			0x0000070507000000,
			0x00000e0a0e000000,
			0x00001c141c000000,
			0x0000382838000000,
			0x0000705070000000,
			0x0000e0a0e0000000,
			0x0001c141c0000000,
			0x0003828380000000,
			0x0007050700000000,
			0x000e0a0e00000000,
			0x001c141c00000000,
			0x0038283800000000,
			0x0070507000000000,
			0x00e0a0e000000000,
			0x01c141c000000000,
			0x0382838000000000,
			0x0705070000000000,
			0x0e0a0e0000000000,
			0x1c141c0000000000,
			0x3828380000000000,
			0x7050700000000000,
			0xe0a0e00000000000,
			0xc141c00000000000,
			0x8283800000000000,
			0x0507000000000000,
			0x0a0e000000000000,
			0x141c000000000000,
			0x2838000000000000,
			0x5070000000000000,
			0xa0e0000000000000,
			0x41c0000000000000
};





