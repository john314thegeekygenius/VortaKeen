
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

// Include the texts
#include "../text/VK_TEXTS.h"
#include "../text/STORYTXT.h"
#include "../text/ENDTEXT.h"
//#include "../text/HELPTEXT.h"

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
// Ending
#include "../graph/bitmaps/ENDSCREEN.h"
#include "../graph/bitmaps/WINDOW_OFF.h"
#include "../graph/bitmaps/WINDOW_ON.h"


unsigned char *GBA_PakRam = (unsigned char*)GBA_GAMEPAK_RAM_START;

void VK_WriteInfo(uint32_t block, VK_DATA_BLOCK *data){
	uint32_t i = 0;
	uint8_t header[4] = {0xAA,0xBB,0xCC,0xDD};
	uint32_t *headptr = &header;

	data->errorcode = 0;
	
	for(i = 0; i < 4; i++){
		GBA_PakRam[i+(block*VK_DATA_BLOCK_SIZE)] = header[i];
	}
	
	for(i = 0; i < VK_DATA_BLOCK_SIZE-4; i++){
		GBA_PakRam[i+(block*VK_DATA_BLOCK_SIZE)+4] = data->data[i];
	}
};

void VK_ReadInfo(uint32_t block,VK_DATA_BLOCK *data){
	uint32_t i = 0;
	uint8_t header[4];

	data->errorcode = 0;
	
	for(i = 0; i < 4; i++){
		header[i] = GBA_PakRam[i+(block*VK_DATA_BLOCK_SIZE)];
	}

	data->errorcode = 1;
	if(header[0] == 0xAA){
		if(header[1] == 0xBB){
			if(header[2] == 0xCC){
				if(header[3] == 0xDD){
					data->errorcode = 0;
				}
			}
		}
	}
	for(i = 0; i < VK_DATA_BLOCK_SIZE-4; i++){
		data->data[i] = GBA_PakRam[i+(block*VK_DATA_BLOCK_SIZE)+4];
	}
	return data;
};



void VK_FormatROM(){
	unsigned int i, e;
	
	// Clear the ROM Save
	for(e = 0; e < 16; e++){
		for(i = 0; i < (VK_DATA_BLOCK_SIZE+8); i++){
			GBA_PakRam[(e*VK_DATA_BLOCK_SIZE)+i] = 0xFF;
		}
	}

	vk_engine_gstate.sound_enabled = 1;
	vk_engine_gstate.music_enabled = 1;
	vk_engine_gstate.gba_palette = 0;
	
	VK_SetPalette(0);

	// Reset the highscores
	for(i = 0; i < 6; i++){
		vk_engine_gstate.thekeenest[i].score = 100;
		for(e = 0; e < 4; e++)
			vk_engine_gstate.thekeenest[i].items[e] = 0;
		for(e = 0; e < 7; e++)
			vk_engine_gstate.thekeenest[i].citys[e] = 0;
		memcpy(vk_engine_gstate.thekeenest[i].name, HIGH_SCORE_NAMES[i%3], strlen(HIGH_SCORE_NAMES[i%3])+1 );
	}

	// Write the data
	VK_SaveOptions();

	VK_SaveHighScores();
	
};

void VK_SaveOptions(){

	VK_DATA_BLOCK SaveData;
	int i;

	// Clear the data
	memset(&SaveData,0,sizeof(VK_DATA_BLOCK));

	uint8_t *data = &SaveData.data;

	// Write the data
	data[0] = vk_engine_gstate.sound_enabled;
	data[1] = vk_engine_gstate.music_enabled;
	data[2] = vk_engine_gstate.gba_palette;

	// Write to ROM
	VK_WriteInfo(1,&SaveData);
};

int VK_LoadOptions(){

	VK_DATA_BLOCK ld;
	int i;

	vk_engine_gstate.sound_enabled = 1;
	vk_engine_gstate.music_enabled = 1;
	vk_engine_gstate.gba_palette = 0;

	VK_SetPalette(0);

	// Read from ROM
	VK_ReadInfo(1,&ld);
	
	if(ld.errorcode){
		return 1;
	}

	uint8_t *data = &ld.data;

	// Read the data
	vk_engine_gstate.sound_enabled = data[0];
	vk_engine_gstate.music_enabled = data[1];
	vk_engine_gstate.gba_palette = data[2];
	
	VK_SoundEnabled = vk_engine_gstate.sound_enabled;
	VK_MusicEnabled = vk_engine_gstate.music_enabled;

	VK_SetPalette(0);
	return 0;
};


void VK_SaveHighScores(){
	int i,e;
	// Save the high scores
	VK_DATA_BLOCK scores;
	
	// Copy the data
	uint8_t *sd = &scores.data;
	for(i = 0; i < 6; i++){
		*sd = (vk_engine_gstate.thekeenest[i].score)&0xFF; sd += 1;
		*sd = (vk_engine_gstate.thekeenest[i].score>>8)&0xFF; sd += 1;
		*sd = (vk_engine_gstate.thekeenest[i].score>>16)&0xFF; sd += 1;
		*sd = (vk_engine_gstate.thekeenest[i].score>>24)&0xFF; sd += 1;

		for(e = 0; e < 4; e++){
			*sd = (vk_engine_gstate.thekeenest[i].items[e])&0xFF; sd += 1;
		}
		for(e = 0; e < 7; e++){
			*sd = (vk_engine_gstate.thekeenest[i].citys[e])&0xFF; sd += 1;
		}
		for(e = 0; e < 14; e++){
			*sd = (vk_engine_gstate.thekeenest[i].name[e])&0xFF; sd += 1;
		}
	}

	// Save the data
	VK_WriteInfo(0,&scores);
};

void VK_LoadHighScores(){
	int i,e;
	// Load the high scores
	VK_DATA_BLOCK scores;

	// Read the data
	VK_ReadInfo(0,&scores);
	
	if(scores.errorcode){
		// Reset the highscores
		for(i = 0; i < 6; i++){
			vk_engine_gstate.thekeenest[i].score = 100;
			for(e = 0; e < 4; e++)
				vk_engine_gstate.thekeenest[i].items[e] = 0;
			for(e = 0; e < 7; e++)
				vk_engine_gstate.thekeenest[i].citys[e] = 0;
			memcpy(vk_engine_gstate.thekeenest[i].name, HIGH_SCORE_NAMES[i%3], strlen(HIGH_SCORE_NAMES[i%3])+1 );
		}

		// Write the data
		VK_SaveHighScores();
		return;
	}
	
	// Copy the data
	uint8_t *sd = &scores.data;
	for(i = 0; i < 6; i++){
		vk_engine_gstate.thekeenest[i].score = (*sd); sd += 1;
		vk_engine_gstate.thekeenest[i].score |= (*sd)<<8; sd += 1;
		vk_engine_gstate.thekeenest[i].score |= (*sd)<<16; sd += 1;
		vk_engine_gstate.thekeenest[i].score |= (*sd)<<24; sd += 1;

		for(e = 0; e < 4; e++){
			vk_engine_gstate.thekeenest[i].items[e] = (*sd); sd += 1;
		}
		for(e = 0; e < 7; e++){
			vk_engine_gstate.thekeenest[i].citys[e] = (*sd); sd += 1;
		}
		for(e = 0; e < 14; e++){
			vk_engine_gstate.thekeenest[i].name[e] = (*sd); sd += 1;
		}
	}

	
	// Sort the scores (bubble)
	for(i = 0; i < 6; i++){
		for(e = 0; e < 5; e++){
			if(vk_engine_gstate.thekeenest[e].score > vk_engine_gstate.thekeenest[e+1].score){
				vk_highscore tmp = vk_engine_gstate.thekeenest[e+1];
				vk_engine_gstate.thekeenest[e+1] = vk_engine_gstate.thekeenest[e];
				vk_engine_gstate.thekeenest[e] = tmp;
			}
		}
	}
};


