
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

vk_sprite VK_GameSprites[256];
uint16_t VK_NumOfSprites = 0;

// Use : sprsizes[h][w]

GBA_SpriteSizes SPR_SIZE_TABLE[5][5] = {
	{0, 0, 0, 0, 0},
	{0, GBA_SPR_8_8 , GBA_SPR_16_8 , GBA_SPR_32_8 , 0},
	{0, GBA_SPR_8_16, GBA_SPR_16_16, GBA_SPR_32_16, 0},
	{0, GBA_SPR_8_32, GBA_SPR_16_32, GBA_SPR_32_32, GBA_SPR_64_32},
	{0, 0, 0, GBA_SPR_32_64, GBA_SPR_64_64},    
};

vk_sprite *VK_GetSprite(){
	vk_sprite * ptr = &VK_GameSprites[VK_NumOfSprites];
	VK_NumOfSprites += 1;
	if(VK_NumOfSprites >= 256){
		VK_NumOfSprites = 255;
	}
	return ptr;
};

void VK_ClearSprites(){
	
};

void VK_RenderSprites(){
	int i;
	
	unsigned short spr_width = YORP_spr_width;
	unsigned short spr_height = YORP_spr_height;
	unsigned short sheet_width = YORP_width;
	unsigned char *spr_data = &YORP_data;

	GBA_SpriteIndex_t spr_indx[4];
	unsigned short spr_cw[4];
	unsigned short spr_ch[4];
	unsigned short spr_off[4][3];	
	unsigned short spr_cnt;
	
	// Load the graphics
	for(i = 0; i < 64; i++){
		uint16_t col = ((i&0x1F)>>1);
		GBA_ASM_MemSet16(GBA_SPRGFX_START+(i<<6),(col<<8)+(col+0x101),32);
	}
//	GBA_DMA_Copy16(GBA_SPRGFX_START, spr_data, (spr_height*spr_width)>>2);
//	GBA_DMA_Copy16(GBA_SPRGFX_START+(spr_width<<3), spr_data+(sheet_width<<3), (spr_height*spr_width)>>2);

	GBA_ResetSprites();

	unsigned short cw = spr_width >> 3;
	unsigned short ch = spr_height >> 3;
	
	spr_cw[0] = (cw&0xFFFE)<<3;
	spr_ch[0] = (ch&0xFFFE)<<3;
	spr_cw[1] = ((cw&0x1)<<3);
	spr_ch[1] = spr_ch[0];
	spr_cw[2] = spr_cw[0];
	spr_ch[2] = ((ch&0x1)<<3);
	
	if(spr_cw[1] || spr_ch[2]){
		// Need extra sprite
		spr_cw[3] = spr_cw[1];
		spr_ch[3] = spr_cw[2];
	}
	
	// Set the offsets
	spr_off[0][0] = 0;
	spr_off[0][1] = 0;

	spr_off[1][0] = spr_cw[0];
	spr_off[1][1] = 0;

	spr_off[2][0] = 0;
	spr_off[2][1] = spr_ch[0];

	spr_off[3][0] = spr_cw[0];
	spr_off[3][1] = spr_ch[0];
	
	// Create gba sprites for keen sprite
	
	unsigned short setoff = 0;
	
	spr_cnt = 0;
	for(i = 0; i < 4; i++){
		spr_off[i][2] = setoff;
		
		if(spr_cw[i]>0&&spr_ch[i]>0){
			spr_indx[i] = GBA_CreateSprite(50+spr_off[i][0],50+spr_off[i][1],SPR_SIZE_TABLE[spr_ch[i]>>3][spr_cw[i]>>3],spr_off[i][2]>>5,GBA_SPRITE_ZTOP,-1);
			setoff += (spr_cw[i]*spr_ch[i]);
		}
	}
	
//	GBA_UPDATE_SPRITES();

	uint16_t sprani = 0;
	while(1){
		
		// Render the sprite
		unsigned short gfxoff = (sprani*spr_width)<<3;
		uint16_t e, bmpof;
		for(i = 0; i < 4; i++){
			if(spr_cw[i]>0&&spr_ch[i]>0){
				// Copy the sprite image
				bmpof = 0;
				for(e = 0; e < spr_ch[i]>>3; e++){
					GBA_DMA_Copy16(GBA_SPRGFX_START+spr_off[i][2]+bmpof, spr_data+gfxoff, spr_cw[i]<<2);
					gfxoff += (sheet_width)<<3;
					bmpof += spr_cw[i]<<3;
				}
			GBA_SET_SPRITE_POSITION(spr_indx[i],50+spr_off[i][0],50+spr_off[i][1]);
			GBA_UPDATE_SPRITE(spr_indx[i]);
			}
		}
		
		sprani += 1;
		if(sprani>=sheet_width/spr_width){
			sprani = 0;
		}
		VK_WaitVRB();
		GBA_Delay(500);		
	}
};

