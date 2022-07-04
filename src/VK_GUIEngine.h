
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

#ifndef __VK_GUI_ENGINE__
#define __VK_GUI_ENGINE__


#define VK_DATA_BLOCK_SIZE 1020

typedef struct VK_DATA_BLOCK {
	uint8_t data[VK_DATA_BLOCK_SIZE]; // 1K of data
	uint8_t errorcode; // error code, if any
}VK_DATA_BLOCK;



// Write a block of data to the game pak
// Each block is 1K bytes.
void VK_WriteInfo(uint32_t block, VK_DATA_BLOCK *data);

// Read a block of data from the game pak
void VK_ReadInfo(uint32_t block,VK_DATA_BLOCK *data);

void VK_MissingParts();
void VK_DisplayMessage(int index);

int VK_LoadOptions();

void VK_WaitAMoment();

void VK_DoIntroDemo();

void VK_DrawBox(uint16_t spawnx, uint16_t spawny, uint16_t width, uint16_t height);
void VK_SpawnBox(uint16_t spawnx, uint16_t spawny, uint16_t width, uint16_t height);

void VK_DrawHighScores();
void VK_DrawStory();

void VK_DrawTitleScreen();
uint16_t VK_LoadMenu();
void VK_DrawMainMenu();
void VK_MainMenu();


void VK_StatusBar();
uint16_t VK_QuitDialog();
void VK_KeensLeft();

void VK_CopyEndScreenGfx();
void VK_DrawEndScreen(uint8_t lights_on);

void VK_DrawEndTBC();

#endif
