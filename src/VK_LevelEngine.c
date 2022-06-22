
/*
 * 
 * VortaKeen - A port of Commander Keen Invasion of the Vorticons for the Gameboy Advance
 * 
 * Started 06.17.22
 * 
 * Copyright (c) John314
 * 
 * Commander Keen is a trademark of ID software
 * 
*/

#include "VK_Headers.h"

#include "../level/LEVEL01.h"
#include "../level/LEVEL01_INFO.h"
#include "../graph/tilesets/LEVEL01_TILESET.h"
#include "../level/LEVEL02.h"
#include "../level/LEVEL02_INFO.h"
#include "../graph/tilesets/LEVEL02_TILESET.h"
#include "../level/LEVEL03.h"
#include "../level/LEVEL03_INFO.h"
#include "../graph/tilesets/LEVEL03_TILESET.h"
#include "../level/LEVEL04.h"
#include "../level/LEVEL04_INFO.h"
#include "../graph/tilesets/LEVEL04_TILESET.h"
#include "../level/LEVEL05.h"
#include "../level/LEVEL05_INFO.h"
#include "../graph/tilesets/LEVEL05_TILESET.h"
#include "../level/LEVEL06.h"
#include "../level/LEVEL06_INFO.h"
#include "../graph/tilesets/LEVEL06_TILESET.h"
#include "../level/LEVEL07.h"
#include "../level/LEVEL07_INFO.h"
#include "../graph/tilesets/LEVEL07_TILESET.h"
#include "../level/LEVEL08.h"
#include "../level/LEVEL08_INFO.h"
#include "../graph/tilesets/LEVEL08_TILESET.h"
#include "../level/LEVEL09.h"
#include "../level/LEVEL09_INFO.h"
#include "../graph/tilesets/LEVEL09_TILESET.h"
#include "../level/LEVEL10.h"
#include "../level/LEVEL10_INFO.h"
#include "../graph/tilesets/LEVEL10_TILESET.h"
#include "../level/LEVEL11.h"
#include "../level/LEVEL11_INFO.h"
#include "../graph/tilesets/LEVEL11_TILESET.h"
#include "../level/LEVEL12.h"
#include "../level/LEVEL12_INFO.h"
#include "../graph/tilesets/LEVEL12_TILESET.h"
#include "../level/LEVEL13.h"
#include "../level/LEVEL13_INFO.h"
#include "../graph/tilesets/LEVEL13_TILESET.h"
#include "../level/LEVEL14.h"
#include "../level/LEVEL14_INFO.h"
#include "../graph/tilesets/LEVEL14_TILESET.h"
#include "../level/LEVEL15.h"
#include "../level/LEVEL15_INFO.h"
#include "../graph/tilesets/LEVEL15_TILESET.h"
#include "../level/LEVEL16.h"
#include "../level/LEVEL16_INFO.h"
#include "../graph/tilesets/LEVEL16_TILESET.h"
// Add more levels here?

#include "../level/LEVEL80.h"
#include "../level/LEVEL80_INFO.h"
#include "../graph/tilesets/LEVEL80_TILESET.h"
#include "../level/LEVEL81.h"
#include "../level/LEVEL81_INFO.h"
#include "../graph/tilesets/LEVEL81_TILESET.h"
#include "../level/LEVEL90.h"
#include "../level/LEVEL90_INFO.h"
#include "../graph/tilesets/LEVEL90_TILESET.h"


uint16_t clear_tile_offset = 0;

GBA_IN_EWRAM unsigned short vk_level_data[128*128];

GBA_IN_EWRAM unsigned char vk_tileanimations[256];

GBA_IN_EWRAM unsigned short ck_update_positions[0x1000][2];
GBA_IN_EWRAM unsigned short ck_update_locations[1024][2];
unsigned short ck_number_of_updates = 0;
unsigned short ck_number_of_ulocations = 0;

unsigned short *vk_level_tileinfo = NULL;

unsigned short vk_num_of_tiles = 0;

unsigned short vk_level_width = 0;
unsigned short vk_level_height = 0;

signed short vk_level_offsetx = 0;
signed short vk_level_offsety = 0;

