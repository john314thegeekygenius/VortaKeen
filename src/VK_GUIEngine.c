
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

// Intro
#include "../graph/bitmaps/ONE_MOMENT.h"
#include "../graph/bitmaps/INT_APOGEE.h"
#include "../graph/bitmaps/INT_AN.h"
#include "../graph/bitmaps/INT_PREST.h"
#include "../graph/bitmaps/INT_OF_AN.h"
#include "../graph/bitmaps/ID_SOFT.h"
#include "../graph/bitmaps/INT_PROD.h"
// Main Menu
#include "../graph/bitmaps/TITLE_SCREEN.h"
#include "../graph/sheets/CK_FONT.h"
// High Scores
#include "../graph/bitmaps/HIGH_SCORE.h"
#include "../graph/bitmaps/NAME.h"
#include "../graph/bitmaps/SCORE.h"
#include "../graph/bitmaps/PARTS.h"


unsigned char *GBA_PakRam = (unsigned char*)GBA_GAMEPAK_RAM_START;

void VK_WriteInfo(uint32_t block, VK_DATA_BLOCK *data){
	uint32_t i = 0;
	uint8_t header[4] = {0xAA,0xBB,0xCC,0xDD};
	uint32_t *headptr = &header;

	data->errorcode = 0;
	
	for(i = 0; i < 4; i++){
		GBA_PakRam[i+(block*VK_DATA_BLOCK_SIZE)] = header[i];
	}
	
	for(i = 0; i < VK_DATA_BLOCK_SIZE; i++){
		GBA_PakRam[i+(block*VK_DATA_BLOCK_SIZE)+4] = data->data[i];
	}
};

void VK_ReadInfo(uint32_t block,VK_DATA_BLOCK *data){
	uint32_t i = 0;
	uint8_t header[4];
	uint32_t *headptr = &header;

	data->errorcode = 0;
	
	for(i = 0; i < 4; i++){
		header[i] = GBA_PakRam[i+(block*VK_DATA_BLOCK_SIZE)];
	}

	if(*headptr != 0xAABBCCDD){
		data->errorcode = 1;
	}
	for(i = 0; i < VK_DATA_BLOCK_SIZE; i++){
		data->data[i] = GBA_PakRam[i+(block*VK_DATA_BLOCK_SIZE)+4];
	}
	return data;
};



void VK_WaitAMoment(){
	
	int i,e;
	
	// Set some tiles
	for(i = 0; i < 64; i++){
		VK_GBA_BG_Tiles[i] = 0;
		VK_GBA_BG_Tiles[i+64] = 1;
	}
		
	// Clear the screen with black tiles
	for(i = 0; i < 32*32; i++){
		VK_GBA_BG_MAPA[i] = 1;
		VK_GBA_BG_MAPB[i] = 0;
	}

	GBA_DMA_Copy32((VK_GBA_BG_Tiles+128),ONE_MOMENT_data,ONE_MOMENT_size>>2);
	
	// Display the Wait A Moment
	for(e = 0; e < (ONE_MOMENT_height>>3); e++){
		for(i = 0; i < (ONE_MOMENT_width>>3); i++){
			VK_GBA_BG_MAPA[((e+7)<<5)+i+10] = (e*(ONE_MOMENT_width>>3))+i+2;
		}
	}
	// Shift the screen over 4 pixels to center the image completely
	
	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0x4;
	GBA_Delay(2000);
	
	VK_FadeOut();
	
	vk_engine_demo = VK_DEMO_INTRO;
};


