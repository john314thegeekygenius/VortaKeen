
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


//      Robot Tank    =========================================

vk_obj_ani VKA_robotank_roll_1,
			VKA_robotank_roll_2,
			VKA_robotank_roll_3,
			VKA_robotank_roll_4;

vk_obj_ani VKA_robotank_turn_1,
			VKA_robotank_turn_2;


// --------------------------- R  L   # Frames    Collistion Box    Next Animation
vk_obj_ani VKA_robotank_roll_1 = { 0, 4, VK_2_FRAMES, { 0<<8, 0<<8, 16<<8, 24<<8}, &VKA_robotank_roll_2 };
vk_obj_ani VKA_robotank_roll_2 = { 1, 5, VK_2_FRAMES, { 0<<8, 0<<8, 16<<8, 24<<8}, &VKA_robotank_roll_3 };
vk_obj_ani VKA_robotank_roll_3 = { 2, 6, VK_2_FRAMES, { 0<<8, 0<<8, 16<<8, 24<<8}, &VKA_robotank_roll_4 };
vk_obj_ani VKA_robotank_roll_4 = { 3, 7, VK_2_FRAMES, { 0<<8, 0<<8, 16<<8, 24<<8}, &VKA_robotank_roll_1 };

vk_obj_ani VKA_robotank_turn_1 = { 8, 8, VK_2_FRAMES, { 0<<8, 0<<8, 16<<8, 24<<8}, &VKA_robotank_turn_2 };
vk_obj_ani VKA_robotank_turn_2 = { 9, 9, VK_2_FRAMES, { 0<<8, 0<<8, 16<<8, 24<<8}, &VKA_robotank_roll_1 };



int VKF_robotank_init(vk_object *obj){
	// Shoot countdown
	obj->var1 = VK_GetRNG();
	// Wait for shot time
	obj->var2 = 0x60;
	return 0;
};

int VKF_robotank_collide(vk_object *obj, vk_object *cobj){
	
	// Interact with keen
	if(cobj->type == vko_keen){
		// Bounce Keen arround
		if(obj->facing == 0){
			if(cobj->pos_x+cobj->animation->cbox.right < obj->pos_x+obj->animation->cbox.right){
				cobj->vel_x -= 0x800; // ZOOOOOOM!
				// Play sound
				VK_PlaySound(VKS_YORPBUMPSND);
				return 1;
			}
		}
		if(obj->facing == 1){
			if(cobj->pos_x+cobj->animation->cbox.left > obj->pos_x+obj->animation->cbox.left){
				cobj->vel_x += 0x800; // ZOOOOOOM!
				// Play sound
				VK_PlaySound(VKS_YORPBUMPSND);
				return 1;
			}
		}
	}
	return 0;
};


int VKF_robotank_think(vk_object *obj){
	// vk_keen_obj is defined in VK_ObjectsEngine.c

	if(obj->hit_left){
		obj->facing = 0;
		obj->hit_left = 0;
		
		// Stop the tank
		obj->vel_x = 0;

		// Make the tank turn arround
		VK_SetObjAnimation(obj,&VKA_robotank_turn_1);

	}
	if(obj->hit_right){
		obj->facing = 1;

		obj->hit_right = 0;

		// Stop the tank
		obj->vel_x = 0;

		// Make the tank turn arround
		VK_SetObjAnimation(obj,&VKA_robotank_turn_1);
	}

	// Count down to shoot
	if(obj->var1){
		obj->var1 -= 1;


		if( (obj->animation == &VKA_robotank_roll_1) || 
			(obj->animation == &VKA_robotank_roll_2) ||
			(obj->animation == &VKA_robotank_roll_3) ||
			(obj->animation == &VKA_robotank_roll_4) ){
			int edge = VK_ObjAtEdge(obj);
			if(obj->facing==0){
				if(edge==-1){
					obj->facing = 1;

					// Stop the tank
					obj->vel_x = 0x00;

					// Make the tank turn arround
					VK_SetObjAnimation(obj,&VKA_robotank_turn_1);
				}
			}else{
				if(edge==1){
					obj->facing = 0;

					// Stop the tank
					obj->vel_x = 0x00;

					// Make the tank turn arround
					VK_SetObjAnimation(obj,&VKA_robotank_turn_1);
				}
			}
		}
		
		if(obj->animation == &VKA_robotank_roll_1){
			if(obj->facing==1){
				obj->vel_x = 0x80;
			}
			if(obj->facing==0){
				obj->vel_x = -0x80;
			}
		}
		
	}else{
		// Stop the tank
		obj->vel_x = 0x00;
		
		// Count down the timer
		if(obj->var2){
			obj->var2 -= 1;
			if(obj->var2==0x30){
				// Spawn shot
				VK_SpawnShot(obj->pos_x,obj->pos_y,obj->facing,1);
				// Play sound if need keen
				if(obj->pos_y > vk_keen_obj->pos_y-(256<<8) && obj->pos_y < vk_keen_obj->pos_y+(256<<8)){
					if(obj->pos_x > vk_keen_obj->pos_x-(256<<8) && obj->pos_x < vk_keen_obj->pos_x+(256<<8)){
					// Play sound
					VK_PlaySound(VKS_TANKFIRE);
					}
				}
			}
		}else{
			// Count down to next look animation
			obj->var1 = VK_GetRNG()+0x20;
			
			// Reset timer 
			obj->var2 = 0x60;
			
			// Turn
			VK_SetObjAnimation(obj,&VKA_robotank_turn_1);
		}
	}

	// Move the robot tank
	obj->pos_x += obj->vel_x;
	obj->pos_y += obj->vel_y;
	
	obj->vel_y += 0x10;
	
	return 0;
};
