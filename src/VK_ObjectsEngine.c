
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

GBA_IN_EWRAM vk_object vk_level_objects[VK_MAX_OBJECTS];
uint16_t vk_num_of_objects = 0;

vk_object *vk_keen_obj = NULL;

uint16_t vk_oupdate_tick = 0;

#include "VK_Animations.h"

vk_object *VK_CreateObject(uint16_t sprite_id, uint16_t x, uint16_t y){
	vk_object * obj = &vk_level_objects[vk_num_of_objects];
	vk_num_of_objects+= 1;
	if(vk_num_of_objects >= VK_MAX_OBJECTS){
		vk_num_of_objects = 0; // Loop arround?
		while(1); // Just hang
	}
	obj->s = NULL;
	obj->animation = NULL;
	obj->collide = NULL;
	obj->think = NULL;

	obj->vel_x = 0;
	obj->vel_y = 0;

	switch(sprite_id){
		case 1:
			// Yorp
			obj->s = VK_CreateSprite(1);
			obj->animation = &VKA_yorp_idle_1;
			obj->collide = &VKF_yorp_collide;
			obj->think = &VKF_yorp_think;
			VKF_yorp_init(obj);
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
			// WARNING: This makes it so that only one keen sprite is playable
			vk_keen_obj = obj;
		break;
	};
	obj->pos_x = x;
	obj->pos_y = y;
	obj->gfx_needs_update = 1;
	obj->frame_count = 0;

	return obj;
};

void VK_RemoveObjects(){
	VK_ClearSprites();

	vk_oupdate_tick = 0;
	vk_num_of_objects = 0;
};

void VK_SetObjAnimation(vk_object *obj,vk_obj_ani *animation){
	obj->frame_count = 0;
	obj->animation = animation;
	if(obj->facing==0){
		obj->s->s.spr_gfx_ani = obj->animation->gfxoff_l;
	}else{
		obj->s->s.spr_gfx_ani = obj->animation->gfxoff_r;
	}
	obj->gfx_needs_update = 1;
};

void VK_RenderObjects(){
	int i, cx, cy;

	vk_oupdate_tick += 1;

	for(i=0;i<vk_num_of_objects;i++){
		vk_object * obj = &vk_level_objects[i];
		
		if(obj!=NULL&&obj->s!=NULL){
			
			if(vk_oupdate_tick >= 0x4){
				if(obj->animation!=NULL){
					obj->frame_count += 1;

					if(obj->frame_count >= obj->animation->ticks){
						VK_SetObjAnimation(obj,obj->animation->next);
					}
				}

				if(obj->collide!=NULL){
					obj->collide(obj);
				}
				if(obj->think!=NULL){
					obj->think(obj);
				}
			}

			cx = obj->pos_x-(vk_level_offsetx<<4);
			cy = obj->pos_y-(vk_level_offsety<<4);
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

	if(vk_oupdate_tick >= 0x4){
		vk_oupdate_tick = 0;
	}

};

