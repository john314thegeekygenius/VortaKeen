
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

#ifndef __VK_SPRITE_ENGINE__
#define __VK_SPRITE_ENGINE__

typedef struct vk_sprite {
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
	uint32_t *spriteptr;
}vk_sprite;

vk_sprite *VK_GetSprite();
void VK_ClearSprites();
void VK_RenderSprites();


#endif
