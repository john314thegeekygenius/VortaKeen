
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

volatile uint16_t* VK_GBA_BG_MAPA = (volatile uint16_t*)GBA_SCREEN_BLOCK(30);
volatile uint16_t* VK_GBA_BG_MAPB = (volatile uint16_t*)GBA_SCREEN_BLOCK(31);
volatile uint8_t* VK_GBA_BG_Tiles = (volatile uint16_t*)GBA_CHAR_BLOCK(0);
volatile uint8_t* VK_GBA_BG_Tiles2 = (volatile uint16_t*)GBA_CHAR_BLOCK(2);
volatile uint8_t* VK_GBA_BG_Tiles3 = (volatile uint16_t*)GBA_CHAR_BLOCK(2)+0x1000;
volatile uint8_t* VK_GBA_BG_TilesEnd= (volatile uint16_t*)GBA_CHAR_BLOCK(4)-(32*64);

const uint32_t VK_GBA_TILES2_OFF = 0x200;
const uint32_t VK_GBA_TILES3_OFF = 0x280;
const uint32_t VK_GBA_TILES_END_OFF = 0x3C0;

const uint16_t vk_ega_palette[4][17] = {
	{ 0x67f9,    0x0, 0x5400,  0x2a0, 0x56a0,  0x15, 0x5415,  0x155, 0x56b5, 0x294a, 0x7d4a, 0x2bea, 0x7fea, 0x295f, 0x7d5f, 0x2bff, 0x7fff },
	{ 0x67f9,    0x0,    0x0,    0x0,    0x0,   0x0,    0x0,    0x0,    0x0, 0x294a, 0x7d4a, 0x2bea, 0x7fea, 0x295f, 0x7d5f, 0x2bff, 0x7fff },
	{ 0x67f9,    0x0,    0x0,    0x0,    0x0,   0x0,    0x0,    0x0,    0x0,    0x0, 0x5400,  0x2a0, 0x56a0,   0x15, 0x5415,  0x155, 0x56b5 },
	{ 0x67f9,    0x0,    0x0,    0x0,    0x0,   0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0 },
};

const uint16_t vk_gb_palette[4][17] = {
	{ 0x67f9,  0x4e1,  0x4e1,  0x6b1,  0x6b1, 0x1986, 0x1986, 0x1986,  0x6b1, 0x1986, 0x1986,  0x6f3,  0x6f3,  0x6b1,  0x6b1,  0x6f3,  0x6f3 },
	{ 0x67f9,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0, 0x1986, 0x1986,  0x6f3,  0x6f3,  0x6b1,  0x6b1,  0x6f3,  0x6f3 },
	{ 0x67f9,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,  0x4e1,  0x4e1,  0x6b1,  0x6b1, 0x1986, 0x1986, 0x1986,  0x6b1 },
	{ 0x67f9,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0 },
};

const uint16_t vk_greyscale_palette[4][17] = {
	{ 0x67f9,   0x0, 0x14a5, 0x4a52, 0x4a52, 0x294a, 0x2d6b, 0x35ad, 0x56b5, 0x294a, 0x35ad, 0x6f7b, 0x739c, 0x4a52, 0x4e73, 0x7bde, 0x7fff },
	{ 0x67f9,   0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0, 0x294a, 0x35ad, 0x6f7b, 0x739c, 0x4a52, 0x4e73, 0x7bde, 0x7fff },
	{ 0x67f9,   0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0, 0x14a5, 0x4a52, 0x4a52, 0x294a, 0x2d6b, 0x35ad, 0x56b5, 0x294a },
	{ 0x67f9,   0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0,    0x0 },
};


