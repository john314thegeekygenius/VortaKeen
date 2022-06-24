
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

#ifndef __VK_OBJECTS_ENGINE__
#define __VK_OBJECTS_ENGINE__

#define VK_MAX_OBJECTS 128

typedef struct vk_obj_ani {
	uint16_t gfxoff;
	uint16_t ticks;
	struct vk_obj_ani *next;
}vk_obj_ani;

typedef struct vk_object {
	uint16_t x;
	uint16_t y;

	// Animation Stuff
	uint16_t gfx_needs_update;
	uint16_t frame_count;
	vk_obj_ani *animation;

	// Sprite stuff
	vk_sprite *s;
}vk_object;

vk_object *VK_CreateObject(uint16_t sprite_id);
void VK_RemoveObjects();
void VK_RenderObjects();


#endif
