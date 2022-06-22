
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

uint16_t VK_WaitForButton(){
	while(1){
		uint16_t b = VK_ButtonDown();
		if(b){
			return b;
		}
	}
};

uint16_t vk_input_buttons[] = {
	GBA_BUTTON_A,
	GBA_BUTTON_B,
	GBA_BUTTON_LSHOLDER,
	GBA_BUTTON_RSHOLDER,
	GBA_BUTTON_UP,
	GBA_BUTTON_DOWN,
	GBA_BUTTON_LEFT,
	GBA_BUTTON_RIGHT,
	GBA_BUTTON_START,
	GBA_BUTTON_SELECT,
};

uint16_t VK_ButtonDown(){
	int i;
	for (int i = 0; i < 10; i++){
		if(VK_CheckButton(vk_input_buttons[i]))
			return vk_input_buttons[i];
	}
	return 0; // No button
};

uint16_t vk_last_button = 0;
uint16_t vk_current_button = 0;

uint16_t VK_ButtonUp(){
	if(vk_current_button!=vk_last_button){
		return vk_last_button;
	}
	return 0;
};

void VK_UpdateInput(){
	vk_last_button = vk_current_button;
	vk_current_button = VK_ButtonDown();
};