void VK_DoIntroDemo(){
	int i,e;
	unsigned int bmptileoff = 0;
	
	// Load the level and tileset
	VK_LoadLevel(90);
	
	// Load the graphics
	bmptileoff = 0;
	GBA_DMA_Copy32((VK_GBA_BG_Tiles2+bmptileoff),INT_APOGEE_data,INT_APOGEE_size>>2);
	bmptileoff += INT_APOGEE_size;
	GBA_DMA_Copy32((VK_GBA_BG_Tiles2+bmptileoff),INT_AN_data,INT_AN_size>>2);
	bmptileoff += INT_AN_size;
	GBA_DMA_Copy32((VK_GBA_BG_Tiles2+bmptileoff),INT_PREST_data,INT_PREST_size>>2);
	bmptileoff += INT_PREST_size;
	GBA_DMA_Copy32((VK_GBA_BG_Tiles2+bmptileoff),INT_OF_AN_data,INT_OF_AN_size>>2);
	bmptileoff += INT_OF_AN_size;
	GBA_DMA_Copy32((VK_GBA_BG_Tiles2+bmptileoff),ID_SOFT_data,ID_SOFT_size>>2);
	bmptileoff += ID_SOFT_size;
	GBA_DMA_Copy32((VK_GBA_BG_Tiles2+bmptileoff),INT_PROD_data,INT_PROD_size>>2);


	// Position the level
	VK_UnLockCamera();
	VK_PositionLevel(vk_level_width-20,3);
	VK_PositionCamera(0x4,0x6);

	// Render the level
	VK_RenderLevel();

	// Lock the camera
	VK_LockCamera();
	
	// Render the APOGEE logo
	for(e = 0; e < (INT_APOGEE_height>>3); e++){
		for(i = 0; i < (INT_APOGEE_width>>3); i++){
			VK_GBA_BG_MAPB[((e+2)<<5)+i] = (e*(INT_APOGEE_width>>3))+i+(VK_GBA_TILES2_OFF);
		}
	}

	int16_t APOGEE_UpdateTick = 0;
	int16_t APOGEE_y = (12*8);
	int16_t APOGEE_x = (INT_APOGEE_width>>1)-120;

	*(volatile uint16_t*)GBA_REG_BG1HOFS = APOGEE_x;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = APOGEE_y;

	// Fade in the level
	VK_FadeIn();
	
	// Scroll up until ready
	
	while(APOGEE_y < (31*8)){
		// Render the level
		VK_UpdateLevel();
		
		VK_RenderLevel();
		
		APOGEE_UpdateTick ++;
		if(APOGEE_UpdateTick > 0x1){
			APOGEE_UpdateTick = 0;
			APOGEE_y += 0x1;
		}
		
		*(volatile uint16_t*)GBA_REG_BG1HOFS = APOGEE_x;
		*(volatile uint16_t*)GBA_REG_BG1VOFS = APOGEE_y;

		VK_WaitVRB();

		// Break on key input
		if(VK_ButtonDown()){
			VK_FadeOut();
			vk_engine_demo = VK_DEMO_MAINMENU;
			return;
		}
	}
	
	bmptileoff = (INT_APOGEE_size>>6);

	// Render the AN text
	for(e = 0; e < (INT_AN_height>>3); e++){
		for(i = 0; i < (INT_AN_width>>3); i++){
			VK_GBA_BG_MAPB[((e+0)<<5)+i+10] = (e*(INT_AN_width>>3))+i+(VK_GBA_TILES2_OFF + bmptileoff);
		}
	}

	bmptileoff += (INT_AN_size>>6);

	// Render the PRESENTAION text
	for(e = 0; e < (INT_PREST_height>>3); e++){
		for(i = 0; i < (INT_PREST_width>>3); i++){
			VK_GBA_BG_MAPB[((e+6)<<5)+i+6] = (e*(INT_PREST_width>>3))+i+(VK_GBA_TILES2_OFF + bmptileoff);
		}
	}
	bmptileoff += (INT_PREST_size>>6);

	// Render the OF AN text
	for(e = 0; e < (INT_OF_AN_height>>3); e++){
		for(i = 0; i < (INT_OF_AN_width>>3); i++){
			VK_GBA_BG_MAPB[((e+8)<<5)+i+9] = (e*(INT_OF_AN_width>>3))+i+(VK_GBA_TILES2_OFF + bmptileoff);
		}
	}
	bmptileoff += (INT_OF_AN_size>>6);

	// Render the ID Software logo
	for(e = 0; e < (ID_SOFT_height>>3); e++){
		for(i = 0; i < (ID_SOFT_width>>3); i++){
			VK_GBA_BG_MAPB[((e+10)<<5)+i+7] = (e*(ID_SOFT_width>>3))+i+(VK_GBA_TILES2_OFF + bmptileoff);
		}
	}
	bmptileoff += (ID_SOFT_size>>6);
	
	// Render the PRODUCTION text
	for(e = 0; e < (INT_PROD_height>>3); e++){
		for(i = 0; i < (INT_PROD_width>>3); i++){
			VK_GBA_BG_MAPB[((e+16)<<5)+i+7] = (e*(INT_PROD_width>>3))+i+(VK_GBA_TILES2_OFF + bmptileoff);
		}
	}

	uint16_t countdown = 0x100;
	
	while(countdown > 0){
		// Render the level
		VK_UpdateLevel();
		
		VK_RenderLevel();
		
		countdown -= 0x1;

		*(volatile uint16_t*)GBA_REG_BG1HOFS = APOGEE_x;
		*(volatile uint16_t*)GBA_REG_BG1VOFS = APOGEE_y;
		
		VK_WaitVRB();

		// Break on key input
		if(VK_ButtonDown()){
			countdown = 0;
		}
	}
	
	VK_FadeOut();
	
	// Render the rest of the things
	vk_engine_demo = VK_DEMO_MAINMENU;

};



