
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

#include "../graph/sheets/YORP.h"
#include "../graph/sheets/GARG.h"
#include "../graph/sheets/VORTICON.h"
#include "../graph/sheets/CAN.h"
#include "../graph/sheets/TANK.h"
#include "../graph/sheets/ICEUBE.h"
#include "../graph/sheets/ICEPOW.h"
#include "../graph/sheets/LAZER.h"
#include "../graph/sheets/ZAPZOT.h"
#include "../graph/sheets/CHAIN.h"

#include "../graph/sheets/MAPKEEN.h"
#include "../graph/sheets/KEEN.h"

vk_sprite VK_GameSprites[VK_MAX_SPRITES];
uint16_t VK_NumOfSprites = 0;
uint32_t VK_GBA_SGC = 0;

// Use : sprsizes[h][w]

GBA_SpriteSizes SPR_SIZE_TABLE[6][6] = {
	{0,            0,             0, 0,                0},
	{0, GBA_SPR_8_8 , GBA_SPR_16_8 , 0, GBA_SPR_32_8 , 0},
	{0, GBA_SPR_8_16, GBA_SPR_16_16, 0, GBA_SPR_32_16, 0},
	{0,            0,             0, 0,                0},
	{0, GBA_SPR_8_32, GBA_SPR_16_32, 0, GBA_SPR_32_32, GBA_SPR_64_32},
	{0,            0,             0, 0, GBA_SPR_32_64, GBA_SPR_64_64},    
};

vk_sprite *VK_CreateSprite(uint16_t sprite_id){
	vk_sprite * ptr = &VK_GameSprites[VK_NumOfSprites];
	int i;

	
	for(i = 0; i < VK_NumOfSprites; i++){
		if(VK_GameSprites[i].active==0){
			ptr = &VK_GameSprites[i];
			VK_NumOfSprites -= 1;
			break;
		}
	}

	VK_NumOfSprites += 1;
	if(VK_NumOfSprites >= VK_MAX_SPRITES){
		VK_NumOfSprites = VK_MAX_SPRITES-1;
	}

	// Use the standered Keen Vorticons sprite id layout
	switch(sprite_id){
		case 1:
			// Yorp
			ptr->s.spr_width = YORP_spr_width;
			ptr->s.spr_height = YORP_spr_height;
			ptr->s.sheet_width = YORP_width;
			ptr->s.spr_data = &YORP_data;
		break;
		case 2:
			// Garg
			ptr->s.spr_width = GARG_spr_width;
			ptr->s.spr_height = GARG_spr_height;
			ptr->s.sheet_width = GARG_width;
			ptr->s.spr_data = &GARG_data;
		break;
		case 3:
			// Vorticon
			ptr->s.spr_width = VORTICON_spr_width;
			ptr->s.spr_height = VORTICON_spr_height;
			ptr->s.sheet_width = VORTICON_width;
			ptr->s.spr_data = &VORTICON_data;
		break;
		case 4:
			// Butler robot
			ptr->s.spr_width = CAN_spr_width;
			ptr->s.spr_height = CAN_spr_height;
			ptr->s.sheet_width = CAN_width;
			ptr->s.spr_data = &CAN_data;
		break;
		case 5:
			// Tank robot
			ptr->s.spr_width = TANK_spr_width;
			ptr->s.spr_height = TANK_spr_height;
			ptr->s.sheet_width = TANK_width;
			ptr->s.spr_data = &TANK_data;
		break;
		case 6:
			// Ice cube
			ptr->s.spr_width = ICEUBE_spr_width;
			ptr->s.spr_height = ICEUBE_spr_height;
			ptr->s.sheet_width = ICEUBE_width;
			ptr->s.spr_data = &ICEUBE_data;
		break;
		case 7:
			// Ice shard
			ptr->s.spr_width = ICEPOW_spr_width;
			ptr->s.spr_height = ICEPOW_spr_height;
			ptr->s.sheet_width = ICEPOW_width;
			ptr->s.spr_data = &ICEPOW_data;
		break;
		case 8:
			// Lazer
			ptr->s.spr_width = LAZER_spr_width;
			ptr->s.spr_height = LAZER_spr_height;
			ptr->s.sheet_width = LAZER_width;
			ptr->s.spr_data = &LAZER_data;
		break;
		case 9:
			// Zap Zot
			ptr->s.spr_width = ZAPZOT_spr_width;
			ptr->s.spr_height = ZAPZOT_spr_height;
			ptr->s.sheet_width = ZAPZOT_width;
			ptr->s.spr_data = &ZAPZOT_data;
		break;
		case 10:
			// Chain
			ptr->s.spr_width = CHAIN_spr_width;
			ptr->s.spr_height = CHAIN_spr_height;
			ptr->s.sheet_width = CHAIN_width;
			ptr->s.spr_data = &CHAIN_data;
		break;
		case 254:
			// World Map Commander Keen
			ptr->s.spr_width = MAPKEEN_spr_width;
			ptr->s.spr_height = MAPKEEN_spr_height;
			ptr->s.sheet_width = MAPKEEN_width;
			ptr->s.spr_data = &MAPKEEN_data;
		break;
		case 255:
			// Commander Keen
			ptr->s.spr_width = KEEN_spr_width;
			ptr->s.spr_height = KEEN_spr_height;
			ptr->s.sheet_width = KEEN_width;
			ptr->s.spr_data = &KEEN_data;
		break;
		default:
			// The is no sprite for that!
			return NULL;
		break;
	};

	unsigned short cw = ptr->s.spr_width >> 3;
	unsigned short ch = ptr->s.spr_height >> 3;

	ptr->s.spr_cw[0] = (cw&0xFFFE)<<3;
	ptr->s.spr_ch[0] = (ch&0xFFFE)<<3;
	ptr->s.spr_cw[1] = ((cw&0x1)<<3);
	ptr->s.spr_ch[1] = ptr->s.spr_ch[0];
	ptr->s.spr_cw[2] = ptr->s.spr_cw[0];
	ptr->s.spr_ch[2] = ((ch&0x1)<<3);
	ptr->s.spr_cw[3] = 0;
	ptr->s.spr_ch[3] = 0;

	if(ptr->s.spr_cw[1] && ptr->s.spr_ch[2]){
		// Need extra sprite
		ptr->s.spr_cw[3] = ptr->s.spr_cw[1];
		ptr->s.spr_ch[3] = ptr->s.spr_cw[2];
	}

	// Set the offsets
	ptr->s.spr_off[0][0] = 0;
	ptr->s.spr_off[0][1] = 0;

	ptr->s.spr_off[1][0] = ptr->s.spr_cw[0];
	ptr->s.spr_off[1][1] = 0;

	ptr->s.spr_off[2][0] = 0;
	ptr->s.spr_off[2][1] = ptr->s.spr_ch[0];

	ptr->s.spr_off[3][0] = ptr->s.spr_cw[0];
	ptr->s.spr_off[3][1] = ptr->s.spr_ch[0];

	// Create gba sprites for keen sprite
	ptr->s.spr_indx[0] = 0xFF;
	ptr->s.spr_indx[1] = 0xFF;
	ptr->s.spr_indx[2] = 0xFF;
	ptr->s.spr_indx[3] = 0xFF;
	
	unsigned short setoff = 0;

	ptr->s.num_sprs = 0;

	for(i = 0; i < 4; i++){
		ptr->s.spr_off[i][2] = VK_GBA_SGC+setoff;
		
		if(ptr->s.spr_cw[i]>0&&ptr->s.spr_ch[i]>0){
			ptr->s.spr_indx[i] = GBA_CreateSprite(0xF0,0xF0,SPR_SIZE_TABLE[ptr->s.spr_ch[i]>>3][ptr->s.spr_cw[i]>>3],ptr->s.spr_off[i][2]>>5,GBA_SPRITE_ZFRONT,-1);
			setoff += (ptr->s.spr_cw[i]*ptr->s.spr_ch[i]);
			ptr->s.num_sprs += 1;
		}
	}
	VK_GBA_SGC += (ptr->s.spr_width*ptr->s.spr_height);
	
	ptr->s.spr_gfx_ani = 0;

	ptr->active = 1;
	
	return ptr;
};