signed short vk_map_offsetx = 0;
signed short vk_map_offsety = 0;

unsigned char vk_level_needs_update = 0;
unsigned char vk_level_lock_cam = 0;

unsigned short vk_level_update_tick = 0;


// Load the level and tileset
void VK_LoadLevel(uint16_t levelid){	
	int i,e;
	// Setup the tileset
	unsigned char *TILESET_data = NULL;
	unsigned short TILESET_size = 0;

	switch(levelid){
		case 1:
			 TILESET_size = level01_tileset_size;
			 TILESET_data = &level01_tileset_data;
			 vk_num_of_tiles = level01_num_tiles;
			 vk_level_tileinfo = &level01_tinfo;
			 GBA_DMA_Copy32(vk_level_data,level01_data,level01_size>>1);
			 vk_level_width = level01_width;
			 vk_level_height = level01_height;
		break;
		case 2:
			 TILESET_size = level02_tileset_size;
			 TILESET_data = &level02_tileset_data;
			 vk_num_of_tiles = level02_num_tiles;
			 vk_level_tileinfo = &level02_tinfo;
			 GBA_DMA_Copy32(vk_level_data,level02_data,level02_size>>1);
			 vk_level_width = level02_width;
			 vk_level_height = level02_height;
		break;
		case 3:
			 TILESET_size = level03_tileset_size;
			 TILESET_data = &level03_tileset_data;
			 vk_num_of_tiles = level03_num_tiles;
			 vk_level_tileinfo = &level03_tinfo;
			 GBA_DMA_Copy32(vk_level_data,level03_data,level03_size>>1);
			 vk_level_width = level03_width;
			 vk_level_height = level03_height;
		break;
		case 4:
			 TILESET_size = level04_tileset_size;
			 TILESET_data = &level04_tileset_data;
			 vk_num_of_tiles = level04_num_tiles;
			 vk_level_tileinfo = &level04_tinfo;
			 GBA_DMA_Copy32(vk_level_data,level04_data,level04_size>>1);
			 vk_level_width = level04_width;
			 vk_level_height = level04_height;
		break;
		case 5:
			 TILESET_size = level05_tileset_size;
			 TILESET_data = &level05_tileset_data;
			 vk_num_of_tiles = level05_num_tiles;
			 vk_level_tileinfo = &level05_tinfo;
			 GBA_DMA_Copy32(vk_level_data,level05_data,level05_size>>1);
			 vk_level_width = level05_width;
			 vk_level_height = level05_height;
		break;
		case 6:
			 TILESET_size = level06_tileset_size;
			 TILESET_data = &level06_tileset_data;
			 vk_num_of_tiles = level06_num_tiles;
			 vk_level_tileinfo = &level06_tinfo;
			 GBA_DMA_Copy32(vk_level_data,level06_data,level06_size>>1);
			 vk_level_width = level06_width;
			 vk_level_height = level06_height;
		break;
		case 7:
			 TILESET_size = level07_tileset_size;
			 TILESET_data = &level07_tileset_data;
			 vk_num_of_tiles = level07_num_tiles;
			 vk_level_tileinfo = &level07_tinfo;
			 GBA_DMA_Copy32(vk_level_data,level07_data,level07_size>>1);
			 vk_level_width = level07_width;
			 vk_level_height = level07_height;
		break;
		case 8:
			 TILESET_size = level08_tileset_size;
			 TILESET_data = &level08_tileset_data;
			 vk_num_of_tiles = level08_num_tiles;
			 vk_level_tileinfo = &level08_tinfo;
			 GBA_DMA_Copy32(vk_level_data,level08_data,level08_size>>1);
			 vk_level_width = level08_width;
			 vk_level_height = level08_height;
		break;
		case 9:
			 TILESET_size = level09_tileset_size;
			 TILESET_data = &level09_tileset_data;
			 vk_num_of_tiles = level09_num_tiles;
			 vk_level_tileinfo = &level09_tinfo;
			 GBA_DMA_Copy32(vk_level_data,level09_data,level09_size>>1);
			 vk_level_width = level09_width;
			 vk_level_height = level09_height;
		break;
		case 10:
			 TILESET_size = level10_tileset_size;
			 TILESET_data = &level10_tileset_data;
			 vk_num_of_tiles = level10_num_tiles;
			 vk_level_tileinfo = &level10_tinfo;
			 GBA_DMA_Copy32(vk_level_data,level10_data,level10_size>>1);
			 vk_level_width = level10_width;
			 vk_level_height = level10_height;
		break;
		case 11:
			 TILESET_size = level11_tileset_size;
			 TILESET_data = &level11_tileset_data;
			 vk_num_of_tiles = level11_num_tiles;
			 vk_level_tileinfo = &level11_tinfo;
			 GBA_DMA_Copy32(vk_level_data,level11_data,level11_size>>1);
			 vk_level_width = level11_width;
			 vk_level_height = level11_height;
		break;
		case 12:
			 TILESET_size = level12_tileset_size;
			 TILESET_data = &level12_tileset_data;
			 vk_num_of_tiles = level12_num_tiles;
			 vk_level_tileinfo = &level12_tinfo;
			 GBA_DMA_Copy32(vk_level_data,level12_data,level12_size>>1);
			 vk_level_width = level12_width;
			 vk_level_height = level12_height;
		break;
		case 13:
			 TILESET_size = level13_tileset_size;
			 TILESET_data = &level13_tileset_data;
			 vk_num_of_tiles = level13_num_tiles;
			 vk_level_tileinfo = &level13_tinfo;
			 GBA_DMA_Copy32(vk_level_data,level13_data,level13_size>>1);
			 vk_level_width = level13_width;
			 vk_level_height = level13_height;
		break;
		case 14:
			 TILESET_size = level14_tileset_size;
			 TILESET_data = &level14_tileset_data;
			 vk_num_of_tiles = level14_num_tiles;
			 vk_level_tileinfo = &level14_tinfo;
			 GBA_DMA_Copy32(vk_level_data,level14_data,level14_size>>1);
			 vk_level_width = level14_width;
			 vk_level_height = level14_height;
		break;
		case 15:
			 TILESET_size = level15_tileset_size;
			 TILESET_data = &level15_tileset_data;
			 vk_num_of_tiles = level15_num_tiles;
			 vk_level_tileinfo = &level15_tinfo;
			 GBA_DMA_Copy32(vk_level_data,level15_data,level15_size>>1);
			 vk_level_width = level15_width;
			 vk_level_height = level15_height;
		break;
		case 16:
			 TILESET_size = level16_tileset_size;
			 TILESET_data = &level16_tileset_data;
			 vk_num_of_tiles = level16_num_tiles;
			 vk_level_tileinfo = &level16_tinfo;
			 GBA_DMA_Copy32(vk_level_data,level16_data,level16_size>>1);
			 vk_level_width = level16_width;
			 vk_level_height = level16_height;
		break;

		case 80:
			 TILESET_size = level80_tileset_size;
			 TILESET_data = &level80_tileset_data;
			 vk_num_of_tiles = level80_num_tiles;
			 vk_level_tileinfo = &level80_tinfo;
			 GBA_DMA_Copy32(vk_level_data,level80_data,level80_size>>1);
			 vk_level_width = level80_width;
			 vk_level_height = level80_height;
		break;
		
		case 81:
			 TILESET_size = level81_tileset_size;
			 TILESET_data = &level81_tileset_data;
			 vk_num_of_tiles = level81_num_tiles;
			 vk_level_tileinfo = &level81_tinfo;
			 GBA_DMA_Copy32(vk_level_data,level81_data,level81_size>>1);
			 vk_level_width = level81_width;
			 vk_level_height = level81_height;
		break;		

		case 90:
			 TILESET_size = level90_tileset_size;
			 TILESET_data = &level90_tileset_data;
			 vk_num_of_tiles = level90_num_tiles;
			 vk_level_tileinfo = &level90_tinfo;
			 GBA_DMA_Copy32(vk_level_data,level90_data,level90_size>>1);
			 vk_level_width = level90_width;
			 vk_level_height = level90_height;
		break;
	}

	clear_tile_offset = vk_num_of_tiles;

	if(TILESET_data==NULL || vk_level_tileinfo == NULL)
		return ; // Uh oh!

	// Clear the tileset with black
	for(i = 0; i < 64*128; i++){
		VK_GBA_BG_Tiles[i] = 0x1;
	}
	
	// Load the tileinfo
	for(i = 0; i < (vk_num_of_tiles*6); i++){
		vk_tileanimations[i] = vk_level_tileinfo[(i*6)];
	}
	
	// Copy the tileset
	GBA_DMA_Copy32(VK_GBA_BG_Tiles,TILESET_data,TILESET_size>>2);
	
	// Clear tile at the end of the tiles
	uint16_t cleartof = ((clear_tile_offset%8)<<7) + ((clear_tile_offset>>3)<<11);
	for(i = 0; i < 64; i++){
		VK_GBA_BG_Tiles[i+cleartof] = 0;
		VK_GBA_BG_Tiles[i+cleartof+64] = 1;
		VK_GBA_BG_Tiles[i+cleartof+1024] = 2;
		VK_GBA_BG_Tiles[i+cleartof+64+1024] = 3;

	}
	// Clear the maps
	for(i = 0; i < 32*32; i++){
		VK_GBA_BG_MAPA[i] = 0;
		VK_GBA_BG_MAPB[i] = ((clear_tile_offset%8)<<1) + ((clear_tile_offset>>3)<<5);
	}
	
	// Generate animation list
	uint16_t *tile = &vk_level_data; // Use a pre defined pointer cause it's faster?

	ck_number_of_updates = 0;
	for(e = 0; e < vk_level_height; e++){
		for(i = 0; i < vk_level_width; i++){
			if((vk_tileanimations[*tile]&0xF) != 0x1){
				ck_update_positions[ck_number_of_updates][0] = i;
				ck_update_positions[ck_number_of_updates][1] = e;
				ck_number_of_updates++;
				if(ck_number_of_updates>0x1000){
					ck_number_of_updates = 0x1000;
				}
			}
			tile ++; // Move the pointer
		}
	}

	
	vk_level_needs_update = 1;
	vk_level_update_tick = 0;
};