void VK_DrawBox(uint16_t spawnx, uint16_t spawny, uint16_t width, uint16_t height){
	uint16_t bx,by;
	// Draw the box
	for(by = spawny; by <= spawny+height; by ++){
		for(bx = spawnx; bx <= spawnx+width; bx ++){
			if(by == spawny){
				VK_GBA_BG_MAPA[(by<<5)+bx] = 0x2C2;
			}else if(by == spawny+height){
				VK_GBA_BG_MAPA[(by<<5)+bx] = 0x2C7;
			}else if(bx == spawnx){
				VK_GBA_BG_MAPA[(by<<5)+bx] = 0x2C4;
			}else if(bx == spawnx+width){
				VK_GBA_BG_MAPA[(by<<5)+bx] = 0x2C5;
			}else{
				VK_GBA_BG_MAPA[(by<<5)+bx] = 0x2E0;
			}
		}
	}
	bx = spawnx;
	by = spawny;
	VK_GBA_BG_MAPA[(by<<5)+bx] = 0x2C1;
	VK_GBA_BG_MAPA[(by<<5)+bx+width] = 0x2C3;
	VK_GBA_BG_MAPA[((by+height)<<5)+bx] = 0x2C6;
	VK_GBA_BG_MAPA[((by+height)<<5)+bx+width] = 0x2C8;

};

void VK_SpawnBox(uint16_t spawnx, uint16_t spawny, uint16_t width, uint16_t height){
	uint16_t bw,bh,bx,by;
	
	bh = 0;
	bw = 0;
	
	if(width<=0 || height <=0)
		return;

	while(bw < (width>>1)){
		bw ++;
		VK_DrawBox(spawnx-bw,spawny-bh,bw<<1,2);
		GBA_WAIT_VBLANK
		GBA_Delay(25);
	}
	while(bh < (height>>1)){
		bh ++;
		VK_DrawBox(spawnx-(width>>1),spawny-bh,width,bh<<1);
		GBA_WAIT_VBLANK
		GBA_Delay(25);
	}
	VK_DrawBox(spawnx-(width>>1),spawny-(height>>1),width,height);
	// 0x2C1
};

uint8_t *high_score_names[3] = {
	"Yorpy",
	"Gargile",
	"Zzapp!",
};