void VK_SetupVideo(){
	int i;
	
	// Setup the video
	*(volatile uint16_t*)GBA_REG_DISPCNT = GBA_MODE_0 | GBA_ENABLE_BG0 | GBA_ENABLE_BG1 | GBA_SPRITE_ENABLE | GBA_SPRITE_MAP_1D;

	// Copy the palette
	GBA_DMA_Copy16((uint16_t*)GBA_PAL_BG_START,(uint16_t*)vk_ega_palette[0],17);

	GBA_DMA_Copy16((uint16_t*)GBA_PAL_SPR_START,(uint16_t*)vk_ega_palette[0],17);

	
	// Clear all the tiles to blank
	GBA_DMA_MemSet32((uint32_t*)GBA_VRAM,0,128*128);


	// Clear the maps
	for(i = 0; i < 32*32; i++){
		VK_GBA_BG_MAPA[i] = 0;
		VK_GBA_BG_MAPB[i] = 0;
	}
	
	//GBA_BLEND_BGS(GBA_BLD_S_SPR | GBA_BLDM_APLHA |  GBA_BLD_D_BG3 |  GBA_BLD_D_BG2 |  GBA_BLD_D_BG1)
	
	// Setup two backgrounds as 64x64 size
	GBA_FINISH_BG0(GBA_BG_FRONT | 0x00 | (30 << 8) | GBA_BG_SIZE_32x32 | GBA_BG_WRAP);
	GBA_FINISH_BG1(GBA_BG_TOP   | 0x00 | (31 << 8) | GBA_BG_SIZE_32x32 | GBA_BG_WRAP);

	// "Hide" all the sprites
	GBA_HideSprites();

	// Show the sprite
	GBA_UPDATE_SPRITES()
};


void VK_WaitVRB(){
	GBA_WAIT_FOR_VBLANK
	// Wait again
	GBA_WAIT_VBLANK
};

void VK_SetPalette(uint16_t offset){
	if(vk_engine_gstate.gba_palette==0){
		GBA_DMA_Copy16((uint16_t*)GBA_PAL_BG_START,(uint16_t*)vk_ega_palette[offset],17);
		GBA_DMA_Copy16((uint16_t*)GBA_PAL_SPR_START,(uint16_t*)vk_ega_palette[offset],17);
	}
	if(vk_engine_gstate.gba_palette==1){
		GBA_DMA_Copy16((uint16_t*)GBA_PAL_BG_START,(uint16_t*)vk_gb_palette[offset],17);
		GBA_DMA_Copy16((uint16_t*)GBA_PAL_SPR_START,(uint16_t*)vk_gb_palette[offset],17);
	}
	if(vk_engine_gstate.gba_palette==2){
		GBA_DMA_Copy16((uint16_t*)GBA_PAL_BG_START,(uint16_t*)vk_greyscale_palette[offset],17);
		GBA_DMA_Copy16((uint16_t*)GBA_PAL_SPR_START,(uint16_t*)vk_greyscale_palette[offset],17);
	}

};

void VK_FadeOut(){
	// Swap the palettes to the next one
	int fadev = 0;
	
	for(fadev = 0; fadev < 4; fadev++){
		// Copy the palette
		if(vk_engine_gstate.gba_palette==0){
			GBA_DMA_Copy16((uint16_t*)GBA_PAL_BG_START,(uint16_t*)vk_ega_palette[fadev],17);
			GBA_DMA_Copy16((uint16_t*)GBA_PAL_SPR_START,(uint16_t*)vk_ega_palette[fadev],17);
		}
		if(vk_engine_gstate.gba_palette==1){
			GBA_DMA_Copy16((uint16_t*)GBA_PAL_BG_START,(uint16_t*)vk_gb_palette[fadev],17);
			GBA_DMA_Copy16((uint16_t*)GBA_PAL_SPR_START,(uint16_t*)vk_gb_palette[fadev],17);
		}
		if(vk_engine_gstate.gba_palette==2){
			GBA_DMA_Copy16((uint16_t*)GBA_PAL_BG_START,(uint16_t*)vk_greyscale_palette[fadev],17);
			GBA_DMA_Copy16((uint16_t*)GBA_PAL_SPR_START,(uint16_t*)vk_greyscale_palette[fadev],17);
		}
			
		
		GBA_Delay(250);
	}
};

