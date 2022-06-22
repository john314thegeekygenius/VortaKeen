
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


uint16_t vk_engine_demo = 0;
vk_game_state vk_engine_gstate;

void VK_DoGameLoop(){

	vk_engine_gstate.in_game = 1;

	// We faded out, so fade in
	vk_engine_gstate.faded = 1;
	
	// Load the world map
	vk_engine_gstate.level_to_load = 80;
	
	uint16_t KEENX = 0, KEENY = 0, LEVEL_ON = 0;
	
	while(vk_engine_gstate.in_game){
		
		if(vk_engine_gstate.level_to_load!=0){		
			VK_LoadLevel(vk_engine_gstate.level_to_load);
			vk_engine_gstate.in_game = vk_engine_gstate.level_to_load;
			vk_engine_gstate.level_to_load = 0;
		}


		VK_UpdateInput();
		
		if(VK_ButtonDown()==GBA_BUTTON_RIGHT){
			KEENX += 1;
		}
		if(VK_ButtonDown()==GBA_BUTTON_LEFT){
			KEENX -= 1;
		}
		if(VK_ButtonDown()==GBA_BUTTON_DOWN){
			KEENY += 1;
		}
		if(VK_ButtonDown()==GBA_BUTTON_UP){
			KEENY -= 1;
		}
		if(VK_ButtonDown()==GBA_BUTTON_A){
			KEENX = 0;
			KEENY = 0;
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
		VK_UnLockCamera();
		VK_PositionLevel(KEENX>>4,KEENY>>4);
		VK_PositionCamera(KEENX&0xF,KEENY&0xF);
		
		VK_UpdateLevel();
		VK_RenderLevel();
		
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