void VK_DrawHighScores(){
	int i,e;
	unsigned int bmptileoff = 0;
	
	vk_highscore vk_thekeenest[6];

	
	// These need to be reloaded in case they were overwritten

	// Load the level and tileset
	VK_LoadLevel(90);
	// Load the font bitmap
	GBA_DMA_Copy32((VK_GBA_BG_TilesEnd-ck_font_size),ck_font_data,ck_font_size>>2);
	
	
	// Load the high scores
	VK_DATA_BLOCK scores;

	// Read the data
	VK_ReadInfo(0,&scores);

	// Copy if ok
	if(scores.errorcode==0){
		GBA_DMA_Copy32( &vk_thekeenest, &scores.data, sizeof(vk_highscore)>>2);
	}else{
		for(i = 0; i < 6; i++){
			vk_thekeenest[i].score = 100;
			for(e = 0; e < 4; e++)
				vk_thekeenest[i].items[e] = 0;
			for(e = 0; e < 7; e++)
				vk_thekeenest[i].citys[e] = 0;
			memcpy(vk_thekeenest[i].name, high_score_names[i%3], strlen(high_score_names[i%3]) );
		}
		GBA_DMA_Copy32(&scores.data, &vk_thekeenest, sizeof(vk_thekeenest)>>2);

		// Write the data
		VK_WriteInfo(0,&scores);
	}
	
	// Sort the scores (bubble)
	for(i = 0; i < 6; i++){
		for(e = 0; e < 5; e++){
			if(vk_thekeenest[e].score > vk_thekeenest[e+1].score){
				vk_highscore tmp = vk_thekeenest[e+1];
				vk_thekeenest[e+1] = vk_thekeenest[e];
				vk_thekeenest[e] = tmp;
			}
		}
	}
	
	// Load the graphics
	bmptileoff = 0;
	GBA_DMA_Copy32((VK_GBA_BG_Tiles2+bmptileoff),HIGH_SCORE_data,HIGH_SCORE_size>>2);
	bmptileoff += HIGH_SCORE_size;
	GBA_DMA_Copy32((VK_GBA_BG_Tiles2+bmptileoff),NAME_data,NAME_size>>2);
	bmptileoff += NAME_size;
	GBA_DMA_Copy32((VK_GBA_BG_Tiles2+bmptileoff),SCORE_data,SCORE_size>>2);
	bmptileoff += SCORE_size;
	GBA_DMA_Copy32((VK_GBA_BG_Tiles2+bmptileoff),PARTS_data,PARTS_size>>2);
	bmptileoff += PARTS_size;

	// Position the level
	VK_UnLockCamera();
	VK_PositionLevel(vk_level_width-40,2);
	VK_PositionCamera(0x6,0x6);

	// Render the level
	VK_RenderLevel();

	// Lock the camera
	VK_LockCamera();
	
	// Render the HIGH SCORES logo
	for(e = 0; e < (HIGH_SCORE_height>>3); e++){
		for(i = 0; i < (HIGH_SCORE_width>>3); i++){
			VK_GBA_BG_MAPB[((e+2)<<5)+i+7] = (e*(HIGH_SCORE_width>>3))+i+(VK_GBA_TILES2_OFF);
		}
	}
	
	bmptileoff = (HIGH_SCORE_size>>6);

	// Render the AN text
	for(e = 0; e < (NAME_height>>3); e++){
		for(i = 0; i < (NAME_width>>3); i++){
			VK_GBA_BG_MAPB[((e+6)<<5)+i+0] = (e*(NAME_width>>3))+i+(VK_GBA_TILES2_OFF + bmptileoff);
		}
	}

	bmptileoff += (NAME_size>>6);

	// Render the PRESENTAION text
	for(e = 0; e < (SCORE_height>>3); e++){
		for(i = 0; i < (SCORE_width>>3); i++){
			VK_GBA_BG_MAPB[((e+6)<<5)+i+14] = (e*(SCORE_width>>3))+i+(VK_GBA_TILES2_OFF + bmptileoff);
		}
	}
	bmptileoff += (SCORE_size>>6);

	// Render the OF AN text
	for(e = 0; e < (PARTS_height>>3); e++){
		for(i = 0; i < (PARTS_width>>3); i++){
			VK_GBA_BG_MAPB[((e+6)<<5)+i+24] = (e*(PARTS_width>>3))+i+(VK_GBA_TILES2_OFF + bmptileoff);
		}
	}
	

	*(volatile uint16_t*)GBA_REG_BG1HOFS = 0x00;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = 0x0F;

	// Fade in the level
	VK_FadeIn();
	
	while(1){

		// Render the level
		VK_UpdateLevel();
		
		VK_RenderLevel();

		// Write the names
		VK_TextX = 1;
		VK_TextY = 8;
		for(i = 0; i < 6; i++){
			VK_Print2(vk_thekeenest[i].name);
			VK_TextY += 2;
		}
		
		VK_WaitVRB();

		// Break on key input
		if(VK_ButtonDown()){
			break;
		}
	}
	
	VK_FadeOut();
	
	// Render the rest of the things
	vk_engine_demo = VK_DEMO_MAINMENU;

};

