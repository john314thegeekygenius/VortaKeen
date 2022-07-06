
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

const int VK_END_FADE_TIME = 6000; // Wait 6 seconds
const int VK_END_TEXT_WAIT_TIME = 2000; // Wait 2 seconds
const int VK_END_LIGHT_WAIT_TIME = 3000; // Wait 3 seconds
const int VK_END_LIGHT_WAIT_TIME2 = 4000; // Wait 4 seconds




uint16_t vk_engine_demo = 0;
vk_game_state vk_engine_gstate;


void VK_ToggleLights(){
	vk_engine_gstate.lights_out = !vk_engine_gstate.lights_out;
	VK_SetPalette(vk_engine_gstate.lights_out<<1);
};


void VK_QuitGame(){
	uint16_t i,e, gameovertimer, is_highscore;
	uint16_t name_offset = 0;
	char name[18];
	
	// Load the GameOver bitmap
	GBA_DMA_Copy32(VK_GBA_BG_Tiles3,GAME_OVER_data,GAME_OVER_size>>2);

	// Depending on player score, output gameover, or congrats
	is_highscore = 0;
	for(e = 0; e < 6; e++){
		if(vk_engine_gstate.score > vk_engine_gstate.thekeenest[e].score){
			is_highscore = 1+e;
			break;
		}
	}
	if(is_highscore){

		// Play the gameover sound
		VK_PlaySound(VKS_HISCORESND);


		VK_UnLockCamera();
		VK_PositionCamera(0,0);
		
		VK_UpdateLevel();
		VK_RenderLevel();
		
		VK_ClearTopLayer();
		
				
		VK_SpawnBox(14,9,29,15);
		
		// Render the GAME OVER bitmap
		for(e = 0; e < (GAME_OVER_height>>3); e++){
			for(i = 0; i < (GAME_OVER_width>>3); i++){
				VK_GBA_BG_MAPB[((e+4)<<5)+i+8] = (e*(GAME_OVER_width>>3))+i+(VK_GBA_TILES3_OFF);
			}
		}
		
		VK_TextX = 9;
		VK_TextY = 8;
		VK_Print("SCORE:");
		VK_TextX += 6;
		VK_Print(VK_Iota(vk_engine_gstate.score));
		VK_TextY += 4;
		VK_TextX = 7;
		VK_Print("CONGRATULATIONS!");
		VK_TextY += 1;
		VK_TextX -= 2;
		VK_Print("You got");
		VK_TextX += 8;
		if(is_highscore==1){
			VK_Print("First Place!");
		}
		if(is_highscore==2){
			VK_Print("Second Place!");
		}
		if(is_highscore==3){
			VK_Print("Third Place!");
		}
		if(is_highscore==4){
			VK_Print("Fourth Place!");
		}
		if(is_highscore==5){
			VK_Print("Fifth Place!");
		}
		if(is_highscore==6){
			VK_Print("Last Place!");
		}

		VK_TextX = 1;
		VK_TextY = 14;
		VK_Print("Enter Name:");
		VK_TextX = 4;
		VK_TextY = 16;
		VK_Print("Press Select to finish");

		// Draw the items
		if(vk_engine_gstate.gotJoystick){
			VK_DrawTile(10,9,(vk_special_items+10));
		}
		if(vk_engine_gstate.gotBattery){
			VK_DrawTile(12,9,(vk_special_items+11));
		}
		if(vk_engine_gstate.gotVacuum){
			VK_DrawTile(14,9,(vk_special_items+12));
		}
		if(vk_engine_gstate.gotWhiskey){
			VK_DrawTile(16,9,(vk_special_items+13));
		}

		
		memset(&name,0,18);
		
		
		uint16_t cursor_animation = 0;
		uint16_t cursor_ani_tick = 0;
		
		// Set the first character to A
		name[0] = 'A';
		name_offset = 0;
	
		while(1){
			
			VK_UpdateInput();
			
			if(VK_ButtonUp()==GBA_BUTTON_UP){
				if(name[name_offset]==0){
					name[name_offset] = 'A';
				}else if(name[name_offset]<'z'+4){
					name[name_offset] += 1;
				}
			}
			if(VK_ButtonUp()==GBA_BUTTON_DOWN){
				if(name[name_offset]<=' '){
					name[name_offset] = 'z'+4;
				}else{
					name[name_offset] -= 1;
				}
			}

			if(VK_ButtonUp()==GBA_BUTTON_B){
				VK_GBA_BG_MAPA[((14)<<5)+name_offset+13] = 0x2E0;
				name[name_offset] = 0;
				if(name_offset>0){
					name_offset -= 1;
				}
			}
			if(VK_ButtonUp()==GBA_BUTTON_A){
				VK_GBA_BG_MAPA[((14)<<5)+name_offset+13] = 0x2E0;

				if(name[name_offset]!=0){
					name_offset += 1;
				}

				if(name_offset>11){
					name_offset = 11;
				}
			}
			if(VK_ButtonUp()==GBA_BUTTON_SELECT){
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

			// Write the name
			for(i = 0; i < 16; i++){
				if(name[i]==0){
					VK_GBA_BG_MAPA[((14)<<5)+i+12] = 0x2E0;
					break;
				}
				VK_GBA_BG_MAPA[((14)<<5)+i+12] = 0x2E0+name[i]-32;
			}

			// Draw selector
			VK_GBA_BG_MAPA[((14)<<5)+name_offset+13] = 0x2C9+cursor_animation;
			
			VK_WaitVRB();
			
		}
		

		for(e = 5; e >= is_highscore; e--){
			memcpy(&vk_engine_gstate.thekeenest[e].score,&vk_engine_gstate.thekeenest[e-1].score,4);
			memcpy(&vk_engine_gstate.thekeenest[e].items,&vk_engine_gstate.thekeenest[e-1].items,4);
			memcpy(&vk_engine_gstate.thekeenest[e].citys,&vk_engine_gstate.thekeenest[e-1].citys,7);
			memcpy(&vk_engine_gstate.thekeenest[e].name,&vk_engine_gstate.thekeenest[e-1].name,16);
		}
		vk_engine_gstate.thekeenest[is_highscore-1].score = vk_engine_gstate.score;
		memcpy(vk_engine_gstate.thekeenest[is_highscore-1].name,name,16);
		
		vk_engine_gstate.thekeenest[is_highscore-1].items[0] = vk_engine_gstate.gotJoystick;
		vk_engine_gstate.thekeenest[is_highscore-1].items[1] = vk_engine_gstate.gotBattery;
		vk_engine_gstate.thekeenest[is_highscore-1].items[2] = vk_engine_gstate.gotVacuum;
		vk_engine_gstate.thekeenest[is_highscore-1].items[3] = vk_engine_gstate.gotWhiskey;
		
		// Save the new high scores
		VK_SaveHighScores();

	}else{

		VK_UnLockCamera();
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
		
	}

	// Play the gameover sound
	VK_PlaySound(VKS_GAMEOVERSND);

	gameovertimer = 0xA0;

	while(gameovertimer){
		gameovertimer --;
		VK_WaitVRB();
	}
	
	VK_FadeOut();
	vk_engine_gstate.in_game = 0;
	
	vk_engine_demo = VK_DEMO_HIGHSCORES;
};

void VK_NewGame(){
	int i;
	// Setup stuff for a new game
	
	vk_engine_gstate.lights_out = 0;
	vk_engine_gstate.next_1up = 20000;
	
	vk_engine_gstate.num_of_done = 0;
	vk_engine_gstate.gotBattery = 0;
	vk_engine_gstate.gotVacuum = 0;
	vk_engine_gstate.gotJoystick = 0;
	vk_engine_gstate.gotPogo = 0;
	vk_engine_gstate.gotWhiskey = 0;
	vk_engine_gstate.gotKeycardR = 0;
	vk_engine_gstate.gotKeycardG = 0;
	vk_engine_gstate.gotKeycardB = 0;
	vk_engine_gstate.gotKeycardY = 0;
	for(i=0;i<16;i++)
		vk_engine_gstate.levelDone[i] = 0;
	vk_engine_gstate.numLives = 4;
	vk_engine_gstate.ammo = 0;
	vk_engine_gstate.score = 0;
	vk_engine_gstate.posX = 0xFFFF;
	vk_engine_gstate.posY = 0xFFFF;
	vk_engine_gstate.viewportX = 0;
	vk_engine_gstate.viewportY = 0;
	for(i=0;i<9;i++)
		vk_engine_gstate.cities[i] = 0;

	// Make sure we load everything
	vk_engine_gstate.has_loaded = 0;

	// Reset the ending
	vk_engine_gstate.endsequence = 0;
	vk_engine_gstate.endtext = 0;

	// We faded out, so fade in
	vk_engine_gstate.faded = 1;
		
	VK_ReturnToWorldmap();
};

void VK_InitGame(){

	vk_engine_gstate.lights_out = 0;

	// Reset the ending
	vk_engine_gstate.endsequence = 0;
	vk_engine_gstate.endtext = 0;

	// We faded out, so fade in
	vk_engine_gstate.faded = 1;
		
	VK_ReturnToWorldmap();
};

void VK_ReturnToWorldmap(){

	// Fade out if we need to
	if(vk_engine_gstate.faded == 0){
		VK_FadeOut();
		vk_engine_gstate.faded = 1;
	}

	// Load the world map
	VK_LoadLevel(80);
	vk_engine_gstate.in_game = 80;
	vk_engine_gstate.level_to_load = 0;
	vk_engine_gstate.lights_out = 0;

	if((vk_engine_gstate.posX != 0xFFFF)&& (vk_engine_gstate.posY != 0xFFFF)){
		vk_keen_obj->pos_x = vk_engine_gstate.posX;
		vk_keen_obj->pos_y = vk_engine_gstate.posY;
		
		vk_engine_gstate.posX = 0xFFFF;
		vk_engine_gstate.posY = 0xFFFF;
		
		vk_viewport_x = vk_engine_gstate.viewportX;
		vk_viewport_y = vk_engine_gstate.viewportY;
	}
	
	// Check for win state
	vk_engine_gstate.endsequence = 0;

	if(vk_engine_gstate.gotBattery && 
		vk_engine_gstate.gotJoystick &&
		vk_engine_gstate.gotVacuum &&
		vk_engine_gstate.gotWhiskey){
		// Tell the engine what's going on
		vk_engine_gstate.endsequence = 1;
		// Reposition keen
		vk_keen_obj->pos_x = VK_END_KEEN_POS[0]; // Defined in VK_LevelEngine.c
		vk_keen_obj->pos_y = VK_END_KEEN_POS[1]; 
		vk_keen_obj->facing = 0; // Face left
		extern vk_obj_ani VKA_mapkeen_idle_h;
		VK_SetObjAnimation(vk_keen_obj,&VKA_mapkeen_idle_h);
		// Set the viewport
		vk_viewport_x = (vk_keen_obj->pos_x-(6<<12));
		vk_viewport_y = (vk_keen_obj->pos_y-(2<<12)); // Make it a little lower
		// Remove the old ship
		vk_endship_obj = NULL;
		vk_endpression_obj = NULL;
	}else{
		// Fix the map (cover up levels completed)
		VK_ClearWorldMap();
	}
	VK_ForceLevelUpdate();

	VK_UnLockCamera();
	VK_PositionCamera((vk_viewport_x>>8)&0xF,(vk_viewport_y>>8)&0xF);
	VK_PositionLevel((vk_viewport_x>>12),(vk_viewport_y>>12));

	VK_RenderLevel();
	
};

void VK_DoGameLoop(){

	if(vk_engine_gstate.in_game==0){
		vk_engine_gstate.in_game = 1;
	}
	
	// We should never be teleporting when we first load
	vk_engine_gstate.teleporting = 0;
	
	while(vk_engine_gstate.in_game){
		
		if(vk_engine_gstate.level_to_load!=0){
			VK_LoadLevel(vk_engine_gstate.level_to_load);
			vk_engine_gstate.in_game = vk_engine_gstate.level_to_load;
			vk_engine_gstate.level_to_load = 0;
			
			// Spawn the spaceship
			if(vk_engine_gstate.endsequence==2){
				if(vk_endship_obj == NULL){
					// VK_END_SHIP_POS defined in VK_ObjectEngine.c
					vk_endship_obj = VK_CreateObject(20,VK_END_SHIP_POS[0],VK_END_SHIP_POS[1]);
					vk_endpression_obj = VK_CreateObject(21,0x8F0000,0x8F0000);
					// HACK: So things work right
					vk_keen_obj = vk_endship_obj;
				}
			}
		}
		
		if(vk_engine_gstate.endsequence==2||vk_engine_gstate.endsequence==4){
			if(vk_endship_obj->var1 < 2){
				// Fix the camera position
				vk_viewport_x = vk_endship_obj->pos_x-(8<<12);
				vk_viewport_y = vk_endship_obj->pos_y-(4<<12);

				VK_UnLockCamera();
				VK_PositionCamera((vk_viewport_x>>8)&0xF,(vk_viewport_y>>8)&0xF);
				VK_PositionLevel((vk_viewport_x>>12),(vk_viewport_y>>12));
			}
			if(vk_endship_obj->var1==6){
				// Fade out
				VK_FadeOut();
				
				// Hide the ship
				VK_HideObjects();
				
				// Make it 3
				vk_engine_gstate.endsequence += 1;

				// Draw End scene
				VK_CopyEndScreenGfx();
				
				VK_DrawEndScreen(0);
				VK_FadeIn();
				
				// Do the dialog
				int i;
				for(i = 0; i < 8; i++){
					if(i==2){
						VK_DrawEndText3(i+1);
					}else{
						VK_DrawEndText2(i+1);
					}
					GBA_Delay(VK_END_TEXT_WAIT_TIME); // Defined in VK_MainEngine.c
					VK_DrawEndScreen(0);
					// Special cases
					if(i==0){
						// Wait
						GBA_Delay(VK_END_LIGHT_WAIT_TIME); // Defined in VK_MainEngine.c
						// Window light on
						VK_DrawEndScreen(1);
						// Play light switch sound
						VK_PlaySound(VKS_CLICKSND);
						// Wait
						GBA_Delay(VK_END_LIGHT_WAIT_TIME); // Defined in VK_MainEngine.c
						// Window light off
						VK_DrawEndScreen(2);
						// Play light switch sound
						VK_PlaySound(VKS_CLICKSND);	
						// Wait
						GBA_Delay(VK_END_LIGHT_WAIT_TIME2); // Defined in VK_MainEngine.c
					}
					if(i==1){
						// Wait
						GBA_Delay(VK_END_LIGHT_WAIT_TIME); // Defined in VK_MainEngine.c
						// Window light on
						VK_DrawEndScreen(1);
						// Play light switch sound
						VK_PlaySound(VKS_CLICKSND);

					}
					if(i>=2&&i<=6){
						// Window light on
						VK_DrawEndScreen(1);
					}
					if(i==6){
						// Wait
						GBA_Delay(VK_END_LIGHT_WAIT_TIME); // Defined in VK_MainEngine.c
						// Window light off
						VK_DrawEndScreen(2);
						// Play light switch sound
						VK_PlaySound(VKS_CLICKSND);	
						// Wait
						GBA_Delay(VK_END_LIGHT_WAIT_TIME2); // Defined in VK_MainEngine.c
						// Window light on
						VK_DrawEndScreen(1);
						// Play light switch sound
						VK_PlaySound(VKS_CLICKSND);
					}
				}
//				VK_FadeOut();
				VK_ReloadLevelGfx();
				vk_engine_gstate.faded = 1;
				vk_endship_obj->var1 = 7;
				vk_engine_gstate.endsequence += 1;

				VK_UnLockCamera();
				VK_PositionCamera((vk_viewport_x>>8)&0xF,(vk_viewport_y>>8)&0xF);
				VK_PositionLevel((vk_viewport_x>>12),(vk_viewport_y>>12));

				VK_ForceLevelUpdate();
			}
		}
		if(vk_engine_gstate.endsequence==5){
			VK_DrawEndTBC();
			VK_UpdateInput();
			while(!VK_ButtonUp()){
				VK_UpdateInput();
				VK_WaitVRB();
			}
			// Hide the ship
			VK_HideObjects();
			
			// Display end screen
			VK_DrawFinalText();

			// Redraw the level
			VK_ForceLevelUpdate();
			VK_RenderLevel();

			// End Game
			VK_QuitGame();
		}
		

		if(vk_engine_gstate.endsequence==0){

			if(vk_engine_gstate.teleporting){
				vk_engine_gstate.teleporting -= 1;
				if(vk_engine_gstate.teleporting==0x00){
					vk_level_data[vk_engine_gstate.teleporter_pos] = vk_engine_gstate.teleporter;

					// Remove the animation
					ck_number_of_updates -= 1;
					ck_update_positions[ck_number_of_updates][0] = 0;
					ck_update_positions[ck_number_of_updates][1] = 0;

					vk_viewport_x = vk_keen_obj->pos_x-(8<<12);
					vk_viewport_y = vk_keen_obj->pos_y-(6<<12);
					
					VK_ForceLevelUpdate();
				}
			}else{

				VK_UpdateInput();
				
				uint16_t button_up = VK_ButtonUp();
				
				// Grab the input
				if(VK_CheckButton(GBA_BUTTON_RIGHT)){
					vk_keen_input[0] = 1;
				}else{
					vk_keen_input[0] = 0;
				}
				if(VK_CheckButton(GBA_BUTTON_LEFT)){
					vk_keen_input[1] = 1;
				}else{
					vk_keen_input[1] = 0;
				}
				if(VK_CheckButton(GBA_BUTTON_UP)){
					vk_keen_input[2] = 1;
				}else{
					vk_keen_input[2] = 0;
				}
				if(VK_CheckButton(GBA_BUTTON_DOWN)){
					vk_keen_input[3] = 1;
				}else{
					vk_keen_input[3] = 0;
				}
				if(VK_CheckButton(GBA_BUTTON_A)){
					vk_keen_input[4] = 1;
				}else{
					vk_keen_input[4] = 0;
				}
				if(VK_CheckButton(GBA_BUTTON_B)){
					vk_keen_input[5] = 1;
				}else{
					vk_keen_input[5] = 0;
				}
				if(vk_engine_gstate.pogo_swaped==1){
					if(VK_ButtonUp() == (GBA_BUTTON_LSHOLDER)){
						vk_keen_input[6] = 1;
					}else{
						vk_keen_input[6] = 0;
					}
					if(VK_ButtonUp() == (GBA_BUTTON_RSHOLDER)){
						VK_InfoOptions();
					}
				}else{
					if(VK_ButtonUp() == (GBA_BUTTON_RSHOLDER)){
						vk_keen_input[6] = 1;
					}else{
						vk_keen_input[6] = 0;
					}
					if(VK_ButtonUp() == (GBA_BUTTON_LSHOLDER)){
						VK_InfoOptions();
					}
				}

				// Position the level
				if(vk_keen_obj->hitmap){
					
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
					/*
					if(button_up==GBA_BUTTON_RSHOLDER){
						if(vk_engine_gstate.in_game>0&&vk_engine_gstate.in_game<=16){
							vk_engine_gstate.levelDone[vk_engine_gstate.in_game-1] = 1;

							VK_PlaySound(14);
							while(VK_SoundDone());
							VK_ReturnToWorldmap();

						}	
					}*/
					
					if(vk_keen_obj->pos_x > (vk_viewport_x + (8<<12))){//
						vk_viewport_x += 0x200;
						if(vk_keen_obj->pos_x < (vk_viewport_x + (8<<12))){//
							vk_viewport_x = (vk_keen_obj->pos_x-(8<<12));
						}
					}
					if(vk_keen_obj->pos_x < (vk_viewport_x+(6<<12))){//
						vk_viewport_x -= 0x200;
						if(vk_keen_obj->pos_x > (vk_viewport_x + (6<<12))){//
							vk_viewport_x = (vk_keen_obj->pos_x-(6<<12));
						}
					}
					if(vk_keen_obj->pos_y > (vk_viewport_y + (7<<12))){//
						vk_viewport_y += 0x400;
						if(vk_keen_obj->pos_y < (vk_viewport_y + (7<<12))){//
							vk_viewport_y = (vk_keen_obj->pos_y-(7<<12));
						}
					}
					if(vk_keen_obj->pos_y < (vk_viewport_y+(3<<12))){//
						vk_viewport_y -= 0x400;
						if(vk_keen_obj->pos_y > (vk_viewport_y + (3<<12))){//
							vk_viewport_y = (vk_keen_obj->pos_y-(3<<12));
						}
					}

					VK_UnLockCamera();
					VK_PositionCamera((vk_viewport_x>>8)&0xF,(vk_viewport_y>>8)&0xF);
					VK_PositionLevel((vk_viewport_x>>12),(vk_viewport_y>>12));
				}
			}
			
			VK_UpdateLevel();

			if(vk_engine_gstate.teleporting==0){
				if(vk_keen_obj->type==vko_keen){
					VK_CollideKeenWLevel(vk_keen_obj);
				}
				if(vk_keen_obj->type==vko_mapkeen){
					VK_CollideMapKeenWLevel(vk_keen_obj);
				}

				if(vk_keen_obj->type==vko_keen){
					VKF_keen_input(vk_keen_obj);
				}else{
					VKF_mapkeen_input(vk_keen_obj);
				}
			}
		}
		
		VK_RenderLevel();
		
		if(vk_engine_gstate.teleporting==0){
			VK_RenderObjects();
		}else{
			VK_HideObjects();
		}
		
		if(vk_engine_gstate.endsequence==1){
			if(vk_engine_gstate.endtext){
				VK_DrawEndText(vk_engine_gstate.endtext-1);
				GBA_Delay(VK_END_TEXT_WAIT_TIME); // Defined in VK_MainEngine.c
				if(vk_engine_gstate.endtext == 1){
					vk_engine_gstate.endsequence += 1;
					vk_engine_gstate.faded = 1; // Fade out
					VK_FadeOut();
					vk_engine_gstate.level_to_load = 81; // Load the level
				}
				vk_engine_gstate.endtext += 1;
			}
		}

		if(vk_engine_gstate.level_to_load==0){
			if(vk_engine_gstate.faded==1){
				vk_engine_gstate.faded = 0;
				
				if(vk_engine_gstate.endsequence==1){
					if(vk_engine_gstate.in_game==80){
						// Wait for a bit
						GBA_Delay(VK_END_FADE_TIME); // Defined in VK_MainEngine.c
						vk_engine_gstate.endtext = 1; // Set this to 1
					}
				}
				VK_FadeIn();
				if(vk_engine_gstate.endsequence==0){
					if(vk_engine_gstate.in_game==80){
						if(vk_engine_gstate.teleporting==0x00){
							VK_KeensLeft();
						}
					}
				}
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

	VK_LoadHighScores();
	
	VK_LoadOptions();
	
	VK_SoundEnabled = vk_engine_gstate.sound_enabled;
		
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
