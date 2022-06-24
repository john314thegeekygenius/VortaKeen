
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

#define VK_MAX_SPRITES 128

typedef struct vk_gba_spr{
	unsigned short spr_width;
	unsigned short spr_height;
	unsigned short sheet_width;
	unsigned short sheet_height;
	unsigned char *spr_data;

	unsigned short spr_indx[4];
	unsigned short spr_cw[4];
	unsigned short spr_ch[4];
	unsigned short spr_off[4][3];
	
	unsigned short spr_gfx_ani;
}vk_gba_spr;

typedef struct vk_sprite {
	int16_t x;
	int16_t y;
	vk_gba_spr s;
}vk_sprite;

vk_sprite *VK_CreateSprite(uint16_t sprite_id);
void VK_ClearSprites();
void VK_RenderSprites();
void VK_RenderSprite(vk_sprite *spr);


#endif
