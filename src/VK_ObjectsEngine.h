
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

typedef struct vk_box {
	int16_t left;
	int16_t top;
	int16_t right;
	int16_t bottom;	
}vk_box;

typedef struct vk_obj_ani {
	uint16_t gfxoff_r;
	uint16_t gfxoff_l;
	uint16_t ticks;
	vk_box cbox;
	struct vk_obj_ani *next;
}vk_obj_ani;

typedef enum {
	vko_none 	= 0,
	vko_keen 	= 1,
	vko_yorp 	= 2,
	vko_garg 	= 3,
	vko_vorticon = 4,
}vk_obj_type;


typedef struct vk_object {
	int32_t pos_x;
	int32_t pos_y;
	int16_t vel_x;
	int16_t vel_y;
	uint8_t facing; // 0 - left  1 - right
	uint8_t hit_left, hit_right, hit_top, hit_bottom; // Which side of keen hit
	uint8_t hitmap; // 0 - none  1 - all
	
	uint8_t on_ground; 
						/*
							0 	Fall through
							1 	Solid
							2 	Slippery
							3 	Ice 
						*/

	uint16_t var1;
	uint16_t var2;
	uint16_t var3;
	uint16_t var4;
	
	vk_obj_type type; // What kind of object is it
	
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

vk_object *VK_CreateObject(uint16_t sprite_id, uint32_t x, uint32_t y);
void VK_SetObjAnimation(vk_object *obj,vk_obj_ani *animation);
void VK_RemoveObjects();
void VK_RenderObjects();


#endif
