
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


//      Yorp    =========================================

vk_obj_ani VKA_yorp_idle_1,
			VKA_yorp_idle_2,
			VKA_yorp_idle_3,
			VKA_yorp_idle_4;

vk_obj_ani VKA_yorp_walk_1,
			VKA_yorp_walk_2;
vk_obj_ani VKA_yorp_hit_1,
			VKA_yorp_hit_2;
vk_obj_ani VKA_yorp_die_1,
			VKA_yorp_die_2;

// --------------------------- R  L   # Frames    Collistion Box    Next Animation
vk_obj_ani VKA_yorp_idle_1 = { 0, 0, VK_2_FRAMES, { 4<<8, 4<<8, 12<<8, 24<<8}, &VKA_yorp_idle_2 };
vk_obj_ani VKA_yorp_idle_2 = { 1, 1, VK_2_FRAMES, { 4<<8, 4<<8, 12<<8, 24<<8}, &VKA_yorp_idle_3 };
vk_obj_ani VKA_yorp_idle_3 = { 2, 2, VK_2_FRAMES, { 4<<8, 4<<8, 12<<8, 24<<8}, &VKA_yorp_idle_4 };
vk_obj_ani VKA_yorp_idle_4 = { 3, 3, VK_2_FRAMES, { 4<<8, 4<<8, 12<<8, 24<<8}, &VKA_yorp_idle_1 };

vk_obj_ani VKA_yorp_walk_1 = { 4, 6, VK_2_FRAMES, { 4<<8, 4<<8, 12<<8, 24<<8}, &VKA_yorp_walk_2 };
vk_obj_ani VKA_yorp_walk_2 = { 5, 7, VK_2_FRAMES, { 4<<8, 4<<8, 12<<8, 24<<8}, &VKA_yorp_walk_1 };

vk_obj_ani VKA_yorp_hit_1 = { 8, 8, VK_2_FRAMES, { 4<<8, 4<<8, 12<<8, 24<<8}, &VKA_yorp_hit_2 };
vk_obj_ani VKA_yorp_hit_2 = { 9, 9, VK_2_FRAMES, { 4<<8, 4<<8, 12<<8, 24<<8}, &VKA_yorp_hit_1 };

vk_obj_ani VKA_yorp_die_1 = { 10, 10, VK_2_FRAMES, { 4<<8, 4<<8, 12<<8, 24<<8}, &VKA_yorp_die_2 };
vk_obj_ani VKA_yorp_die_2 = { 11, 11, VK_2_FRAMES, { 4<<8, 4<<8, 12<<8, 24<<8}, &VKA_yorp_die_2 };



int VKF_yorp_init(vk_object *obj){
	// Set next jump time
	obj->var3 = VK_GetRNG();
	// Hit timer
	obj->var4 = 0x00;
	return 0;
};

int VKF_yorp_collide(vk_object *obj, vk_object *cobj){
	if(obj->animation == &VKA_yorp_die_1 || obj->animation == &VKA_yorp_die_2){
		return 0;
	}

	// Interact with shots
	if(cobj->type == vko_shot_friendly || cobj->type == vko_shot_deadly){
		// Kill the shot
		cobj->var1 = 1;
		
		// Kill the yorp
		VK_SetObjAnimation(obj,&VKA_yorp_die_1);
		VK_PlaySound(VKS_YORPSCREAM);
	}
	
	
	// Interact with keen
	if(obj->var4==0x00){
		if(cobj->pos_y+cobj->animation->cbox.bottom > obj->pos_y+obj->animation->cbox.top){
			if(cobj->vel_y > 0x40){
				// Keen hit yorp on the head!
				VK_SetObjAnimation(obj,&VKA_yorp_hit_1);
				obj->var4 = 0x140;
				obj->vel_x = 0;
				// Play sound
				VK_PlaySound(VKS_YORPBOPSND);
				return 1;
			}
		}
		// Bounce Keen arround
		if(cobj->pos_x+cobj->animation->cbox.right < obj->pos_x+obj->animation->cbox.right){
			cobj->vel_x -= 0x400; // ZOOOOOOM!
			// Play sound
			VK_PlaySound(VKS_YORPBUMPSND);
			return 1;
		}
		if(cobj->pos_x+cobj->animation->cbox.left > obj->pos_x+obj->animation->cbox.left){
			cobj->vel_x += 0x400; // ZOOOOOOM!
			// Play sound
			VK_PlaySound(VKS_YORPBUMPSND);
			return 1;
		}
	}	
	return 0;
};

