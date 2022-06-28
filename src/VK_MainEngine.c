
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

#include "../graph/bitmaps/GAME_OVER.h"

uint16_t vk_engine_demo = 0;
vk_game_state vk_engine_gstate;


void VK_QuitGame(){
	uint16_t i,e, gameovertimer;
	// Play the gameover sound
	VK_PlaySound(VKS_GAMEOVERSND);
	
	// Load the GameOver bitmap
	GBA_DMA_Copy32(VK_GBA_BG_Tiles3,GAME_OVER_data,GAME_OVER_size>>2);

	// Depending on player score, output gameover, or congrats
	if(1==0){
	}else{
		// TODO:
		// Make this based off keens actual location
		VK_UnLockCamera();
		VK_PositionLevel(2,2);
		VK_PositionCamera(0,0);
		
		VK_UpdateLevel();
		VK_RenderLevel();
		
		VK_ClearTopLayer();
		
		// Render the GAME OVER bitmap
		for(e = 0; e < (GAME_OVER_height>>3); e++){
			for(i = 0; i < (GAME_OVER_width>>3); i++){
				VK_GBA_BG_MAPB[((e+8)<<5)+i+8] = (e*(GAME_OVER_width>>3))+i+(VK_GBA_TILES3_OFF);
			}
		}
		
		gameovertimer = 0x100;

		while(gameovertimer){
			gameovertimer --;
			VK_WaitVRB();
		}
	}

	VK_FadeOut();
	vk_engine_gstate.in_game = 0;
	
	vk_engine_demo = VK_DEMO_HIGHSCORES;
};

void VK_NewGame(){
	// Setup stuff for a new game
	
	vk_engine_gstate.next_1up = 20000;

	// We faded out, so fade in
	vk_engine_gstate.faded = 1;
	
	VK_ReturnToWorldmap();
};

void VK_ReturnToWorldmap(){
	// Load the world map
	vk_engine_gstate.level_to_load = 80;
	
	// Fix the map (cover up levels completed)
	
	
	// Fade out if we need to
	if(vk_engine_gstate.faded == 0){
		VK_FadeOut();
		vk_engine_gstate.faded = 1;
	}
};

void VK_DoGameLoop(){

	vk_engine_gstate.in_game = 1;
	
	
	uint16_t LEVEL_ON = 0;
	
	while(vk_engine_gstate.in_game){
		
		if(vk_engine_gstate.level_to_load!=0){		
			VK_LoadLevel(vk_engine_gstate.level_to_load);
			vk_engine_gstate.in_game = vk_engine_gstate.level_to_load;
			vk_engine_gstate.level_to_load = 0;
		}


		VK_UpdateInput();
		
		uint16_t button_up = VK_ButtonUp();
		if(button_up==GBA_BUTTON_START){
			// Spawn status bar
			VK_StatusBar();
		}
		if(button_up==GBA_BUTTON_SELECT){
			// Spawn exit dialog
			if(VK_QuitDialog()){
				VK_QuitGame();
				return;
			}
		}
		if(button_up==GBA_BUTTON_RSHOLDER){
			if(vk_engine_gstate.level_to_load == 80){
				vk_engine_gstate.level_to_load = 1;
			}else{
				LEVEL_ON += 1;
				if(LEVEL_ON>16)
					LEVEL_ON = 80;
				vk_engine_gstate.level_to_load = LEVEL_ON;
			}
			vk_engine_gstate.faded = 1;
			if(vk_engine_gstate.in_game==80){
				VK_PlaySound(2);
				VK_FadeOut();
				while(VK_SoundDone()==0);
			}else{
				VK_PlaySound(14);
				while(VK_SoundDone()==0);
				VK_FadeOut();
			}
			
		}

		// Position the level
		if(vk_keen_obj->hitmap){
			VK_UnLockCamera();
			VK_PositionCamera((vk_keen_obj->pos_x>>8)&0xF,(vk_keen_obj->pos_y>>8)&0xF);
			VK_PositionLevel((vk_keen_obj->pos_x>>12)-8,(vk_keen_obj->pos_y>>12)-6);
		}
		
		VK_UpdateLevel();
		VKF_keen_input(vk_keen_obj);
		VK_RenderLevel();
		
		VK_RenderObjects();
		
		if(vk_engine_gstate.level_to_load==0){
			if(vk_engine_gstate.faded==1){
				vk_engine_gstate.faded = 0;
				
				if(vk_engine_gstate.in_game==80){
					VK_PlaySound(40);
				}
				
				VK_FadeIn();
			}
			if(vk_engine_gstate.faded==2){
				vk_engine_gstate.faded = 0;
				VK_FadeOut();
			}
		}
		
		VK_WaitVRB();
		
	}
};

void VK_MainEngine(){
	
	while(1){
		// Run the demos
		switch(vk_engine_demo){
			case VK_DEMO_INTRO:
				VK_WaitAMoment();
				VK_DoIntroDemo();
			break;	
			case VK_DEMO_MAINMENU:
				VK_DrawTitleScreen();
				VK_MainMenu();
			break;
			case VK_DEMO_HIGHSCORES:
				VK_DrawHighScores();
			break;
			case VK_DEMO_STORY:
				VK_DrawStory();
			break;
			case VK_DEMO_GAME:
				VK_DoGameLoop();
			break;
		}
	}
};
