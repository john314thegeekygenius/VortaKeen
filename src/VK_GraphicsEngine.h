
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

#ifndef __VK_GRAPHICS_ENGINE__
#define __VK_GRAPHICS_ENGINE__

extern volatile uint16_t* VK_GBA_BG_MAPA ;
extern volatile uint16_t* VK_GBA_BG_MAPB ;

extern volatile uint8_t* VK_GBA_BG_Tiles;
extern volatile uint8_t* VK_GBA_BG_Tiles2;
extern volatile uint8_t* VK_GBA_BG_Tiles3;
extern volatile uint8_t* VK_GBA_BG_TilesEnd;


extern const uint32_t VK_GBA_TILES2_OFF;
extern const uint32_t VK_GBA_TILES3_OFF;
extern const uint32_t VK_GBA_TILES_END_OFF;

void VK_SetupVideo();

void VK_SetPalette(uint16_t offset);

void VK_FadeOut();
void VK_FadeIn();

extern uint16_t VK_TextX;
extern uint16_t VK_TextY;
void VK_PrintTXT(char *string);
void VK_Print(char *string);
void VK_Print2(char *string);
char * VK_Iota16(int32_t val);
char * VK_Iota(int32_t val);
	
void VK_WaitVRB();

#endif