int VK_SaveGame(int slot){
	VK_DATA_BLOCK SaveData;
	int i;

	// Clear the data
	memset(&SaveData,0,sizeof(VK_DATA_BLOCK));

	uint8_t *data = &SaveData.data;

	// Write the save data
	*data = 0x31; data += 1; // Save file checksum
	*data = 0x45; data += 1;

	*data = (vk_engine_gstate.num_of_done)&0xFF; data += 1;
	*data = (vk_engine_gstate.num_of_done>>8)&0xFF; data += 1;

	*data = (vk_engine_gstate.gotBattery)&0xFF; data += 1;
	*data = (vk_engine_gstate.gotBattery>>8)&0xFF; data += 1;

	*data = (vk_engine_gstate.gotVacuum)&0xFF; data += 1;
	*data = (vk_engine_gstate.gotVacuum>>8)&0xFF; data += 1;

	*data = (vk_engine_gstate.gotJoystick)&0xFF; data += 1;
	*data = (vk_engine_gstate.gotJoystick>>8)&0xFF; data += 1;

	*data = (vk_engine_gstate.gotWhiskey)&0xFF; data += 1;
	*data = (vk_engine_gstate.gotWhiskey>>8)&0xFF; data += 1;

	*data = (vk_engine_gstate.gotPogo)&0xFF; data += 1;
	*data = (vk_engine_gstate.gotPogo>>8)&0xFF; data += 1;

	*data = (vk_engine_gstate.gotKeycardR)&0xFF; data += 1;
	*data = (vk_engine_gstate.gotKeycardR>>8)&0xFF; data += 1;

	*data = (vk_engine_gstate.gotKeycardG)&0xFF; data += 1;
	*data = (vk_engine_gstate.gotKeycardG>>8)&0xFF; data += 1;

	*data = (vk_engine_gstate.gotKeycardB)&0xFF; data += 1;
	*data = (vk_engine_gstate.gotKeycardB>>8)&0xFF; data += 1;

	*data = (vk_engine_gstate.gotKeycardY)&0xFF; data += 1;
	*data = (vk_engine_gstate.gotKeycardY>>8)&0xFF; data += 1;

	for(i = 0; i < 16; i++){
		*data = (vk_engine_gstate.levelDone[i])&0xFF; data += 1;
		*data = (vk_engine_gstate.levelDone[i]>>8)&0xFF; data += 1;
	}
	for(i = 0; i < 9; i++){
		*data = (vk_engine_gstate.cities[i])&0xFF; data += 1;
		*data = (vk_engine_gstate.cities[i]>>8)&0xFF; data += 1;
	}

	*data = (vk_engine_gstate.numLives)&0xFF; data += 1;
	*data = (vk_engine_gstate.numLives>>8)&0xFF; data += 1;

	*data = (vk_engine_gstate.ammo)&0xFF; data += 1;
	*data = (vk_engine_gstate.ammo>>8)&0xFF; data += 1;

	*data = (vk_engine_gstate.score)&0xFF; data += 1;
	*data = (vk_engine_gstate.score>>8)&0xFF; data += 1;
	*data = (vk_engine_gstate.score>>16)&0xFF; data += 1;
	*data = (vk_engine_gstate.score>>24)&0xFF; data += 1;

	*data = (vk_engine_gstate.next_1up)&0xFF; data += 1;
	*data = (vk_engine_gstate.next_1up>>8)&0xFF; data += 1;
	*data = (vk_engine_gstate.next_1up>>16)&0xFF; data += 1;
	*data = (vk_engine_gstate.next_1up>>24)&0xFF; data += 1;

	*data = (vk_engine_gstate.posX)&0xFF; data += 1;
	*data = (vk_engine_gstate.posX>>8)&0xFF; data += 1;
	*data = (vk_engine_gstate.posX>>16)&0xFF; data += 1;
	*data = (vk_engine_gstate.posX>>24)&0xFF; data += 1;

	*data = (vk_engine_gstate.posY)&0xFF; data += 1;
	*data = (vk_engine_gstate.posY>>8)&0xFF; data += 1;
	*data = (vk_engine_gstate.posY>>16)&0xFF; data += 1;
	*data = (vk_engine_gstate.posY>>24)&0xFF; data += 1;

	*data = (vk_engine_gstate.viewportX)&0xFF; data += 1;
	*data = (vk_engine_gstate.viewportX>>8)&0xFF; data += 1;
	*data = (vk_engine_gstate.viewportX>>16)&0xFF; data += 1;
	*data = (vk_engine_gstate.viewportX>>24)&0xFF; data += 1;

	*data = (vk_engine_gstate.viewportY)&0xFF; data += 1;
	*data = (vk_engine_gstate.viewportY>>8)&0xFF; data += 1;
	*data = (vk_engine_gstate.viewportY>>16)&0xFF; data += 1;
	*data = (vk_engine_gstate.viewportY>>24)&0xFF; data += 1;

	// Save the game
	VK_WriteInfo(2+slot,&SaveData);

	return 0;
	
};

int VK_CheckSave(int slot){
	VK_DATA_BLOCK db;
	VK_ReadInfo(2+slot,&db);
	if(db.errorcode==0){
		if(db.data[0]==0x31){
			if(db.data[1]==0x45){
				return 1;
			}
		}
	}
	return 0;
};