// Render the level
void VK_RenderLevel(){
	int i,e;
	
	if(vk_level_needs_update==2){
		vk_level_needs_update = 0;
		
		for(i = 0; i < ck_number_of_ulocations; i++){
			VK_GBA_BG_MAPA[ck_update_locations[i][0]] = ck_update_locations[i][1];
			VK_GBA_BG_MAPA[ck_update_locations[i][0]+1] = ck_update_locations[i][1]+1;
			VK_GBA_BG_MAPA[ck_update_locations[i][0]+32] = ck_update_locations[i][1]+16;
			VK_GBA_BG_MAPA[ck_update_locations[i][0]+33] = ck_update_locations[i][1]+17;
		}
	}
		
	if(vk_level_needs_update==1){
		vk_level_needs_update = 0;
		GBA_WAIT_VBLANK
		for(e = 0; e < 11; e++){
			for(i = 0; i < 16; i++){
				uint16_t lvlt = vk_level_data[((e+vk_level_offsety)*vk_level_width)+i+vk_level_offsetx];
				uint16_t tile = ((lvlt%8)<<1) + ((lvlt>>3)<<5);

				VK_GBA_BG_MAPA[((e<<1)<<5)+(i<<1)] = tile;
				VK_GBA_BG_MAPA[((e<<1)<<5)+(i<<1)+1] = tile+1;
				VK_GBA_BG_MAPA[(((e<<1)+1)<<5)+(i<<1)] = tile+16;
				VK_GBA_BG_MAPA[(((e<<1)+1)<<5)+(i<<1)+1] = tile+17;
			}
		}
	}
	
	// Offset the screen
	if(vk_level_lock_cam==0){
		*(volatile uint16_t*)GBA_REG_BG0HOFS = vk_map_offsetx;
		*(volatile uint16_t*)GBA_REG_BG0VOFS = vk_map_offsety;

		*(volatile uint16_t*)GBA_REG_BG1HOFS = vk_map_offsetx;
		*(volatile uint16_t*)GBA_REG_BG1VOFS = vk_map_offsety;
	}
	
};


