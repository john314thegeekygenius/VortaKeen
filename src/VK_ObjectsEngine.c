
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


void VK_KeenCollectPoint(uint16_t amount){
	// Add the score
	vk_engine_gstate.score += amount;
	if(vk_engine_gstate.score >= vk_engine_gstate.next_1up){
		vk_engine_gstate.next_1up <<= 1; // Multiply by 2
		// Play the sound
		VK_PlaySound(VKS_HISCORESND);
	}else{
		// Play the sound
		VK_PlaySound(VKS_GOTITEMSND);
	}
};

// Advanced collistion code for keen vs level
int VK_CollideKeenWLevel(vk_object *obj){
	if(obj->animation==NULL){
		return 0;
	}

	int32_t kleft = (obj->pos_x+obj->animation->cbox.left)>>8;
	int32_t kright = (obj->pos_x+obj->animation->cbox.right)>>8;
	int32_t ktop = (obj->pos_y+obj->animation->cbox.top)>>8;
	int32_t kbottom = (obj->pos_y+obj->animation->cbox.bottom)>>8;

	int32_t leftTile = kleft>>4;
	int32_t rightTile = kright>>4;
	int32_t topTile = ktop>>4;
	int32_t bottomTile = kbottom>>4;

	int16_t tileY,tileX,tile;
	
    for(tileY = topTile-1; tileY <= bottomTile; tileY++){
        for(tileX = leftTile-1; tileX <= rightTile; tileX++){
            tile = (tileY*vk_level_width)+tileX;
            if(tileX<0||tileY<0||tileX>=vk_level_width||tileY>=vk_level_height){
                continue;
            }
			if(kright > (tileX<<4)){
				if(kleft < (tileX<<4)+(16)){
					if(kbottom > (tileY<<4)){
						if(ktop < (tileY<<4)+16){
							// Collide with tile
							switch(vk_level_tileinfo[(vk_level_data[tile]*6)+1]){
								case 1:
									// Kill keen
									VKF_keen_die(obj);
								break;
								case 6:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add the score
									VK_KeenCollectPoint(500);
									break;
								case 7:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add the score
									VK_KeenCollectPoint(100);
									break;
								case 8:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add the score
									VK_KeenCollectPoint(200);
									break;
								case 9:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add the score
									VK_KeenCollectPoint(1000);
									break;
								case 10:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add the score
									VK_KeenCollectPoint(5000);
									break;
								/*

				-2 	Masked 	14 	Whiskey
				-1 	Foreground 	15 	Raygun
				0 	Nothing 	16 	Pogo
				1 	Kills 	17 	Exit
				2 	Door 1 	18 	Key 1
				3 	Door 2 	19 	Key 2
				4 	Door 3 	20 	Key 3
				5 	Door 4 	21 	Key 4
				6 	500 points 	22 	Message box pop-up
				7 	100 points 	23 	Light switch
				8 	200 points 	24 	Teleporter
				9 	1000 points 	25 	Switch on
				10 	5000 points 	26 	Switch off
				11 	Joystick 	27 	Ankh (Keen 3)
				12 	Battery 	28 	Ammo clip (Keen 3)
				13 	Vacuum 		
				* 
								*/
							}
						}
					}
				}
			}
		}
	}
	if(kleft < 32){
		obj->pos_x = (32<<8) - obj->animation->cbox.left;
		obj->vel_x = 0;
	}
	if(kright > (vk_level_width<<4)-32){
		obj->pos_x = (((vk_level_width<<4)-48)<<8) + obj->animation->cbox.right;
		obj->vel_x = 0;
	}
	if(ktop < 32){
		obj->pos_y = (32<<8) - obj->animation->cbox.top;
		obj->vel_y = 0;
	}
	if(kbottom > (vk_level_height<<4)){
		// Kill keen
		VKF_keen_die(obj);
	}
};


