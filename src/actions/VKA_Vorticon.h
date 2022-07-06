
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


//      Vorticon    =========================================

#define VORTICON_SHOTS 4
#define VORTICON_COMMANDER_SHOTS 105
#define VORTICON_COMMANDER_LEVEL 16
// You could define the commander level as: 16 || vk_level_id==XX
// where XX is another level, thus alowing for him to be in more than one level

vk_obj_ani VKA_vorticon_walk_1,
			VKA_vorticon_walk_2,
			VKA_vorticon_walk_3,
			VKA_vorticon_walk_4;

vk_obj_ani VKA_vorticon_idle_1,
			VKA_vorticon_idle_2;

vk_obj_ani VKA_vorticon_jump;

vk_obj_ani VKA_vorticon_die_1,
			VKA_vorticon_die_2,
			VKA_vorticon_die_3,
			VKA_vorticon_die_4,
			VKA_vorticon_die_5,
			VKA_vorticon_die_6;

// --------------------------- R  L   # Frames    Collistion Box    Next Animation
vk_obj_ani VKA_vorticon_walk_1 = { 4, 0, VK_2_FRAMES, { 2<<8, 1<<8, 22<<8, 32<<8}, &VKA_vorticon_walk_2 };
vk_obj_ani VKA_vorticon_walk_2 = { 5, 1, VK_2_FRAMES, { 2<<8, 1<<8, 22<<8, 32<<8}, &VKA_vorticon_walk_3 };
vk_obj_ani VKA_vorticon_walk_3 = { 6, 2, VK_2_FRAMES, { 2<<8, 1<<8, 22<<8, 32<<8}, &VKA_vorticon_walk_4 };
vk_obj_ani VKA_vorticon_walk_4 = { 7, 3, VK_2_FRAMES, { 2<<8, 1<<8, 22<<8, 32<<8}, &VKA_vorticon_walk_1 };

vk_obj_ani VKA_vorticon_idle_1 = { 8, 8, VK_2_FRAMES, { 2<<8, 1<<8, 22<<8, 32<<8}, &VKA_vorticon_idle_2 };
vk_obj_ani VKA_vorticon_idle_2 = { 9, 9, VK_2_FRAMES, { 2<<8, 1<<8, 22<<8, 32<<8}, &VKA_vorticon_idle_1 };

vk_obj_ani VKA_vorticon_jump = { 10, 11, VK_2_FRAMES, { 2<<8, 1<<8, 22<<8, 32<<8}, &VKA_vorticon_jump };

vk_obj_ani VKA_vorticon_die_1 = { 12, 12, VK_4_FRAMES, { 2<<8, 1<<8, 22<<8, 32<<8}, &VKA_vorticon_die_2 };
vk_obj_ani VKA_vorticon_die_2 = { 13, 13, VK_4_FRAMES, { 2<<8, 1<<8, 22<<8, 32<<8}, &VKA_vorticon_die_3 };
vk_obj_ani VKA_vorticon_die_3 = { 14, 14, VK_4_FRAMES, { 2<<8, 1<<8, 22<<8, 32<<8}, &VKA_vorticon_die_4 };
vk_obj_ani VKA_vorticon_die_4 = { 15, 15, VK_4_FRAMES, { 2<<8, 1<<8, 22<<8, 32<<8}, &VKA_vorticon_die_5 };
vk_obj_ani VKA_vorticon_die_5 = { 16, 16, VK_4_FRAMES, { 2<<8, 1<<8, 22<<8, 32<<8}, &VKA_vorticon_die_6 };
vk_obj_ani VKA_vorticon_die_6 = { 17, 17, VK_4_FRAMES, { 2<<8, 1<<8, 22<<8, 32<<8}, &VKA_vorticon_die_6 };




int VKF_vorticon_init(vk_object *obj){
	// Health
	if(obj->type == vko_vorticon){
		obj->var1 = VORTICON_SHOTS;
	}
	if(obj->type == vko_vorticon_commander){
		obj->var1 = VORTICON_COMMANDER_SHOTS;
	}
	// Think frames
	obj->var2 = 0x00;
	// Set next jump time
	obj->var3 = VK_GetRNG();
	// Next think time
	obj->var4 = 0x00;
	return 0;
};