// Update the map
void VK_UpdateLevel(){
	int i,e;
	
	vk_level_update_tick ++;
	
	if(vk_level_update_tick > 0x8){
		vk_level_update_tick = 0;
			
		ck_number_of_ulocations = 0;
		for(i = 0; i < ck_number_of_updates; i++){
			// Find the tile
			uint16_t *tile = &vk_level_data;
			tile += (ck_update_positions[i][1]*vk_level_width)+ck_update_positions[i][0];

			if((vk_tileanimations[*tile]&0xF) == 0x1){
				continue;
			}

			if((vk_tileanimations[*tile]&0xF) == 0x4){
				// Animate the tile
				if(((vk_tileanimations[*tile]>>4)&0xF) == 0x3){
					*tile -= 3;
				}else{
					*tile += 1;
				}
			}
			if((vk_tileanimations[*tile]&0xF) == 0x2){
				if(((vk_tileanimations[*tile]>>4)&0xF) == 0x1){
					*tile -= 1;
				}else{
					*tile += 1;
				}
			}
			
			uint16_t tx = (ck_update_positions[i][0]-vk_level_offsetx);
			uint16_t ty = (ck_update_positions[i][1]-vk_level_offsety);

			if( tx >= 0 && ty >= 0){
				if( tx < 16 && ty < 16){
					ck_update_locations[ck_number_of_ulocations][0] = (ty<<6)+(tx<<1);
					ck_update_locations[ck_number_of_ulocations][1] = (((*tile)%8)<<1) + (((*tile)>>3)<<5);
					ck_number_of_ulocations++;
					if(ck_number_of_ulocations>=1024){
						ck_number_of_ulocations = 1023;
					}
				}
			}
		}
		/*
		// Use a pre defined pointer cause it's faster?
		uint16_t *tile = &vk_level_data;
		uint16_t updateloc = 0;

		for(e = 0; e < vk_level_height; e++){
			for(i = 0; i < vk_level_width; i++){
				ck_update_locations[updateloc] = 0xFFFF;
				if((vk_tileanimations[*tile]&0xF) != 0x1){
					// Animate the tile
					if(((vk_tileanimations[*tile]>>4)&0xF) == 0x3){
						*tile -= 4;
					}
					*tile += 1;
					
					uint16_t tx = (i-vk_level_offsetx);
					uint16_t ty = (e-vk_level_offsety);

					if( tx >= 0 && ty >= 0){
						if( tx < 16 && ty < 16){
							ck_update_locations[updateloc] = (ty<<6)+(tx<<1);
							ck_update_locations2[updateloc] = (e*vk_level_width) + i;
							updateloc++;
						}
					}
				}
				tile ++; // Move the pointer
			}
		}*/
		
		// Only tell the engine to update tiles only if no update is present
		if(vk_level_needs_update==0){
			vk_level_needs_update = 2;
		}
	}
	
};