// Basic collistion code for object vs level
int VK_CollideObjWLevel(vk_object *obj){
	if(obj->animation==NULL){
		return 0;
	}

	int32_t kleft = (obj->pos_x+obj->animation->cbox.left)>>8;
	int32_t kright = (obj->pos_x+obj->animation->cbox.right)>>8;
	int32_t ktop = (obj->pos_y+obj->animation->cbox.top)>>8;
	int32_t kbottom = (obj->pos_y+obj->animation->cbox.bottom)>>8;

	int32_t leftTile = kleft>>4;
	int32_t rightTile = kright>>4;
	int32_t topTile = ktop>>4;
	int32_t bottomTile = kbottom>>4;


	int16_t tileY,tileX,tile;
	/*
	obj->hit_bottom = 0;
	obj->hit_top = 0;
	obj->hit_right = 0;
	obj->hit_left = 0;
	*/
	obj->on_ground = 0;
    
    for(tileY = topTile-1; tileY <= bottomTile; tileY++){
        for(tileX = leftTile-1; tileX <= rightTile; tileX++){
            tile = (tileY*vk_level_width)+tileX;
            if(tileX<0||tileY<0||tileX>=vk_level_width||tileY>=vk_level_height){
                continue;
            }
		
			// Collide with top
			if(vk_level_tileinfo[(vk_level_data[tile]*6)+2]){
				if(kright > (tileX<<4)){
					if(kleft < (tileX<<4)+(16)){
						if(kbottom >= (tileY<<4)){
							if(kbottom <= (tileY<<4)+(obj->vel_y>>8)){
								if(obj->vel_y > 0){
									obj->pos_y = (tileY<<12)-(obj->animation->cbox.bottom);
									obj->vel_y = 0;
									obj->on_ground = vk_level_tileinfo[(vk_level_data[tile]*6)+2];
									obj->hit_bottom = 1;
									// Reset the variables
									ktop = (obj->pos_y+obj->animation->cbox.top)>>8;
									kbottom = (obj->pos_y+obj->animation->cbox.bottom)>>8;
								}
							}
						}
					}
				}
			}
			// Collide with right
			if(vk_level_tileinfo[(vk_level_data[tile]*6)+3]){
				 if(kleft < (tileX<<4)+2){
					if(kright >= (tileX<<4)){
						if(ktop < (tileY<<4)+(16)){
							if(kbottom >(tileY<<4)){
								if(obj->vel_x > 0){
									obj->pos_x = ((tileX<<12)-(16<<8))+obj->animation->cbox.left;
									obj->vel_x = 0;
									obj->hit_left = 1;
									// Reset the variables
									kleft = (obj->pos_x+obj->animation->cbox.left)>>8;
									kright = (obj->pos_x+obj->animation->cbox.right)>>8;
								}
							}
						}
					}
				}
			}
			// Collide with bottom
			if(vk_level_tileinfo[(vk_level_data[tile]*6)+4]){
				if(kright > (tileX<<4)){
					if(kleft < (tileX<<4)+(16)){
						if(ktop >= (tileY<<4)+(16)+(obj->vel_y>>8)){
							if(ktop <= (tileY<<4)+(16)){
								if(obj->vel_y < 0){
									obj->pos_y = ((tileY<<12)+(16<<8))-obj->animation->cbox.top;
									obj->vel_y = 0;
									obj->hit_top = 1;
									// Reset the variables
									ktop = (obj->pos_y+obj->animation->cbox.top)>>8;
									kbottom = (obj->pos_y+obj->animation->cbox.bottom)>>8;
								}
							}
						}
					}
				}
			}
			  // Collide with left
			if(vk_level_tileinfo[(vk_level_data[tile]*6)+5]){
				if(kright > (tileX<<4)+(16)-2){
					if(kleft <= (tileX<<4)+(16)){
						if(ktop < (tileY<<4)+(16)){
							if(kbottom > (tileY<<4)){
								if(obj->vel_x < 0){
									obj->pos_x = (tileX<<12)+obj->animation->cbox.right;
									obj->vel_x = 0;
									obj->hit_right = 1;
								}
							}
						}
					}
				}
			}
        }
    }
	return 0;
};

vk_object *VK_CreateObject(uint16_t sprite_id, uint32_t x, uint32_t y){
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
			obj->type = vko_yorp;
			obj->hitmap = 1;
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
			obj->animation = &VKA_keen_idle;
			obj->collide = &VKF_keen_collide;
			obj->think = &VKF_keen_think;
			obj->type = vko_keen;
			obj->hitmap = 1;
			VKF_keen_init(obj);
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
		
		if(obj!=NULL&&obj->s!=NULL&&obj->animation!=NULL){
			
			if(vk_oupdate_tick >= 0x4){
				if(obj->animation!=NULL){
					obj->frame_count += 1;

					if(obj->frame_count >= obj->animation->ticks){
						VK_SetObjAnimation(obj,obj->animation->next);
					}
				}
			}

			if(obj->collide!=NULL){
				obj->collide(obj);
			}
			if(obj->hitmap){
				VK_CollideObjWLevel(obj);
				if(obj->type==vko_keen){
					VK_CollideKeenWLevel(obj);
				}
			}

			if(obj->think!=NULL){
				obj->think(obj);
			}

			cx = (obj->pos_x>>8)-(vk_level_offsetx<<4);
			cy = (obj->pos_y>>8)-(vk_level_offsety<<4);
			cx -= vk_map_offsetx;
			cy -= vk_map_offsety;
			
			if(cx >= (0-obj->s->s.spr_width) && cy >= (0-obj->s->s.spr_height) && cx < (32<<3) && cy < (20<<3)){
				if(obj->gfx_needs_update){
					obj->gfx_needs_update = 0;
					VK_SetSpriteGraphics(obj->s);
				}
			}
			obj->s->x = cx;
			obj->s->y = cy;
			VK_RenderSprite(obj->s);
		}
	}
	VK_TextX = 2;
	VK_TextY = 2;
	VK_Print("X:");
	VK_TextX = 4;
	VK_Print(VK_Iota16(vk_keen_obj->pos_x));

	VK_TextX = 2;
	VK_TextY = 3;
	VK_Print("Y:");
	VK_TextX = 4;
	VK_Print(VK_Iota16(vk_keen_obj->pos_y));

	VK_TextX = 2;
	VK_TextY = 4;
	VK_Print("V1:");
	VK_TextX = 6;
	VK_Print(VK_Iota16(vk_keen_obj->var1));

	VK_TextX = 2;
	VK_TextY = 5;
	VK_Print("V3:");
	VK_TextX = 6;
	VK_Print(VK_Iota16(vk_keen_obj->var3));

	if(vk_oupdate_tick >= 0x4){
		vk_oupdate_tick = 0;
	}

};

