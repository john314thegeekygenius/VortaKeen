
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


//      Garg    =========================================

vk_obj_ani VKA_garg_idle_1,
			VKA_garg_idle_2,
			VKA_garg_idle_3,
			VKA_garg_idle_4;

vk_obj_ani VKA_garg_walk_1,
			VKA_garg_walk_2;

vk_obj_ani VKA_garg_die_1,
			VKA_garg_die_2;

// --------------------------- R  L   # Frames    Collistion Box    Next Animation
vk_obj_ani VKA_garg_idle_1 = { 0, 0, VK_2_FRAMES, { 4<<8, 4<<8, 20<<8, 32<<8}, &VKA_garg_idle_2 };
vk_obj_ani VKA_garg_idle_2 = { 1, 1, VK_2_FRAMES, { 4<<8, 4<<8, 20<<8, 32<<8}, &VKA_garg_idle_3 };
vk_obj_ani VKA_garg_idle_3 = { 2, 2, VK_2_FRAMES, { 4<<8, 4<<8, 20<<8, 32<<8}, &VKA_garg_idle_4 };
vk_obj_ani VKA_garg_idle_4 = { 3, 3, VK_2_FRAMES, { 4<<8, 4<<8, 20<<8, 32<<8}, &VKA_garg_idle_1 };

vk_obj_ani VKA_garg_walk_1 = { 4, 6, VK_2_FRAMES, { 4<<8, 4<<8, 20<<8, 32<<8}, &VKA_garg_walk_2 };
vk_obj_ani VKA_garg_walk_2 = { 5, 7, VK_2_FRAMES, { 4<<8, 4<<8, 20<<8, 32<<8}, &VKA_garg_walk_1 };

vk_obj_ani VKA_garg_die_1 = { 8, 8, VK_4_FRAMES, { 4<<8, 4<<8, 20<<8, 32<<8}, &VKA_garg_die_2 };
vk_obj_ani VKA_garg_die_2 = { 9, 9, VK_2_FRAMES, { 4<<8, 4<<8, 20<<8, 32<<8}, &VKA_garg_die_2 };



int VKF_garg_init(vk_object *obj){
	// Is charging
	obj->var3 = 0;
	
	// Set look delay
	obj->var2 = (VK_2_FRAMES )<<2;

	// Look countdown
	obj->var1 = 0;
	return 0;
};

int VKF_garg_collide(vk_object *obj, vk_object *cobj){
	if(obj->animation == &VKA_garg_die_1 || obj->animation == &VKA_garg_die_2){
		return 0;
	}

	// Interact with shots
	if(cobj->type == vko_shot_friendly || cobj->type == vko_shot_deadly){
		// Kill the shot
		cobj->var1 = 1;
		
		// Kill the garg
		obj->vel_x = 0;
		VK_SetObjAnimation(obj,&VKA_garg_die_1);
		VK_PlaySound(VKS_GARGSCREAM);
	}
	
	
	// Interact with keen
	if(cobj->type == vko_keen){
		// Kill Keen
		VKF_keen_die(cobj);
	}
	return 0;
};


int VKF_garg_think(vk_object *obj){
	// vk_keen_obj is defined in VK_ObjectsEngine.c
	if(!(obj->animation == &VKA_garg_die_1 || obj->animation == &VKA_garg_die_2)){

		if( (obj->animation == &VKA_garg_walk_1) ||
			(obj->animation == &VKA_garg_walk_2)){

			if(obj->hit_left){
				obj->vel_x = -0x80;
				obj->facing = 0;

				obj->hit_left = 0;

				// Stop the garg from charging forever
				obj->var3 = 0;
			}
			if(obj->hit_right){
				obj->vel_x = 0x80;
				obj->facing = 1;

				obj->hit_right = 0;

				// Stop the garg from charging forever
				obj->var3 = 0;
			}
		}
	
		if(obj->var1 == 0){
			if( (obj->animation == &VKA_garg_walk_1) ||
				(obj->animation == &VKA_garg_walk_2)){
					
				if(obj->var3==0){

					// Set look delay
					obj->var2 = (VK_2_FRAMES )<<2;
					
					VK_SetObjAnimation(obj,&VKA_garg_idle_1);
					
					// Stop the garg from gliding away!
					obj->vel_x = 0x00;
				}
			}
		}else{
			obj->var1 -= 1;
		}
		
		
		// Wait for animation to be correct
		if( (obj->animation == &VKA_garg_idle_1)){
			// Wait for look timer to run out
			if(obj->var2==0){
							
				obj->var3 = 0;
				
				// Charge at keen if it can see him
				if(vk_keen_obj->pos_x > obj->pos_x-(128<<8)){
					if(vk_keen_obj->pos_x < obj->pos_x+(128<<8)){
						if(vk_keen_obj->pos_y+(32<<8) > obj->pos_y-(16<<8)){
							if(vk_keen_obj->pos_y < obj->pos_y+(32<<8)){
								obj->var3 = 1;
							}
						}
					}
				}

				
				// Garg needs to move toward keen
				if(vk_keen_obj->pos_x < obj->pos_x){
					// Yorp can't move toward keen, so go opposite direction
					if(obj->hit_right){
						obj->vel_x = 0x80;
						obj->facing = 1;
					}else{
						if(obj->var3 ){
							obj->vel_x = -0x200;
						}else{
							obj->vel_x = -0x80;
						}
						obj->facing = 0;
					}
				}
				if(vk_keen_obj->pos_x > obj->pos_x){
					// Garg can't move toward keen, so go opposite direction
					if(obj->hit_left){
						obj->vel_x = -0x80;
						obj->facing = 0;
					}else{
						if(obj->var3 ){
							obj->vel_x = 0x200;
						}else{
							obj->vel_x = 0x80;
						}
						obj->facing = 1;
					}
				}
				// Count down to next look animation
				obj->var1 = VK_GetRNG();
	
				// Set animation to walking
				VK_SetObjAnimation(obj,&VKA_garg_walk_1);

				obj->hit_right = 0;
				obj->hit_left = 0;

			}else{
				obj->var2 -= 1;
			}
		}

	}else{
		obj->vel_x = 0;
	}
	
	// Move the garg
	obj->pos_x += obj->vel_x;
	obj->pos_y += obj->vel_y;
	
	obj->vel_y += 0x10;
	
	return 0;
};