// Position the map offset
void VK_PositionCamera(uint16_t offsetx,uint16_t offsety){
	vk_map_offsetx = offsetx;
	vk_map_offsety = offsety;
	
	// Clamp the camera
	/*if(vk_level_offsetx==2){
		if(vk_map_offsetx>0){
			vk_map_offsetx = 0; // We don't scroll
		}
	}
	if(vk_level_offsetx==vk_level_width-2){
		if(vk_map_offsetx>0){
			vk_map_offsetx = 0; // We don't scroll
		}
	}*/
};


// Position the level
void VK_PositionLevel(uint16_t offsetx,uint16_t offsety){
	if(offsetx != vk_level_offsetx || offsety != vk_level_offsety){
		vk_level_needs_update = 1;
	}
	vk_level_offsetx = offsetx;
	vk_level_offsety = offsety;
	/*
	// Clamp the offset to the map size
	if(vk_level_offsetx<2){
		vk_level_offsetx = 2;
	}
	if(vk_level_offsetx>vk_level_width-2){
		vk_level_offsetx = vk_level_width-2;
	}*/
};

// Lock the camera
void VK_LockCamera(){
	vk_level_lock_cam = 1;
};

// UnLock the camera
void VK_UnLockCamera(){
	vk_level_lock_cam = 0;
};



