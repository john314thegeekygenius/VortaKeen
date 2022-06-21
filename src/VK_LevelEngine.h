
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

#ifndef __VK_LEVEL_ENGINE__
#define __VK_LEVEL_ENGINE__



extern unsigned short vk_num_of_tiles;

extern unsigned short vk_level_width;
extern unsigned short vk_level_height;

extern unsigned short vk_level_offsetx ;
extern unsigned short vk_level_offsety ;

extern unsigned short vk_map_offsetx ;
extern unsigned short vk_map_offsety ;

extern unsigned char vk_level_needs_update;
extern unsigned short vk_level_update_tick;

void VK_LoadLevel(uint16_t levelid);

// Render the level
void VK_RenderLevel();

// Position the level
void VK_PositionLevel(uint16_t offsetx,uint16_t offsety);

// Update the level
void VK_UpdateLevel();

// Position the map offset
void VK_PositionCamera(uint16_t offsetx,uint16_t offsety);

// Lock the camera
void VK_LockCamera();

// UnLock the camera
void VK_UnLockCamera();

#endif
