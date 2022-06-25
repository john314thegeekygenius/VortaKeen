
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
	uint16_t gfxoff_r;
	uint16_t gfxoff_l;
	uint16_t ticks;
	struct vk_obj_ani *next;
}vk_obj_ani;

typedef struct vk_object {
	int16_t pos_x;
	int16_t pos_y;
	int16_t vel_x;
	int16_t vel_y;
	uint8_t facing; // 0 - left  1 - right


	uint16_t var1;
	uint16_t var2;
	uint16_t var3;
	uint16_t var4;

	// Animation Stuff
	uint16_t gfx_needs_update;
	uint16_t frame_count;
	vk_obj_ani *animation;
	
	// Logic stuff
	 int (*collide)(struct vk_object*obj);
	 int (*think)(struct vk_object*obj);

	// Sprite stuff
	vk_sprite *s;
}vk_object;

extern vk_object *vk_keen_obj;

vk_object *VK_CreateObject(uint16_t sprite_id, uint16_t x, uint16_t y);
void VK_SetObjAnimation(vk_object *obj,vk_obj_ani *animation);
void VK_RemoveObjects();
void VK_RenderObjects();


#endif
