
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
#include "../level/LEVEL90.h"
#include "../graph/tilesets/LEVEL90_TILESET.h"

uint16_t clear_tile_offset = 0;

GBA_IN_EWRAM unsigned short vk_level_data[128*128];

GBA_IN_EWRAM unsigned char vk_tileanimations[256];

unsigned short vk_num_of_tiles = 0;

unsigned short vk_level_width = 0;
unsigned short vk_level_height = 0;

unsigned short vk_level_offsetx = 0;
unsigned short vk_level_offsety = 0;

unsigned short vk_map_offsetx = 0;
unsigned short vk_map_offsety = 0;

unsigned char vk_level_needs_update = 0;
unsigned char vk_level_lock_cam = 0;

unsigned short vk_level_update_tick = 0;


// Load the level and tileset
void VK_LoadLevel(uint16_t levelid){	
	int i;
	// Setup the tileset
	unsigned char *TILESET_data = NULL;
	unsigned short TILESET_size = 0;
	
	TILESET_size = level90_tileset_size;
	TILESET_data = &level90_tileset_data;
	vk_num_of_tiles = level90_num_tiles;

	clear_tile_offset = vk_num_of_tiles;

	if(TILESET_data==NULL)
		return ; // Uh oh!

	// Clear the tileset with black
	for(i = 0; i < 64*128; i++){
		VK_GBA_BG_Tiles[i] = 0x1;
	}
	for(i = 0; i < 256; i++){
		vk_tileanimations[i] = 0x0001;
	}
	// TODO:
	// Load actual tile info
	vk_tileanimations[9] = 0x04;
	vk_tileanimations[10] = 0x14;
	vk_tileanimations[11] = 0x24;
	vk_tileanimations[12] = 0x34;
	
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

	// Copy the level
	GBA_DMA_Copy32(vk_level_data,level90_data,level90_size>>1);
	vk_level_width = level90_width;
	vk_level_height = level90_height;
	
	vk_level_needs_update = 1;
	vk_level_update_tick = 0;
};

// Render the level
void VK_RenderLevel(){
	int i,e;
	
	if(vk_level_needs_update){
		
		for(e = 0; e < 16; e++){
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
	
	if(vk_level_update_tick > 0x08){
		vk_level_update_tick = 0;
		
		// Use a pre defined pointer cause it's faster?
		uint16_t *tile = &vk_level_data;
		for(e = 0; e < vk_level_height; e++){
			for(i = 0; i < vk_level_width; i++){
				if((vk_tileanimations[*tile]&0xF) != 0x1){
					// Animate the tile
					if(((vk_tileanimations[*tile]>>4)&0xF) == 0x3){
						*tile -= 4;
					}
					*tile += 1;
				}
				tile ++; // Move the pointer
			}
		}
		vk_level_needs_update = 1;
	}
	
};

// Position the map offset
void VK_PositionCamera(uint16_t offsetx,uint16_t offsety){
	vk_map_offsetx = offsetx;
	vk_map_offsety = offsety;
};


// Position the level
void VK_PositionLevel(uint16_t offsetx,uint16_t offsety){
	vk_level_offsetx = vk_level_width-21; // subtract 2.5 from edge to center
	vk_level_offsety = 3;
};

// Lock the camera
void VK_LockCamera(){
	vk_level_lock_cam = 1;
};

// UnLock the camera
void VK_UnLockCamera(){
	vk_level_lock_cam = 0;
};