void VK_DrawStory(){
	int i,e;
	unsigned int bmptileoff = 0;
	
	// Load the level and tileset
	VK_LoadLevel(90);
	// Load the font bitmap
	GBA_DMA_Copy32((VK_GBA_BG_TilesEnd-ck_font_size),ck_font_data,ck_font_size>>2);
	

	// Position the level
	VK_UnLockCamera();
	VK_PositionLevel(vk_level_width-84,5);
	VK_PositionCamera(0x0,0x0);

	// Render the level
	VK_RenderLevel();

	// Lock the camera
	VK_LockCamera();
	
	// Fade in the level
	VK_FadeIn();
	
	VK_DrawBox(0,0,29,14);
	VK_DrawBox(0,12,29,2);
	
	while(1){

		// Render story text

		VK_WaitVRB();

		// Break on key input
		if(VK_ButtonDown()){
			break;
		}
	}

	VK_FadeOut();
	
	// Render the rest of the things
	vk_engine_demo = VK_DEMO_MAINMENU;
};



void VK_DrawTitleScreen(){
	
	int i,e;
	
	// Set some tiles
	for(i = 0; i < 64; i++){
		VK_GBA_BG_Tiles[i] = 0;
		VK_GBA_BG_Tiles[i+64] = 1;
	}
		
	// Clear the screen with black tiles
	for(i = 0; i < 32*32; i++){
		VK_GBA_BG_MAPA[i] = 1;
		VK_GBA_BG_MAPB[i] = 0;
	}

	GBA_DMA_Copy32((VK_GBA_BG_Tiles+128),TITLE_SCREEN_data,TITLE_SCREEN_size>>2);
	
	// Display the title screen
	for(e = 0; e < (TITLE_SCREEN_height>>3); e++){
		for(i = 0; i < (TITLE_SCREEN_width>>3); i++){
			VK_GBA_BG_MAPA[(e<<5)+i] = (e*(TITLE_SCREEN_width>>3))+i+2;
		}
	}
	
	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG1HOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = 0x0;
	
	vk_engine_demo = VK_DEMO_MAINMENU;
};


uint16_t VK_LoadMenu(){
	VK_SpawnBox(14,9,29,3);
	
	// Write the text on the dialog box
	VK_TextX = 6;
	VK_TextY = 9;
	VK_Print("Continue Which Game?");
	VK_TextX = 2;
	VK_TextY = 10;
	VK_Print("1: 2: 3: 4: 5: 6: 7: 8: 9: ");

	uint16_t cursor_animation = 0;
	uint16_t cursor_ani_tick = 0;
	uint16_t cursor_x = 0;
	
	while(1){
		
		VK_UpdateInput();
		
		// Do Menu Logic
		if(VK_ButtonUp()==GBA_BUTTON_A){
			
			if(1){
				// Spawn a box
				VK_SpawnBox(14,9,29,3);
				
				// Write the text on the dialog box
				VK_TextX = 4;
				VK_TextY = 9;
				VK_Print("That game hasn't been");
				VK_TextX = 4;
				VK_TextY = 10;
				VK_Print("saved yet!:");
				
				VK_UpdateInput();

				// Wait for button press
				while(!VK_ButtonUp()){
					VK_UpdateInput();
					
					cursor_ani_tick++;
					if(cursor_ani_tick>0x80){
						cursor_ani_tick = 0;
						cursor_animation += 1;
						if(cursor_animation>5){
							cursor_animation = 0;
						}
					}

					*(volatile uint16_t*)GBA_REG_BG1HOFS = -0x78;
					*(volatile uint16_t*)GBA_REG_BG1VOFS = -0x50;

					// Draw the cursor
					VK_GBA_BG_MAPB[0] = 0x2C9+cursor_animation;
					
					VK_WaitVRB();

				}
				// Redraw the main one
				VK_SpawnBox(14,9,29,3);
				
				// Write the text on the dialog box
				VK_TextX = 6;
				VK_TextY = 9;
				VK_Print("Continue Which Game?");
				VK_TextX = 2;
				VK_TextY = 10;
				VK_Print("1: 2: 3: 4: 5: 6: 7: 8: 9: ");
			}
			
		}
		if(VK_ButtonUp()==GBA_BUTTON_B){
			return 0;
		}

		// Move the cursor
		if(VK_ButtonUp()==GBA_BUTTON_LEFT){
			if(cursor_x==0){
				cursor_x = 8*24;
			}else{
				cursor_x -= 24;
			}
		}
		if(VK_ButtonUp()==GBA_BUTTON_RIGHT){
			if(cursor_x==8*24){
				cursor_x = 0;
			}else{
				cursor_x += 24;
			}
		}
		
		cursor_ani_tick++;
		if(cursor_ani_tick>0x2){
			cursor_ani_tick = 0;
			cursor_animation += 1;
			if(cursor_animation>5){
				cursor_animation = 0;
			}
		}

		// Position the cursor
		*(volatile uint16_t*)GBA_REG_BG1HOFS = -0x20-cursor_x;
		*(volatile uint16_t*)GBA_REG_BG1VOFS = -0x50;

		// Draw the cursor
		VK_GBA_BG_MAPB[0] = 0x2C9+cursor_animation;
		
		VK_WaitVRB();
	}
	
	return 0;
};