// Yorp Looks,
// Yorp heads toward keen
// Yorp waits rnd time
// Yorp Looks.

// Look is 8 frames
// start animation on frame yorp is looking

// Yorp waits rnd time
// Yorp jumps up to 16 pixels (rng[]>>3)?

// Yorp goes up / down for about 6 seconds when hit
// 25 frames of down up + 1 extra up frame at begining


int VKF_yorp_think(vk_object *obj){
	// vk_keen_obj is defined in VK_ObjectsEngine.c
	if(!(obj->animation == &VKA_yorp_die_1 || obj->animation == &VKA_yorp_die_2)){

		if(obj->hit_left||obj->hit_right){
			// Make yorp think
			obj->var1 = 0;
		}
		
		if(obj->var4==1){
			// Make yorp think
			obj->var2 = (VK_2_FRAMES )<<2;
			VK_SetObjAnimation(obj,&VKA_yorp_idle_1);
		}
		if(obj->var4 > 0){
			obj->var4 -= 1;
			
		}else{

			
			if(obj->var1 == 0){
				if( (obj->animation == &VKA_yorp_walk_1) ||
					(obj->animation == &VKA_yorp_walk_2)){

					// Set look delay
					obj->var2 = (VK_2_FRAMES )<<2;
					
					// Start the animation on a custom frame depending on direction
					if(obj->facing == 0){
						VK_SetObjAnimation(obj,&VKA_yorp_idle_2);
					}else{
						VK_SetObjAnimation(obj,&VKA_yorp_idle_1);
					}
					// Stop the yorp from gliding away!
					obj->vel_x = 0x00;
				}
			}else{
				obj->var1 -= 1;
			}
			
			
			// Wait for animation to be correct
			if( (obj->animation == &VKA_yorp_idle_1)){
				// Wait for look timer to run out
				if(obj->var2==0){
					// Yorp needs to move toward keen
					if(vk_keen_obj->pos_x < obj->pos_x){
						// Yorp can't move toward keen, so go opposite direction
						if(obj->hit_right){
							obj->vel_x = 0x80;
							obj->facing = 1;
						}else{
							obj->vel_x = -0x80;
							obj->facing = 0;
						}
					}
					if(vk_keen_obj->pos_x > obj->pos_x){
						// Yorp can't move toward keen, so go opposite direction
						if(obj->hit_left){
							obj->vel_x = -0x80;
							obj->facing = 0;
						}else{
							obj->vel_x = 0x80;
							obj->facing = 1;
						}
					}
					// Count down to next look animation
					obj->var1 = VK_GetRNG();

					// Set animation to walking
					VK_SetObjAnimation(obj,&VKA_yorp_walk_1);

					obj->hit_right = 0;
					obj->hit_left = 0;

				}else{
					obj->var2 -= 1;
				}
			}

			// Count down to next jump
			if( (obj->animation == &VKA_yorp_walk_1) ||
				(obj->animation == &VKA_yorp_walk_2)){
				if(obj->var3 == 0){
					// Make the yorp jump
					obj->vel_y = -(VK_GetRNG()+0x40); // Max jump height of 1 block?
					// Set next jump time
					obj->var3 = VK_GetRNG();
				}
			}
			if(obj->var3 > 0){
				obj->var3 -= 1;
			}
		}
	}

	// Cap the velocity
	if(obj->vel_x>0x80){
		obj->vel_x = 0x80;
	}
	if(obj->vel_x<-0x80){
		obj->vel_x = -0x80;
	}
	
	// Move the yorp
	obj->pos_x += obj->vel_x;
	obj->pos_y += obj->vel_y;
	
	obj->vel_y += 0x10;
	
	return 0;
};
