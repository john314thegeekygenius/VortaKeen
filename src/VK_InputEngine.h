
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

#ifndef __VK_INPUT_ENGINE__
#define __VK_INPUT_ENGINE__

extern uint16_t vk_keen_input[7]; 

#define VK_CheckButton(b) ((~GBA_BUTTONS) & (b))

uint16_t VK_WaitForButton();
uint16_t VK_ButtonDown();
uint16_t VK_ButtonUp();
void VK_UpdateInput();

#endif