void VK_DrawMainMenu(){

	VK_SpawnBox(15,10,20,8);
	
	// Write the text on the dialog box
	VK_TextX = 10;
	VK_TextY = 7;
	VK_Print("New Game");
	VK_TextY += 1;
	VK_Print("Continue Game");
	VK_TextY += 1;
	VK_Print("Story");
	VK_TextY += 1;
	VK_Print("High Scores");
	VK_TextY += 1;
	VK_Print("Restart Demo");
	VK_TextY += 2;
	VK_TextX -= 3;
	VK_Print("Use The D-Pad");
	
};

void VK_MainMenu(){
	// Load the font bitmap
	GBA_DMA_Copy32((VK_GBA_BG_TilesEnd-ck_font_size),ck_font_data,ck_font_size>>2);
	
	VK_FadeIn();
	
	while(1){
		// Break on key input
		if(VK_ButtonDown()){
			break;
		}
		VK_WaitVRB();
	}

	VK_DrawMainMenu();
	
	uint16_t cursor_animation = 0;
	uint16_t cursor_ani_tick = 0;
	uint16_t cursor_y = 0;
	uint16_t cursor_to_y = 0;
	uint16_t cursor_slide_tick = 0;

	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0x0;
	
	while(1){
		
		VK_UpdateInput();
		
		// Do Menu Logic
		if(VK_ButtonUp()==GBA_BUTTON_A){
			switch(cursor_y){
				case 0:
				// Start a new game
				VK_FadeOut();
				vk_engine_demo = VK_DEMO_GAME;
				return;
				case 8:
				// Load game dialog
				if(VK_LoadMenu()){
					// Start a game
					VK_FadeOut();
					vk_engine_demo = VK_DEMO_GAME;
					return;
				}else{
					// Reset the menu
					VK_DrawTitleScreen();
					VK_DrawMainMenu();
					cursor_y = 0;
					cursor_to_y = 0;
					cursor_slide_tick = 0;
				}
				break;
				case 16:
				// Story
				VK_FadeOut();
				vk_engine_demo = VK_DEMO_STORY;
				return;
				case 24:
				// High Scores
				VK_FadeOut();
				vk_engine_demo = VK_DEMO_HIGHSCORES;
				return;
				case 32:
				// Restart Demo
				VK_FadeOut();
				vk_engine_demo = VK_DEMO_MAINMENU;
				return;
				break;
			}
		}
		
		// Move the cursor
		if(cursor_y == cursor_to_y){

			if(VK_ButtonUp()==GBA_BUTTON_UP){
				
				if(cursor_y==0){
					cursor_y = (8*4);
					cursor_to_y = (8*4);
				}else{
					cursor_to_y = cursor_y - 8;
				}
			}
			if(VK_ButtonUp()==GBA_BUTTON_DOWN){
				if(cursor_y==(8*4)){
					cursor_y = 0;
					cursor_to_y = 0;
				}else{
					cursor_to_y = cursor_y + 8;
				}
			}
		}
		cursor_ani_tick++;
		if(cursor_ani_tick>0x2){
			cursor_ani_tick = 0;
			cursor_animation += 1;
			if(cursor_animation>5){
				cursor_animation = 0;
			}
		}
		
		cursor_slide_tick ++;
		if(cursor_slide_tick>0x0){
			cursor_slide_tick = 0;
			// Move the cursor
			if(cursor_y < cursor_to_y){
				cursor_y += 1;
			}
			if(cursor_y > cursor_to_y){
				cursor_y -= 1;
			}
		}


		// Draw the cursor
		VK_GBA_BG_MAPB[0] = 0x2C9+cursor_animation;
		
		*(volatile uint16_t*)GBA_REG_BG1HOFS = -0x40;
		*(volatile uint16_t*)GBA_REG_BG1VOFS = -0x38-cursor_y;
		
		VK_WaitVRB();
	}
};

