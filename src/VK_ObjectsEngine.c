
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

#include "VK_Animations.h"


GBA_IN_EWRAM vk_object vk_level_objects[VK_MAX_OBJECTS];
uint16_t vk_num_of_objects = 0;



vk_object *VK_CreateObject(uint16_t sprite_id){
	vk_object * obj = &vk_level_objects[vk_num_of_objects];
	vk_num_of_objects+= 1;
	if(vk_num_of_objects >= VK_MAX_OBJECTS){
		vk_num_of_objects = 0; // Loop arround?
		while(1); // Just hang
	}

	switch(sprite_id){
		case 1:
			// Yorp
			obj->s = VK_CreateSprite(1);
			obj->animation = &VKA_yorp_idle_1;
			break;
		case 2:
			// Garg
		break;
		case 3:
			// Vorticon
		break;
		case 4:
			// Butler robot
		break;
		case 5:
			// Tank robot
		break;
		case 6:
			// Ice up / right
		break;
		case 7:
			// Ice up
		break;
		case 8:
			// Ice down
		break;
		case 9:
			// Ice up / left
		break;
		case 10:
			// Chain
		break;
		case 255:
			// Commander Keen
			obj->s = VK_CreateSprite(255);
			
		break;
	};
	obj->gfx_needs_update = 1;
	obj->frame_count = 0;

	return obj;
};

void VK_RemoveObjects(){
	VK_ClearSprites();

	vk_num_of_objects = 0;
};

void VK_RenderObjects(){
	int i, cx, cy;
	for(i=0;i<vk_num_of_objects;i++){
		vk_object * obj = &vk_level_objects[i];
		
		if(obj!=NULL&&obj->s!=NULL){
			
			if(obj->animation!=NULL){
				obj->frame_count += 1;

				if(obj->frame_count >= obj->animation->ticks){
					obj->frame_count = 0;
					obj->animation = obj->animation->next;
					obj->s->s.spr_gfx_ani = obj->animation->gfxoff;
					obj->gfx_needs_update = 1;
				}
			}

			cx = obj->x-(vk_level_offsetx<<4);
			cy = obj->y-(vk_level_offsety<<4);
			cx -= vk_map_offsetx;
			cy -= vk_map_offsety;
			
			if(cx >= (0-obj->s->s.spr_width) && cy >= (0-obj->s->s.spr_height) && cx < (32<<3) && cy < (20<<3)){
				if(obj->gfx_needs_update){
					obj->gfx_needs_update = 0;
					VK_SetSpriteGraphics(obj->s);
				}
				obj->s->x = cx;
				obj->s->y = cy;
				VK_RenderSprite(obj->s);
			}
		}
	}
};