void VK_ClearSprites(){
	uint16_t i;
	GBA_ResetSprites();
	for(i = 0; i < VK_MAX_SPRITES; i++){
		VK_GameSprites[i].active = 0;
	}
	VK_NumOfSprites = 0;
	VK_GBA_SGC = 0;
	
	GBA_UPDATE_SPRITES();
};

void VK_RemoveSprite(vk_sprite *ptr){
	int e;
	if(ptr!=NULL){
		ptr->active = 0;
		// Reset the position
		for(e = 0; e < 4; e++){
			if(ptr->s.spr_indx[e]>=0&&ptr->s.spr_indx[e]<128){
				GBA_SET_SPRITE_CLEAR(ptr->s.spr_indx[e]);
			}
		}		
		
		GBA_UPDATE_SPRITES();
	}
};

void VK_SetSpriteGraphics(vk_sprite * ptr){
	if(ptr!=NULL){
		// Render the sprite
		uint16_t e,i, bmpof;
		for(i = 0; i < 4; i++){
			if(ptr->s.spr_cw[i]==0){
				continue;
			}
			// Copy the sprite image
			bmpof = 0;
			unsigned short gfxoff = (ptr->s.spr_gfx_ani*ptr->s.spr_width)<<3;
			gfxoff += ptr->s.spr_off[i][0]<<3;
			gfxoff += (ptr->s.spr_off[i][1]*(ptr->s.sheet_width));
			for(e = 0; e < ptr->s.spr_ch[i]>>3; e++){
				GBA_DMA_Copy16(GBA_SPRGFX_START+ptr->s.spr_off[i][2]+bmpof, ptr->s.spr_data+gfxoff, ptr->s.spr_cw[i]<<2);
				gfxoff += (ptr->s.sheet_width)<<3;
				bmpof += ptr->s.spr_cw[i]<<3;
			}
		}
	}
};

void VK_RenderSprites(){
	uint16_t i,e;

	for(i = 0; i < VK_NumOfSprites; i++){
		vk_sprite *ptr = &VK_GameSprites[i];
		for(e = 0; e < 4; e++){
			if(ptr->s.spr_indx[e]>=0&&ptr->s.spr_indx[e]<128){
				GBA_SET_SPRITE_POSITION(ptr->s.spr_indx[e],ptr->x+ptr->s.spr_off[e][0],ptr->y+ptr->s.spr_off[e][1]);
			}
		}
	}
	GBA_UPDATE_SPRITES();
};


void VK_RenderSprite(vk_sprite *ptr){
	uint16_t e;
	for(e = 0; e < 4; e++){
		if(ptr->s.spr_indx[e]>=0&&ptr->s.spr_indx[e]<128){
			if(ptr->x >= -0x40 && ptr->y >= -0x40 && ptr->x < 0x100 && ptr->y < 0xB0){
				GBA_SET_SPRITE_POSITION(ptr->s.spr_indx[e],ptr->x+ptr->s.spr_off[e][0],ptr->y+ptr->s.spr_off[e][1]);
			}else{
				// Hide the sprite
				GBA_SET_SPRITE_POSITION(ptr->s.spr_indx[e],0xF0,0xB0);
			}
			GBA_UPDATE_SPRITE(ptr->s.spr_indx[e]);
		}
	}
	
};