int VK_LoadGame(int slot){
	VK_DATA_BLOCK LoadData;
	int i;
	
	VK_ReadInfo(2+slot,&LoadData);

	if(LoadData.errorcode){
		return 1;
	}

	uint8_t *data = &LoadData.data;

	// Read the save data
	
	if((data[0] != 0x31) || (data[1] != 0x45)){
		return 1;
	}
	data += 2;

	vk_engine_gstate.num_of_done = (*data); data+=1;
	vk_engine_gstate.num_of_done |= (*data)<<8; data+=1;

	vk_engine_gstate.gotBattery = (*data); data+=1;
	vk_engine_gstate.gotBattery |= (*data)<<8; data+=1;

	vk_engine_gstate.gotVacuum = (*data); data+=1;
	vk_engine_gstate.gotVacuum |= (*data)<<8; data+=1;

	vk_engine_gstate.gotJoystick = (*data); data+=1;
	vk_engine_gstate.gotJoystick |= (*data)<<8; data+=1;

	vk_engine_gstate.gotWhiskey = (*data); data+=1;
	vk_engine_gstate.gotWhiskey |= (*data)<<8; data+=1;

	vk_engine_gstate.gotPogo = (*data); data+=1;
	vk_engine_gstate.gotPogo |= (*data)<<8; data+=1;

	vk_engine_gstate.gotKeycardR = (*data); data+=1;
	vk_engine_gstate.gotKeycardR |= (*data)<<8; data+=1;

	vk_engine_gstate.gotKeycardG = (*data); data+=1;
	vk_engine_gstate.gotKeycardG |= (*data)<<8; data+=1;

	vk_engine_gstate.gotKeycardB = (*data); data+=1;
	vk_engine_gstate.gotKeycardB |= (*data)<<8; data+=1;

	vk_engine_gstate.gotKeycardY = (*data); data+=1;
	vk_engine_gstate.gotKeycardY |= (*data)<<8; data+=1;

	for(i = 0; i < 16; i++){
		vk_engine_gstate.levelDone[i] = (*data); data+=1;
		vk_engine_gstate.levelDone[i] |= (*data)<<8; data+=1;
	}
	for(i = 0; i < 9; i++){
		vk_engine_gstate.cities[i] = (*data); data+=1;
		vk_engine_gstate.cities[i] |= (*data)<<8; data+=1;
	}

	vk_engine_gstate.numLives = (*data); data+=1;
	vk_engine_gstate.numLives |= (*data)<<8; data+=1;

	vk_engine_gstate.ammo = (*data); data+=1;
	vk_engine_gstate.ammo |= (*data)<<8; data+=1;

	vk_engine_gstate.score = (*data); data+=1;
	vk_engine_gstate.score |= (*data)<<8; data+=1;
	vk_engine_gstate.score |= (*data)<<16; data+=1;
	vk_engine_gstate.score |= (*data)<<24; data+=1;

	vk_engine_gstate.next_1up = (*data); data+=1;
	vk_engine_gstate.next_1up |= (*data)<<8; data+=1;
	vk_engine_gstate.next_1up |= (*data)<<16; data+=1;
	vk_engine_gstate.next_1up |= (*data)<<24; data+=1;

	vk_engine_gstate.posX = (*data); data+=1;
	vk_engine_gstate.posX |= (*data)<<8; data+=1;
	vk_engine_gstate.posX |= (*data)<<16; data+=1;
	vk_engine_gstate.posX |= (*data)<<24; data+=1;

	vk_engine_gstate.posY = (*data); data+=1;
	vk_engine_gstate.posY |= (*data)<<8; data+=1;
	vk_engine_gstate.posY |= (*data)<<16; data+=1;
	vk_engine_gstate.posY |= (*data)<<24; data+=1;

	vk_engine_gstate.viewportX = (*data); data+=1;
	vk_engine_gstate.viewportX |= (*data)<<8; data+=1;
	vk_engine_gstate.viewportX |= (*data)<<16; data+=1;
	vk_engine_gstate.viewportX |= (*data)<<24; data+=1;

	vk_engine_gstate.viewportY = (*data); data+=1;
	vk_engine_gstate.viewportY |= (*data)<<8; data+=1;
	vk_engine_gstate.viewportY |= (*data)<<16; data+=1;
	vk_engine_gstate.viewportY |= (*data)<<24; data+=1;


	return 0;
	
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
		VK_DrawBox(spawnx-bw,spawny-bh,bw<<1,1);
		bw ++;
		GBA_WAIT_VBLANK
		GBA_Delay(25);
	}
	while(bh < (height>>1)){
		VK_DrawBox(spawnx-(width>>1),spawny-bh,width,bh<<1);
		bh ++;
		GBA_WAIT_VBLANK
		GBA_Delay(25);
	}
	VK_DrawBox(spawnx-(width>>1),spawny-(height>>1),width,height);
	// 0x2C1
};