int VKF_vorticon_collide(vk_object *obj, vk_object *cobj){
	if(obj->var1==0){
		return 0;
	}

	// Interact with shots
	if(cobj->type == vko_shot_friendly || cobj->type == vko_shot_deadly){
		// Kill the shot
		cobj->var1 = 1;

		if(obj->var1){
			obj->var1 -= 1;
		}

		if(obj->var1==0){
			// Kill the vorticon
			obj->vel_x = 0;
			VK_SetObjAnimation(obj,&VKA_vorticon_die_1);
			VK_PlaySound(VKS_VORTSCREAM);
		}
	}
	
	
	// Interact with keen
	if(cobj->type == vko_keen){
		// Kill Keen
		VKF_keen_die(cobj);
	}

	return 0;
};

int VKF_vorticon_think(vk_object *obj){
	// vk_keen_obj is defined in VK_ObjectsEngine.c
	if(obj->var1){

		if( (obj->animation == &VKA_vorticon_walk_1) ||
			(obj->animation == &VKA_vorticon_walk_2) ||
			(obj->animation == &VKA_vorticon_walk_3) ||
			(obj->animation == &VKA_vorticon_walk_4) ){

			if(obj->hit_left){
				obj->vel_x = -0xC0;
				obj->facing = 0;

				obj->hit_left = 0;

			}
			if(obj->hit_right){
				obj->vel_x = 0xC0;
				obj->facing = 1;

				obj->hit_right = 0;

			}
		}
		
		if(obj->on_ground){
			if(obj->animation == &VKA_vorticon_jump){
				// Make vorticon think
				obj->var4 = 0;
			}

			if(obj->var4==0){
				if(obj->var2==0){
					obj->var2 = (VK_4_FRAMES )<<2;
					VK_SetObjAnimation(obj,&VKA_vorticon_idle_1);
					// Stop the vorticon
					obj->vel_x = 0;
				}else{
					obj->var2 -= 1;
				}
			}else{
				obj->var4 -= 1;
			}
		}
		
		if(obj->var2 == 0){
			if((obj->animation == &VKA_vorticon_idle_1) || 
				(obj->animation == &VKA_vorticon_idle_2)){
				// Move toward keen
				if(vk_keen_obj->pos_x < obj->pos_x){
					// Vorticon can't move toward keen, so go opposite direction
					if(obj->hit_right){
						obj->vel_x = 0xC0;
						obj->facing = 1;
					}else{
						obj->vel_x = -0xC0;
						obj->facing = 0;
					}
				}
				if(vk_keen_obj->pos_x > obj->pos_x){
					// Vorticon can't move toward keen, so go opposite direction
					if(obj->hit_left){
						obj->vel_x = -0xC0;
						obj->facing = 0;
					}else{
						obj->vel_x = 0xC0;
						obj->facing = 1;
					}
				}

				// Count down to next look animation
				obj->var4 = VK_GetRNG();

				// Set animation to walking
				VK_SetObjAnimation(obj,&VKA_vorticon_walk_1);

				obj->hit_right = 0;
				obj->hit_left = 0;

			}
		}

		// Count down to next jump
		if( (obj->animation == &VKA_vorticon_walk_1) ||
			(obj->animation == &VKA_vorticon_walk_2) ||
			(obj->animation == &VKA_vorticon_walk_3) ||
			(obj->animation == &VKA_vorticon_walk_4) ){
			if(obj->var3 == 0){
				// Make the vorticon jump
				obj->vel_y = -(VK_GetRNG()+0x40)<<1; // Max jump height of 3 blocks?
				// Set next jump time
				obj->var3 = VK_GetRNG();
				// Set animation to jumping
				VK_SetObjAnimation(obj,&VKA_vorticon_jump);
			}
		}
		if(obj->var3 > 0){
			obj->var3 -= 1;
		}
	}else{
		obj->vel_x = 0;
	}
	
	// Move the vorticon
	obj->pos_x += obj->vel_x;
	obj->pos_y += obj->vel_y;
	
	obj->vel_y += 0x10;
	
	return 0;
};
