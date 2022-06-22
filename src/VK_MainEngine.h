
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

#ifndef __VK_MAIN_ENGINE__
#define __VK_MAIN_ENGINE__


enum {
	VK_DEMO_INTRO = 0,
	VK_DEMO_MAINMENU = 1,
	VK_DEMO_GAME = 2,
	VK_DEMO_STORY = 3,
	VK_DEMO_HIGHSCORES = 4,
};


typedef struct vk_game_state {
	// Engine stuff
	uint16_t level_to_load; // What level needs loaded
	uint16_t in_game; // Is keen in the game
	uint8_t faded; // Has the screen faded at all 0-no 1-out 2-in
	
	// Keen stuff
	uint16_t num_of_done; // Number of levels completed
	uint16_t gotBattery;// 	Have battery part? 0=no, 1=yes
	uint16_t gotVacuum;// 	Have vacuum cleaner part? 0=no, 1=yes
	uint16_t gotJoystick;// 	Have joystick part? 0=no, 1=yes
	uint16_t gotPogo;// 	Have pogo stick? 0=no, 1=yes
	uint16_t gotWhiskey;// 	Have whiskey part? 0=no, 1=yes
	uint16_t gotKeycardR;// 	Have red keycard? 0=no, 1=yes
	uint16_t gotKeycardG;// 	Have green keycard? 0=no, 1=yes
	uint16_t gotKeycardB;// 	Have blue keycard? 0=no, 1=yes
	uint16_t gotKeycardY;// 	Have yellow keycard? 0=no, 1=yes
	uint16_t levelDone[16];// 	16 values for each level, 0=incomplete, 1=completed. With the 32 levels patch this becomes 32 1-byte entries.
	uint16_t numLives;// 	Number of lives Keen has left
	uint16_t ammo;// 	Number of shots Keen has left
	uint16_t score;// 	Player score
	uint16_t posX;// 	Location on world map, in "global" units (256 units = 1 pixel, 4096 units = 1 tile)
	uint16_t posY;// 	Location on world map, in "global" units (256 units = 1 pixel, 4096 units = 1 tile)
	uint16_t viewportX;// 	Location of left of visible screen, in "global" units. Usually player location minus nine tiles.
	uint16_t viewportY;// 	Location of top of visible screen, in "global" units. Usually player location minus three tiles, but variable.
	uint16_t cities[9];// 	In Keen 2, cities saved. 9 2-byte entries, the last seems not to be a valid city, but is kept in memory. 
}vk_game_state;


extern vk_game_state vk_engine_gstate;
extern uint16_t vk_engine_demo;

void VK_WaitAMoment();
void VK_MainEngine();


#endif