void VK_FadeIn(){
	// Swap the palettes to the next one
	int fadev = 0;
	
	for(fadev = 2; fadev >= 0; fadev-=2){
		// Copy the palette
		if(vk_engine_gstate.gba_palette==0){
			GBA_DMA_Copy16((uint16_t*)GBA_PAL_BG_START,(uint16_t*)vk_ega_palette[fadev],17);
			GBA_DMA_Copy16((uint16_t*)GBA_PAL_SPR_START,(uint16_t*)vk_ega_palette[fadev],17);
		}
		if(vk_engine_gstate.gba_palette==1){
			GBA_DMA_Copy16((uint16_t*)GBA_PAL_BG_START,(uint16_t*)vk_gb_palette[fadev],17);
			GBA_DMA_Copy16((uint16_t*)GBA_PAL_SPR_START,(uint16_t*)vk_gb_palette[fadev],17);
		}
		if(vk_engine_gstate.gba_palette==2){
			GBA_DMA_Copy16((uint16_t*)GBA_PAL_BG_START,(uint16_t*)vk_greyscale_palette[fadev],17);
			GBA_DMA_Copy16((uint16_t*)GBA_PAL_SPR_START,(uint16_t*)vk_greyscale_palette[fadev],17);
		}
					
		GBA_Delay(250);
	}
};

uint16_t VK_TextX = 0;
uint16_t VK_TextY = 0;


void VK_PrintTXT(char *string){
	uint16_t offset = 0;

	uint16_t VK_TextInv = 0x2E0;

	while(*string!=0x00){
		
		if((*string)==126){
			VK_TextInv = 0x360;
			string++;
		}else if((*string)==31){
			string++;
			while(offset<28){
				// Draw blank
				VK_GBA_BG_MAPA[(VK_TextY<<5)+VK_TextX+offset] = VK_TextInv;
				offset++;
			}
			return;
		}else{
			// Draw the letter
			VK_GBA_BG_MAPA[(VK_TextY<<5)+VK_TextX+offset] = VK_TextInv+(*string)-' ';
			if((*string)<' '||(*string)>'z'+2){
				string++;
			}
			offset++;
		}
		if(offset>=28){
			return ; //STOP THE STRING!
		}
		string++;
	}
	while(offset<28){
		// Draw blank
		VK_GBA_BG_MAPA[(VK_TextY<<5)+VK_TextX+offset] = VK_TextInv;
		offset++;
	}
};

void VK_Print(char *string){
	uint16_t offset = 0;

	while(*string!=0x00){
		// Draw the letter
		VK_GBA_BG_MAPA[(VK_TextY<<5)+VK_TextX+offset] = 0x2E0+(*string)-' ';
		offset++;
		string++;
	}
};

void VK_Print2(char *string){
	uint16_t offset = 0;
	while(*string!=0x00){
		// Draw the letter
		VK_GBA_BG_MAPA[(VK_TextY<<5)+VK_TextX+offset] = 0x360+(*string)-' ';
		string++;
		offset++;
	}
};

char vk_iota_str[256];

char * VK_Iota16(int32_t val){
	int16_t i = 0, e = 0;
	if(val==0){
		vk_iota_str[0] = '0';
		vk_iota_str[1] = 0;
		return vk_iota_str;
	}
	if(val < 0){
		val = -val;
		vk_iota_str[i++] = '-';
	}
	i = 0;
	while(val){
		char hexv = '0';
		for(e = 0; e < 16; e++){
			if(e==(val&0xF)){
				hexv = "0123456789ABCDEF"[e];
				break;
			}
		}
		for(e = i; e >=1; e--)
			vk_iota_str[e] = vk_iota_str[e-1];
		i ++;
		vk_iota_str[0] = hexv;
		val >>= 4;
	}
	// End the string
	vk_iota_str[i] = 0;
	return vk_iota_str;
};


char * VK_Iota(int32_t val){
	uint16_t i = 0, e = 0;
	
	if(val==0){
		vk_iota_str[0] = '0';
		vk_iota_str[1] = 0;
		return vk_iota_str;
	}
	i = 0;
	if(val < 0){
		val = -val;
		vk_iota_str[i++] = '-';
	}
	while(val){
		char decv = '0'+(val%10);
		
		for(e = i; e >=1; e--)
			vk_iota_str[e] = vk_iota_str[e-1];
		i ++;
		vk_iota_str[0] = decv;
		val /= 10;
	}

	// End the string
	vk_iota_str[i] = 0;
	return vk_iota_str;
};