void VK_DrawHighScores(){
	int i,e;
	unsigned int bmptileoff = 0;
	
	// These need to be reloaded in case they were overwritten

	// Load the level and tileset
	VK_LoadLevel(90);
	// Load the font bitmap
	GBA_DMA_Copy32((VK_GBA_BG_TilesEnd-ck_font_size),ck_font_data,ck_font_size>>2);
	
	//VK_LoadHighScores();
	
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
	
	uint16_t item_animation = 0;
	uint16_t item_tick = 0;
	
	while(1){

		// Render the level
		VK_UpdateLevel();
		
		VK_RenderLevel();

		// Write the names
		VK_TextY = 8;
		for(i = 0; i < 6; i++){
			char *scrstr = VK_Iota(vk_engine_gstate.thekeenest[i].score);
			VK_TextX = 1;
			VK_Print2(vk_engine_gstate.thekeenest[i].name);
			VK_TextX = 20-strlen(scrstr);
			VK_Print2(scrstr);
			if(vk_engine_gstate.thekeenest[i].items[0]){
				VK_DrawTile(22,7+(i<<1),(vk_num_of_tiles-16)+item_animation);
			}
			if(vk_engine_gstate.thekeenest[i].items[1]){
				VK_DrawTile(24,7+(i<<1),(vk_num_of_tiles-12)+item_animation);
			}
			if(vk_engine_gstate.thekeenest[i].items[2]){
				VK_DrawTile(26,7+(i<<1),(vk_num_of_tiles-8)+item_animation);
			}
			if(vk_engine_gstate.thekeenest[i].items[3]){
				VK_DrawTile(28,7+(i<<1),(vk_num_of_tiles-4)+item_animation);
			}
			
			VK_TextY += 2;
		}
		item_tick += 1;
		if(item_tick > 0x8){
			item_tick = 0;
			item_animation += 1;
			if(item_animation>3){
				item_animation = 0;
			}
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
	uint16_t updatetext = 1;
	int TextScroll = 0;
	
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

	// Write instructions
	VK_TextX = 1;
	VK_TextY = 13;
	VK_Print2("  B To Exit / \17 \23 to Read   ");
	
	while(1){

		VK_UpdateInput();

		if(VK_CheckButton(GBA_BUTTON_UP)){
			TextScroll -= 1;
			if(TextScroll<0){
				TextScroll = 0;
			}else{
				updatetext = 1;
			}
		}
		
		if(VK_CheckButton(GBA_BUTTON_DOWN)){
			TextScroll += 1;
			if(TextScroll>=STORYTXT_length-9){
				TextScroll = STORYTXT_length-9;
			}else{
				updatetext = 1;
			}
		}

		// Break on key input
		if(VK_ButtonUp()==GBA_BUTTON_B){
			break;
		}
		
		// Render story text
		if(updatetext){
			updatetext = 0;
			
			/*
			 * 126  -> Start Print 2
			 * 31   -> End Print 2
			 * 
			*/
			
			// Write the text
			for(i = 0; i < 11; i++){
				VK_TextX = 1;
				VK_TextY = 1+i;
				VK_PrintTXT(STORYTXT_text[i+TextScroll]);
			}
		}

		VK_WaitVRB();
		
	}

	VK_FadeOut();
	
	// Render the rest of the things
	vk_engine_demo = VK_DEMO_MAINMENU;
};


void VK_DrawFinalText(){
	int i,e;
	unsigned int bmptileoff = 0;
	uint16_t updatetext = 1;
	int TextScroll = 0;
	
	// Position the screen
	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG1HOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = 0x0;
			
	
	VK_DrawBox(0,0,29,19);
	VK_DrawBox(0,17,29,2);

	// Write instructions
	VK_TextX = 1;
	VK_TextY = 18;
	VK_Print2("  B To Exit / \17 \23 to Read   ");
	
	while(1){

		VK_UpdateInput();

		if(VK_CheckButton(GBA_BUTTON_UP)){
			TextScroll -= 1;
			if(TextScroll<0){
				TextScroll = 0;
			}else{
				updatetext = 1;
			}
		}
		
		if(VK_CheckButton(GBA_BUTTON_DOWN)){
			TextScroll += 1;
			if(TextScroll>=ENDTEXT_length-16){
				TextScroll = ENDTEXT_length-17;
			}else{
				updatetext = 1;
			}
		}

		// Break on key input
		if(VK_ButtonUp()==GBA_BUTTON_B){
			break;
		}
		
		// Render story text
		if(updatetext){
			updatetext = 0;
			
			/*
			 * 126  -> Start Print 2
			 * 31   -> End Print 2
			 * 
			*/
			
			// Write the text
			for(i = 0; i < 16; i++){
				VK_TextX = 1;
				VK_TextY = 1+i;
				VK_PrintTXT(ENDTEXT_text[i+TextScroll]);
			}
		}

		VK_WaitVRB();
		
	}
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

	// Set a custom clear tile
	VK_CLEAR_TILE = 0;
	
	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG1HOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = 0x0;
	
	vk_engine_demo = VK_DEMO_MAINMENU;
	
};


uint16_t VK_LoadMenu(){
	int i;

	VK_GBA_BG_MAPB[0] = 0;
	
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
			
			if(VK_CheckSave(cursor_x>>3)){
				if(VK_LoadGame(cursor_x>>3)){

					VK_GBA_BG_MAPB[0] = 0;
					
					// Spawn a box
					VK_SpawnBox(14,9,29,3);
					
					// Write the text on the dialog box
					VK_TextX = 4;
					VK_TextY = 9;
					VK_Print("Error loading");
					
					VK_UpdateInput();

					// Wait for button press
					while(!VK_ButtonUp()){
						VK_UpdateInput();
						
						cursor_ani_tick++;
						if(cursor_ani_tick>0x2){
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
				}
				return 1;
			}else{

				VK_GBA_BG_MAPB[0] = 0;
				
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
					if(cursor_ani_tick>0x2){
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
				
				VK_GBA_BG_MAPB[0] = 0;
				
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

uint16_t VK_AreYouSure(){
	int i;

	VK_GBA_BG_MAPB[0] = 0;
	
	VK_SpawnBox(14,10,16,4);
	
	// Write the text on the dialog box
	VK_TextX = 7;
	VK_TextY = 10;
	VK_Print("Are you sure?:");

	uint16_t cursor_animation = 0;
	uint16_t cursor_ani_tick = 0;
	
	while(1){
		
		VK_UpdateInput();
		
		// Do Menu Logic
		if(VK_ButtonUp()==GBA_BUTTON_A){
			return 1;
		}
		if(VK_ButtonUp()==GBA_BUTTON_B){
			return 0;
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
		*(volatile uint16_t*)GBA_REG_BG1HOFS = -0xA8;
		*(volatile uint16_t*)GBA_REG_BG1VOFS = -0x50;

		// Draw the cursor
		VK_GBA_BG_MAPB[0] = 0x2C9+cursor_animation;
		
		VK_WaitVRB();
	}
	
	return 0;	
};


void VK_DrawOptions(){
	
	VK_GBA_BG_MAPB[0] = 0;

	VK_SpawnBox(15,10,26,8);
	
	// Write the text on the dialog box
	VK_TextX = 8;
	VK_TextY = 7;
	VK_Print("Sound  On  Off");
	VK_TextY += 1;
	VK_Print("Music  On  Off");
	VK_TextY += 1;
	VK_Print("Palette  N  GB  GR");
	VK_TextY += 1;
	VK_Print("Reset ROM");
	VK_TextY += 1;
	VK_Print("Return to Main Menu");
	VK_TextY += 2;
	VK_TextX -= 3;
	VK_Print("Use The D-Pad");
	
};


void VK_OptionsMenu(){

	// Draw the menu
	VK_DrawOptions();
	
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
				// Toggle Sound
				if(vk_engine_gstate.sound_enabled ){
					vk_engine_gstate.sound_enabled = 0;
				}
				else{
					vk_engine_gstate.sound_enabled = 1;
				}	
				VK_SoundEnabled = vk_engine_gstate.sound_enabled;
				break;
				case 8:
				// Toggle Music
				if(vk_engine_gstate.music_enabled){
					vk_engine_gstate.music_enabled = 0;
				}else {
					vk_engine_gstate.music_enabled = 1;
				}
				VK_MusicEnabled = vk_engine_gstate.music_enabled;
				break;
				case 16:
				// Toggle Palette
				vk_engine_gstate.gba_palette += 1;
				if(vk_engine_gstate.gba_palette>2){
					vk_engine_gstate.gba_palette = 0;
				}
				VK_SetPalette(0);
				break;
				case 24:
				// Reset the ROM
				if(VK_AreYouSure()){
					VK_FormatROM();
				}
				return;
				break;

				default:
				// Quit

				// Save what was changed
				VK_SaveOptions();
				return;

				break;
			}
		}
		
		// Move the cursor
		if(cursor_y == cursor_to_y){

			if(VK_ButtonUp()==GBA_BUTTON_UP){
				
				if(cursor_y==0){
					cursor_y = (4<<3);
					cursor_to_y = (4<<3);
				}else{
					cursor_to_y = cursor_y - 8;
				}
			}
			if(VK_ButtonUp()==GBA_BUTTON_DOWN){
				if(cursor_y==(4<<3)){
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

		if(vk_engine_gstate.sound_enabled){
			VK_GBA_BG_MAPA[(7<<5)+14] = 0x2C9+cursor_animation;
			VK_GBA_BG_MAPA[(7<<5)+18] = 0x2E0;
		}else{
			VK_GBA_BG_MAPA[(7<<5)+14] = 0x2E0;
			VK_GBA_BG_MAPA[(7<<5)+18] = 0x2C9+cursor_animation;
		}

		if(vk_engine_gstate.music_enabled){
			VK_GBA_BG_MAPA[(8<<5)+14] = 0x2C9+cursor_animation;
			VK_GBA_BG_MAPA[(8<<5)+18] = 0x2E0;
		}else{
			VK_GBA_BG_MAPA[(8<<5)+14] = 0x2E0;
			VK_GBA_BG_MAPA[(8<<5)+18] = 0x2C9+cursor_animation;
		}
		
		if(vk_engine_gstate.gba_palette==0){
			VK_GBA_BG_MAPA[(9<<5)+16] = 0x2C9+cursor_animation;
			VK_GBA_BG_MAPA[(9<<5)+19] = 0x2E0;
			VK_GBA_BG_MAPA[(9<<5)+23] = 0x2E0;
		}
		if(vk_engine_gstate.gba_palette==1){
			VK_GBA_BG_MAPA[(9<<5)+16] = 0x2E0;
			VK_GBA_BG_MAPA[(9<<5)+19] = 0x2C9+cursor_animation;
			VK_GBA_BG_MAPA[(9<<5)+23] = 0x2E0;
		}
		if(vk_engine_gstate.gba_palette==2){
			VK_GBA_BG_MAPA[(9<<5)+16] = 0x2E0;
			VK_GBA_BG_MAPA[(9<<5)+19] = 0x2E0;
			VK_GBA_BG_MAPA[(9<<5)+23] = 0x2C9+cursor_animation;
		}

		// Draw the cursor
		VK_GBA_BG_MAPB[0] = 0x2C9+cursor_animation;
		
		*(volatile uint16_t*)GBA_REG_BG1HOFS = -0x30;
		*(volatile uint16_t*)GBA_REG_BG1VOFS = -0x38-cursor_y;
		
		VK_WaitVRB();
	}
};



void VK_DrawMainMenu(){

	VK_SpawnBox(15,10,20,9);
	
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
	VK_Print("Options");
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
				VK_NewGame();
				vk_engine_demo = VK_DEMO_GAME;
				return;
				case 8:
				// Load game dialog
				if(VK_LoadMenu()){
					// Start a game
					VK_FadeOut();
					VK_InitGame();
					vk_engine_gstate.has_loaded = 1;
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
				VK_OptionsMenu();
				// Redraw the main menu
				VK_DrawTitleScreen();
				VK_DrawMainMenu();
				cursor_y = 0;
				cursor_to_y = 0;
				cursor_slide_tick = 0;
				break;
				case 40:
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
					cursor_y = (5<<3);
					cursor_to_y = (5<<3);
				}else{
					cursor_to_y = cursor_y - 8;
				}
			}
			if(VK_ButtonUp()==GBA_BUTTON_DOWN){
				if(cursor_y==(5<<3)){
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


void VK_StatusBar(){
	// Reposition the screen
	*(volatile uint16_t*)GBA_REG_BG0HOFS = (vk_map_offsetx>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = (vk_map_offsety>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG1HOFS = (vk_map_offsetx>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = (vk_map_offsety>>3)<<3;
	
	VK_ClearTopLayer();
	
	VK_SpawnBox(14+(vk_map_offsetx>>3),10+(vk_map_offsety>>3),29,14);
	
	// Write the text on the status bar
	VK_TextX = 1+(vk_map_offsetx>>3);
	VK_TextY = 4+(vk_map_offsety>>3);
	VK_Print2("    SCORE     EXTRA KEEN AT ");
	VK_TextY += 1;
	char *strptr = VK_Iota(vk_engine_gstate.score);
	VK_TextX += 9 - strlen(strptr);
	VK_Print(strptr);
	VK_TextX -= 9 - strlen(strptr);
	
	strptr = VK_Iota(vk_engine_gstate.next_1up);
	VK_TextX += 0x19 - strlen(strptr);
	VK_Print(strptr);
	VK_TextX -= 0x19 - strlen(strptr);

	VK_TextY += 1;
	VK_Print2("    KEENS       SHIP PARTS  ");
	VK_TextY += 4;
	VK_Print2(" RAYGUN   POGO    KEYCARDS  ");
	VK_TextY += 4;
	VK_Print2(" CHARGE ");
	VK_TextY += 1;
	strptr = VK_Iota(vk_engine_gstate.ammo);
	VK_TextX += 0x5 - strlen(strptr);
	VK_Print(strptr);
	VK_TextX -= 0x5 - strlen(strptr);
	VK_TextY += 1;
	VK_Print2("    PLEASE PRESS A BUTTON   ");
	// Line 1
	VK_TextX = 13+(vk_map_offsetx>>3);
	VK_TextY = 5+(vk_map_offsety>>3);
	VK_Print2(" ");
	// Line 2
	VK_TextX = 15+(vk_map_offsetx>>3);
	VK_TextY = 7+(vk_map_offsety>>3);
	VK_Print2(" ");
	VK_TextY += 1;
	VK_Print2(" ");
	VK_TextY += 1;
	VK_Print2(" ");
	// Line 3
	VK_TextX = 9+(vk_map_offsetx>>3);
	VK_TextY = 10+(vk_map_offsety>>3);
	VK_Print2(" ");
	VK_TextY += 1;
	VK_Print2(" ");
	VK_TextY += 1;
	VK_Print2(" ");
	VK_TextY += 1;
	VK_Print2(" ");
	VK_TextY += 1;
	VK_Print2(" ");
	VK_TextY += 1;
	VK_Print2(" ");
	// Line 4
	VK_TextX = 16+(vk_map_offsetx>>3);
	VK_TextY = 10+(vk_map_offsety>>3);
	VK_Print2(" ");
	VK_TextY += 1;
	VK_Print2(" ");
	VK_TextY += 1;
	VK_Print2(" ");
	VK_TextY += 1;
	VK_Print2(" ");
	VK_TextY += 1;
	VK_Print2(" ");
	VK_TextY += 1;
	VK_Print2(" ");

	// Draw ship parts
	if(vk_engine_gstate.gotJoystick){
		VK_DrawTile(0x11+(vk_map_offsetx>>3),8+(vk_map_offsety>>3),(vk_special_items+10));
	}
	if(vk_engine_gstate.gotBattery){
		VK_DrawTile(0x13+(vk_map_offsetx>>3),8+(vk_map_offsety>>3),(vk_special_items+11));
	}
	if(vk_engine_gstate.gotVacuum){
		VK_DrawTile(0x15+(vk_map_offsetx>>3),8+(vk_map_offsety>>3),(vk_special_items+12));
	}
	if(vk_engine_gstate.gotWhiskey){
		VK_DrawTile(0x17+(vk_map_offsetx>>3),8+(vk_map_offsety>>3),(vk_special_items+13));
	}
	
	// Draw keycards
	if(vk_engine_gstate.gotKeycardY){
		VK_DrawTile(0x13+(vk_map_offsetx>>3),0xB+(vk_map_offsety>>3),(vk_special_items+2));
	}
	if(vk_engine_gstate.gotKeycardR){
		VK_DrawTile(0x18+(vk_map_offsetx>>3),0xB+(vk_map_offsety>>3),(vk_special_items+3));
	}
	if(vk_engine_gstate.gotKeycardG){
		VK_DrawTile(0x13+(vk_map_offsetx>>3),0xE +(vk_map_offsety>>3),(vk_special_items+4));
	}
	if(vk_engine_gstate.gotKeycardB){
		VK_DrawTile(0x18+(vk_map_offsetx>>3),0xE +(vk_map_offsety>>3),(vk_special_items+5));
	}

	// Draw raygun
//	if(vk_engine_gstate.gotRaygun){
	VK_DrawTile(0x4+(vk_map_offsetx>>3),0xB+(vk_map_offsety>>3),(vk_special_items));
//	}

	// Draw pogo
	if(vk_engine_gstate.gotPogo){
		VK_DrawTile(0xC+(vk_map_offsetx>>3),0xC+(vk_map_offsety>>3),(vk_special_items+1));
	}

	// Draw sprites
	vk_sprite *keens[8];
	int i;
	for(i=0;(i<6)&&(i<vk_engine_gstate.numLives);i++){
		keens[i] = VK_CreateSprite(255);
		keens[i]->x = 16+(i*16);
		keens[i]->y = 56;
		keens[i]->s.spr_gfx_ani = 0;
		VK_SetSpriteGraphics(keens[i]);
		// Render the sprite
		VK_RenderSprite(keens[i]);
	}

	
	while(1){
		VK_UpdateInput();
		uint16_t button = VK_ButtonUp();
		
		if(button == (GBA_BUTTON_START) ||
		button == (GBA_BUTTON_SELECT) ||
		button == (GBA_BUTTON_A) ||
		button == (GBA_BUTTON_B) ){
			break;
		}
		VK_WaitVRB();
	}

	// Remove it
	for(i=0;(i<6)&&(i<vk_engine_gstate.numLives);i++){
		VK_RemoveSprite(keens[i]);
	}
	
	VK_ForceLevelUpdate();
	VK_UpdateInput();
	
};

uint16_t VK_QuitDialog(){
	// Reposition the screen
	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0;//(vk_map_offsetx>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0;//(vk_map_offsety>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG1HOFS = 0;//(vk_map_offsetx>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = 0;//(vk_map_offsety>>3)<<3;

	uint16_t cursor_animation = 0;
	uint16_t cursor_ani_tick = 0;
	uint16_t outputv = 0;
	
	VK_ClearTopLayer();

	VK_SpawnBox(14,9,21,2);
	
	// Write the text on the dialog box
	VK_TextX = 6;
	VK_TextY = 9;
	VK_Print("Quit to Title:");
	
	
	while(1){
		VK_UpdateInput();
		
		uint16_t button = VK_ButtonUp();

		if(button==(GBA_BUTTON_START)){
			outputv = 1;
			break;
		}
		if(button==(GBA_BUTTON_A)){
			outputv = 1;
			break;
		}

		if(button==(GBA_BUTTON_SELECT)){
			outputv = 0;
			break;
		}
		if(button==(GBA_BUTTON_B)){
			outputv = 0;
			break;
		}
		
		
		cursor_ani_tick++;
		if(cursor_ani_tick>0x2){
			cursor_ani_tick = 0;
			cursor_animation += 1;
			if(cursor_animation>5){
				cursor_animation = 0;
			}
		}

		// Draw the cursor
		VK_GBA_BG_MAPB[308] = 0x2C9+cursor_animation;
		
		VK_WaitVRB();
	}
	
	VK_GBA_BG_MAPB[308] = VK_CLEAR_TILE;

	VK_ForceLevelUpdate();
	
	return outputv;
};



void VK_MissingParts(){
	// Reposition the screen
	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0;//(vk_map_offsetx>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0;//(vk_map_offsety>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG1HOFS = 0;//(vk_map_offsetx>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = 0;//(vk_map_offsety>>3)<<3;

	uint16_t cursor_animation = 0;
	uint16_t cursor_ani_tick = 0;
	
	VK_ClearTopLayer();

	VK_SpawnBox(14,9,21,9);
	
	// Write the text on the dialog box
	VK_TextX = 5;
	VK_TextY = 6;
	VK_Print("Your ship is missing");
	VK_TextY = 7;
	VK_Print("these parts:");
	VK_TextX = 9;
	VK_TextY = 12;
	VK_Print("GO GET THEM!");
	VK_TextX = 8;
	VK_TextY = 13;
	VK_Print("Press button:");


	// Draw ship parts
	if(vk_engine_gstate.gotJoystick){
		VK_DrawTile(0x8,9,(vk_special_items+10));
	}else{
		VK_DrawTile(0x8,9,(vk_special_items+6));
	}
	if(vk_engine_gstate.gotBattery){
		VK_DrawTile(0xC,9,(vk_special_items+11));
	}else{
		VK_DrawTile(0xC,9,(vk_special_items+7));
	}
	if(vk_engine_gstate.gotVacuum){
		VK_DrawTile(0x10,9,(vk_special_items+12));
	}else{
		VK_DrawTile(0x10,9,(vk_special_items+8));
	}
	if(vk_engine_gstate.gotWhiskey){
		VK_DrawTile(0x14,9,(vk_special_items+13));
	}else{
		VK_DrawTile(0x14,9,(vk_special_items+9));
	}
	
	VK_UpdateInput();

	while(VK_ButtonUp()){
		VK_UpdateInput();
	}
	
	VK_UpdateInput();
	while(!VK_ButtonUp()){
		VK_UpdateInput();
		
		
		cursor_ani_tick++;
		if(cursor_ani_tick>0x2){
			cursor_ani_tick = 0;
			cursor_animation += 1;
			if(cursor_animation>5){
				cursor_animation = 0;
			}
		}

		// Draw the cursor
		VK_GBA_BG_MAPB[437] = 0x2C9+cursor_animation;
		
		VK_WaitVRB();
	}
	
	
	VK_GBA_BG_MAPB[437] = VK_CLEAR_TILE;

	VK_ForceLevelUpdate();
	
	
};


void VK_DisplayMessage(int index){
	int i;
	// Reposition the screen
	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0;//(vk_map_offsetx>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0;//(vk_map_offsety>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG1HOFS = 0;//(vk_map_offsetx>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = 0;//(vk_map_offsety>>3)<<3;

	uint16_t cursor_animation = 0;
	uint16_t cursor_ani_tick = 0;
	uint16_t outputv = 0;
	
	VK_ClearTopLayer();

	VK_SpawnBox(14,9,21,11);
	
	// Write the text on the dialog box
	VK_TextX = 5;
	for(i = 0; i < VK_STATUE_LENGTHS[index]; i++){
		VK_TextY = 5+i;
		VK_Print(VK_STATUE_MESSAGES[index][i]);
	}
	VK_TextY = 14;
	VK_Print("Press A Button:");
	
	VK_UpdateInput();

	while(VK_ButtonUp()){
		VK_UpdateInput();
	}
	
	while(1){
		VK_UpdateInput();
		if(VK_ButtonUp()==GBA_BUTTON_A){
			break;
		}
		
		cursor_ani_tick++;
		if(cursor_ani_tick>0x2){
			cursor_ani_tick = 0;
			cursor_animation += 1;
			if(cursor_animation>5){
				cursor_animation = 0;
			}
		}

		// Draw the cursor
		VK_GBA_BG_MAPB[468] = 0x2C9+cursor_animation;
		
		VK_WaitVRB();
	}
	
	VK_GBA_BG_MAPB[468] = VK_CLEAR_TILE;

	VK_ForceLevelUpdate();
	
	
};



void VK_KeensLeft(){
	uint16_t waitticks = 0xC0;

	// Reposition the screen
	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0;//(vk_map_offsetx>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0;//(vk_map_offsety>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG1HOFS = 0;//(vk_map_offsetx>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = 0;//(vk_map_offsety>>3)<<3;

	VK_ClearTopLayer();
	
	// Draw Keens Left box
	VK_DrawBox(5,8,17,5);

	VK_TextX = 9;
	VK_TextY = 9;
	VK_Print("Keens Left");

	VK_PlaySound(VKS_KEENSLEFT);
	
	// Draw sprites
	vk_sprite *keens[8];

	int i;
	for(i=0;(i<8)&&(i<vk_engine_gstate.numLives);i++){
		keens[i] = VK_CreateSprite(254);
		keens[i]->x = 50+(i*16);//-(vk_map_offsetx>>3);
		keens[i]->y = 84;//-(vk_map_offsety>>3);
		keens[i]->s.spr_gfx_ani = 4;
		VK_SetSpriteGraphics(keens[i]);
		// Render the sprite
		VK_RenderSprite(keens[i]);
	}
	

	while(waitticks){
		VK_UpdateInput();
		uint16_t button = VK_ButtonUp();
		
		if(button == (GBA_BUTTON_START) ||
			button == (GBA_BUTTON_SELECT) ||
			button == (GBA_BUTTON_A) ||
			button == (GBA_BUTTON_B) ){
			waitticks = 1;
		}
		waitticks -= 1;
		VK_WaitVRB();
	}

	// Remove it
	for(i=0;(i<8)&&(i<vk_engine_gstate.numLives);i++){

		VK_RemoveSprite(keens[i]);
	}

	VK_ForceLevelUpdate();

};


void VK_DrawInfoOptions(){
	// Reposition the screen
	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0;//(vk_map_offsetx>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0;//(vk_map_offsety>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG1HOFS = 0;//(vk_map_offsetx>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = 0;//(vk_map_offsety>>3)<<3;
	
	VK_ClearTopLayer();

	VK_SpawnBox(15,10,26,8);
	
	// Write the text on the dialog box
	VK_TextX = 8;
	VK_TextY = 7;
	VK_Print("Save Game");
	VK_TextY += 1;
	VK_Print("Sound  On  Off");
	VK_TextY += 1;
	VK_Print("Music  On  Off");
	VK_TextY += 1;
	VK_Print("Palette  N  GB  GR");
	VK_TextY += 1;
	VK_Print("Return to Game");
	VK_TextY += 2;
	VK_TextX -= 3;
	VK_Print("Use The D-Pad");
	
};


uint16_t VK_SaveDialog(){
	// Reposition the screen
	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0;//(vk_map_offsetx>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0;//(vk_map_offsety>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG1HOFS = 0;//(vk_map_offsetx>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = 0;//(vk_map_offsety>>3)<<3;

	VK_GBA_BG_MAPB[0] = VK_CLEAR_TILE;
	
	VK_SpawnBox(14,9,29,3);
	
	// Write the text on the dialog box
	VK_TextX = 6;
	VK_TextY = 9;
	VK_Print("Save Which Game?");
	VK_TextX = 2;
	VK_TextY = 10;
	VK_Print("1: 2: 3: 4: 5: 6: 7: 8: 9: ");

	uint16_t cursor_animation = 0;
	uint16_t cursor_ani_tick = 0;
	uint16_t cursor_x = 0;
	
	// Store current position
	vk_engine_gstate.posX = vk_keen_obj->pos_x;
	vk_engine_gstate.posY = vk_keen_obj->pos_y;
	vk_engine_gstate.viewportX = vk_viewport_x;
	vk_engine_gstate.viewportY = vk_viewport_y;

	
	while(1){
		
		VK_UpdateInput();
		
		// Do Menu Logic
		if(VK_ButtonUp()==GBA_BUTTON_A){
			
			if(VK_CheckSave(cursor_x>>3)){
				VK_GBA_BG_MAPB[0] = VK_CLEAR_TILE;
				
				// Spawn a box
				VK_SpawnBox(14,9,29,3);
				
				// Write the text on the dialog box
				VK_TextX = 2;
				VK_TextY = 9;
				VK_Print("That game already exitsts!");
				VK_TextY = 10;
				VK_Print("Overwrite it?:");
				
				VK_UpdateInput();

				// Wait for button press
				while(1){
					VK_UpdateInput();
					
					if(VK_ButtonUp()==GBA_BUTTON_A){
						VK_SaveGame(cursor_x>>3);

						VK_GBA_BG_MAPB[0] = VK_CLEAR_TILE;
						
						// Spawn a box
						VK_SpawnBox(15,10,26,4);
						
						// Write the text on the dialog box
						VK_TextX = 3;
						VK_TextY = 9;
						VK_Print("You can continue this");
						VK_TextY = 10;
						VK_Print("game from the Main Menu");
						VK_TextY = 11;
						VK_Print("next time you play:");
						
						VK_UpdateInput();

						while(!VK_ButtonUp()){
							VK_UpdateInput();

							cursor_ani_tick++;
							if(cursor_ani_tick>0x2){
								cursor_ani_tick = 0;
								cursor_animation += 1;
								if(cursor_animation>5){
									cursor_animation = 0;
								}
							}

							*(volatile uint16_t*)GBA_REG_BG1HOFS = -0xB0;
							*(volatile uint16_t*)GBA_REG_BG1VOFS = -0x58;

							// Draw the cursor
							VK_GBA_BG_MAPB[0] = 0x2C9+cursor_animation;
							
							VK_WaitVRB();


						}
		
						goto ExitSaveFun;
					}
					if(VK_ButtonUp()==GBA_BUTTON_B){
						break;
					}
					
					cursor_ani_tick++;
					if(cursor_ani_tick>0x2){
						cursor_ani_tick = 0;
						cursor_animation += 1;
						if(cursor_animation>5){
							cursor_animation = 0;
						}
					}

					*(volatile uint16_t*)GBA_REG_BG1HOFS = -0x80;
					*(volatile uint16_t*)GBA_REG_BG1VOFS = -0x50;

					// Draw the cursor
					VK_GBA_BG_MAPB[0] = 0x2C9+cursor_animation;
					
					VK_WaitVRB();

				}
				
				VK_GBA_BG_MAPB[0] = VK_CLEAR_TILE;
				
				// Redraw the main one
				VK_SpawnBox(14,9,29,3);
				
				// Write the text on the dialog box
				VK_TextX = 6;
				VK_TextY = 9;
				VK_Print("Save Which Game?");
				VK_TextX = 2;
				VK_TextY = 10;
				VK_Print("1: 2: 3: 4: 5: 6: 7: 8: 9: ");
			}else{
				VK_SaveGame(cursor_x>>3);

				VK_GBA_BG_MAPB[0] = VK_CLEAR_TILE;
				
				// Spawn a box
				VK_SpawnBox(15,10,26,4);
				
				// Write the text on the dialog box
				VK_TextX = 3;
				VK_TextY = 9;
				VK_Print("You can continue this");
				VK_TextY = 10;
				VK_Print("game from the Main Menu");
				VK_TextY = 11;
				VK_Print("next time you play:");
				
				VK_UpdateInput();

				while(!VK_ButtonUp()){
					VK_UpdateInput();

					cursor_ani_tick++;
					if(cursor_ani_tick>0x2){
						cursor_ani_tick = 0;
						cursor_animation += 1;
						if(cursor_animation>5){
							cursor_animation = 0;
						}
					}

					*(volatile uint16_t*)GBA_REG_BG1HOFS = -0xB0;
					*(volatile uint16_t*)GBA_REG_BG1VOFS = -0x58;

					// Draw the cursor
					VK_GBA_BG_MAPB[0] = 0x2C9+cursor_animation;
					
					VK_WaitVRB();


				}

				goto ExitSaveFun;
				
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
ExitSaveFun:

	VK_GBA_BG_MAPB[0] = VK_CLEAR_TILE;

	VK_ForceLevelUpdate();
	
};


uint16_t VK_SaveDialog2(){
	// Reposition the screen
	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0;//(vk_map_offsetx>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0;//(vk_map_offsety>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG1HOFS = 0;//(vk_map_offsetx>>3)<<3;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = 0;//(vk_map_offsety>>3)<<3;

	VK_GBA_BG_MAPB[0] = VK_CLEAR_TILE;
	
	VK_SpawnBox(15,10,24,4);
	
	// Write the text on the dialog box
	VK_TextX = 5;
	VK_TextY = 9;
	VK_Print("You can SAVE the game");
	VK_TextX = 5;
	VK_TextY = 10;
	VK_Print("ONLY on the World Map!");
	VK_TextX = 6;
	VK_TextY = 11;
	VK_Print("Press Any Button:");

	uint16_t cursor_animation = 0;
	uint16_t cursor_ani_tick = 0;
	
	VK_UpdateInput();
	
	while(!VK_ButtonUp()){
		
		VK_UpdateInput();
		
		cursor_ani_tick++;
		if(cursor_ani_tick>0x2){
			cursor_ani_tick = 0;
			cursor_animation += 1;
			if(cursor_animation>5){
				cursor_animation = 0;
			}
		}

		// Position the cursor
		*(volatile uint16_t*)GBA_REG_BG1HOFS = -0xB8;
		*(volatile uint16_t*)GBA_REG_BG1VOFS = -0x58;

		// Draw the cursor
		VK_GBA_BG_MAPB[0] = 0x2C9+cursor_animation;
		
		VK_WaitVRB();
	}
	VK_GBA_BG_MAPB[0] = VK_CLEAR_TILE;

	VK_ForceLevelUpdate();
	
};

void VK_InfoOptions(){
	uint16_t inmenu = 1;
	// Draw the menu
	VK_DrawInfoOptions();
	int i;
	
	uint16_t cursor_animation = 0;
	uint16_t cursor_ani_tick = 0;
	uint16_t cursor_y = 0;
	uint16_t cursor_to_y = 0;
	uint16_t cursor_slide_tick = 0;

	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0x0;
	
	while(inmenu){
		
		VK_UpdateInput();
		
		// Do Menu Logic
		if(VK_ButtonUp()==GBA_BUTTON_A){
			switch(cursor_y){
				case 0:
				// Save the game
				if(vk_engine_gstate.in_game==80){
					VK_SaveDialog();
				}else{
					VK_SaveDialog2();
				}
				
				for(i = 0; i < 32 *32; i++){
					VK_GBA_BG_MAPB[i] = VK_CLEAR_TILE;
				}

				// Redraw the menu
				VK_DrawInfoOptions();
				break;
				case 8:
				// Toggle Sound
				if(vk_engine_gstate.sound_enabled ){
					vk_engine_gstate.sound_enabled = 0;
				}
				else{
					vk_engine_gstate.sound_enabled = 1;
				}	
				VK_SoundEnabled = vk_engine_gstate.sound_enabled;
				break;
				case 16:
				// Toggle Music
				if(vk_engine_gstate.music_enabled){
					vk_engine_gstate.music_enabled = 0;
				}else {
					vk_engine_gstate.music_enabled = 1;
				}
				VK_MusicEnabled = vk_engine_gstate.music_enabled;
				break;
				case 24:
				// Toggle Palette
				vk_engine_gstate.gba_palette += 1;
				if(vk_engine_gstate.gba_palette>2){
					vk_engine_gstate.gba_palette = 0;
				}
				VK_SetPalette(0);
				break;

				default:
				// Quit
				inmenu = 0;
				break;
			}
		}
		
		
		// Move the cursor
		if(cursor_y == cursor_to_y){

			if(VK_ButtonUp()==GBA_BUTTON_UP){
				
				if(cursor_y==0){
					cursor_y = (4<<3);
					cursor_to_y = (4<<3);
				}else{
					cursor_to_y = cursor_y - 8;
				}
			}
			if(VK_ButtonUp()==GBA_BUTTON_DOWN){
				if(cursor_y==(4<<3)){
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


		
		if(vk_engine_gstate.sound_enabled){
			VK_GBA_BG_MAPA[(8<<5)+14] = 0x2C9+cursor_animation;
			VK_GBA_BG_MAPA[(8<<5)+18] = 0x2E0;
		}else{
			VK_GBA_BG_MAPA[(8<<5)+14] = 0x2E0;
			VK_GBA_BG_MAPA[(8<<5)+18] = 0x2C9+cursor_animation;
		}

		if(vk_engine_gstate.music_enabled){
			VK_GBA_BG_MAPA[(9<<5)+14] = 0x2C9+cursor_animation;
			VK_GBA_BG_MAPA[(9<<5)+18] = 0x2E0;
		}else{
			VK_GBA_BG_MAPA[(9<<5)+14] = 0x2E0;
			VK_GBA_BG_MAPA[(9<<5)+18] = 0x2C9+cursor_animation;
		}
		
		if(vk_engine_gstate.gba_palette==0){
			VK_GBA_BG_MAPA[(10<<5)+16] = 0x2C9+cursor_animation;
			VK_GBA_BG_MAPA[(10<<5)+19] = 0x2E0;
			VK_GBA_BG_MAPA[(10<<5)+23] = 0x2E0;
		}
		if(vk_engine_gstate.gba_palette==1){
			VK_GBA_BG_MAPA[(10<<5)+16] = 0x2E0;
			VK_GBA_BG_MAPA[(10<<5)+19] = 0x2C9+cursor_animation;
			VK_GBA_BG_MAPA[(10<<5)+23] = 0x2E0;
		}
		if(vk_engine_gstate.gba_palette==2){
			VK_GBA_BG_MAPA[(10<<5)+16] = 0x2E0;
			VK_GBA_BG_MAPA[(10<<5)+19] = 0x2E0;
			VK_GBA_BG_MAPA[(10<<5)+23] = 0x2C9+cursor_animation;
		}

		// Draw the cursor
		VK_GBA_BG_MAPB[0] = 0x2C9+cursor_animation;
		
		*(volatile uint16_t*)GBA_REG_BG1HOFS = -0x30;
		*(volatile uint16_t*)GBA_REG_BG1VOFS = -0x38-cursor_y;
		
		VK_WaitVRB();
	}
	
	for(i = 0; i < 32 *32; i++){
		VK_GBA_BG_MAPB[i] = VK_CLEAR_TILE;
	}

	VK_ForceLevelUpdate();
	
	// Save what was changed
	VK_SaveOptions();
};


void VK_DrawEndText(uint8_t text){
	int i;
	// Position the screen
	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG1HOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = 0x0;
			
	VK_DrawBox(0,11,29,VK_END_TXT_LENGTHS[text]+1);
	VK_TextX = 1;
	for(i = 0; i < VK_END_TXT_LENGTHS[text]; i++){
		VK_TextY = 12+i;
		VK_Type(VK_END_TEXT[text][i]);
	}
};

void VK_DrawEndText2(uint8_t text){
	int i;
	// Position the screen
	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG1HOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = 0x0;
			
	VK_DrawBox(0,12,24,VK_END_TXT_LENGTHS[text]+1);
	VK_TextX = 1;
	for(i = 0; i < VK_END_TXT_LENGTHS[text]; i++){
		VK_TextY = 13+i;
		VK_Type(VK_END_TEXT[text][i]);
	}
};

void VK_DrawEndText3(uint8_t text){
	int i;
	// Position the screen
	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG1HOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = 0x0;
			
	VK_DrawBox(0,12,24,VK_END_TXT_LENGTHS[text]+1);
	VK_TextX = 1;
	VK_TextY = 13;
	VK_Type(VK_END_TEXT[text][i]);
	// Just print the rest
	for(i = 1; i < VK_END_TXT_LENGTHS[text]; i++){
		VK_TextY = 13+i;
		VK_Print(VK_END_TEXT[text][i]);
	}
};

void VK_DrawEndTBC(){
	int i;
	// Position the screen
	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG1HOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = 0x0;
			
	VK_DrawBox(2,3,25,2);
	VK_TextX = 3;
	VK_TextY = 4;
	VK_Type("TO BE CONTINUED...");
};

void VK_CopyEndScreenGfx(){
	unsigned int bmptileoff = 0;
	
	// Load the graphics
	GBA_DMA_MemSet32(VK_GBA_BG_Tiles,0,16);
	bmptileoff = 64;
	GBA_DMA_Copy32((VK_GBA_BG_Tiles+bmptileoff),ENDSCREEN_data,ENDSCREEN_size>>2);
	bmptileoff += ENDSCREEN_size;
	GBA_DMA_Copy32((VK_GBA_BG_Tiles+bmptileoff),WINDOW_ON_data,WINDOW_ON_size>>2);
	bmptileoff += WINDOW_ON_size;
	GBA_DMA_Copy32((VK_GBA_BG_Tiles+bmptileoff),WINDOW_OFF_data,WINDOW_OFF_size>>2);
	bmptileoff += WINDOW_OFF_size;

};

void VK_DrawEndScreen(uint8_t lights_on){
	int i,e;
	// Fix position
	*(volatile uint16_t*)GBA_REG_BG0HOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG0VOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG1HOFS = 0x0;
	*(volatile uint16_t*)GBA_REG_BG1VOFS = 0x0;
	if(lights_on==0){
		// Draw the scene
		VK_WaitVRB();
		for(i=0;i<20;i++){
			for(e=0;e<30;e++){
				VK_GBA_BG_MAPA[(i<<5)+e] = (i*30)+e+1;
				VK_GBA_BG_MAPB[(i<<5)+e] = 0;
			}
		}
	}
	if(lights_on==1){
		uint16_t dy = 0, dx = 0;
		// Draw light
		VK_WaitVRB();
		for(i=0;i<WINDOW_ON_height>>3;i++){
			for(e=0;e<WINDOW_ON_width>>3;e++){
				dy = i + 0;
				dx = e + 8;
				VK_GBA_BG_MAPA[(dy<<5)+dx] = (i*(WINDOW_ON_width>>3))+e+1+(20*30);
			}
		}
	}
	if(lights_on==2){
		uint16_t dy = 0, dx = 0;
		// Draw light
		VK_WaitVRB();
		for(i=0;i<WINDOW_OFF_height>>3;i++){
			for(e=0;e<WINDOW_OFF_width>>3;e++){
				dy = i + 0;
				dx = e + 8;
				VK_GBA_BG_MAPA[(dy<<5)+dx] = (i*(WINDOW_OFF_width>>3))+e+1+(20*30)+(WINDOW_ON_size>>6);
			}
		}
	}	
};
