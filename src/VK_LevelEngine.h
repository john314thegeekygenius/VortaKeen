
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


extern const uint32_t VK_TELEPORT_DEST[];



extern unsigned short VK_CLEAR_TILE;

extern unsigned short vk_num_of_tiles;
extern unsigned short vk_special_items;

extern unsigned short vk_level_width;
extern unsigned short vk_level_height;

extern unsigned short *vk_level_map;

extern GBA_IN_EWRAM unsigned short vk_level_data[256*256];
extern GBA_IN_EWRAM unsigned char vk_sprite_data[32*256];
extern unsigned short *vk_level_tileinfo;

extern GBA_IN_EWRAM unsigned short ck_update_positions[0x1000][2];
extern GBA_IN_EWRAM unsigned short ck_update_locations[1024][2];
extern unsigned short ck_number_of_updates;
extern unsigned short ck_number_of_ulocations ;
extern GBA_IN_EWRAM unsigned char vk_tileanimations[0xFFF];


extern const uint8_t vk_bithash[];

extern signed short vk_level_offsetx ;
extern signed short vk_level_offsety ;

extern signed short vk_map_offsetx ;
extern signed short vk_map_offsety ;

extern unsigned char vk_level_needs_update;
extern unsigned short vk_level_update_tick;

extern uint32_t vk_viewport_x;
extern uint32_t vk_viewport_y;

extern unsigned char *TILESET_data;
extern unsigned short TILESET_size;

extern unsigned short vk_level_id;


// Clear GBA Map 1
void VK_ClearTopLayer();

void VK_ClearWorldMap();

void VK_LoadLevel(uint16_t levelid);

void VK_DrawTile(uint16_t x, uint16_t y, uint16_t lvlt);

// Render a tile
void VK_RenderTile(uint16_t offx, uint16_t offy, uint16_t lvlt, uint8_t plane);

// Render the level
void VK_RenderLevel();

// Disable the rendering
void VK_DisableLevelRendering();

// Update the level
void VK_UpdateLevel();

// Force a screen update
void VK_ForceLevelUpdate();

// Position the level
void VK_PositionLevel(uint16_t offsetx,uint16_t offsety);

// Position the map offset
void VK_PositionCamera(uint16_t offsetx,uint16_t offsety);

// Lock the camera
void VK_LockCamera();

// UnLock the camera
void VK_UnLockCamera();

#endif
