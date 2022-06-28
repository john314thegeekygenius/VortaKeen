
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


// Exported from EXE via Chocolate Keen
// All 3 episodes use the same RNG table
const unsigned char vk_rng_table[256] = {
	0, 8, 109, 220, 222, 241, 149, 107, 75, 248, 254, 140, 16, 66, 74, 21, 211, 47, 80, 242, 154, 27, 205, 128, 161, 89, 77, 36, 95, 110, 85, 48, 212, 140, 211, 249, 22, 79, 200, 50, 28, 188, 52, 140, 202, 120, 68, 145, 62, 70, 184, 190, 91, 197, 152, 224, 149, 104, 25, 178, 252, 182, 202, 182, 141, 197, 4, 81, 181, 242, 145, 42, 39, 227, 156, 198, 225, 193, 219, 93, 122, 175, 249, 0, 175, 143, 70, 239, 46, 246, 163, 53, 163, 109, 168, 135, 2, 235, 25, 92, 20, 145, 138, 77, 69, 166, 78, 176, 173, 212, 166, 113, 94, 161, 41, 50, 239, 49, 111, 164, 70, 60, 2, 37, 171, 75, 136, 156, 11, 56, 42, 146, 138, 229, 73, 146, 77, 61, 98, 196, 135, 106, 63, 197, 195, 86, 96, 203, 113, 101, 170, 247, 181, 113, 80, 250, 108, 7, 255, 237, 129, 226, 79, 107, 112, 166, 103, 241, 24, 223, 239, 120, 198, 58, 60, 82, 128, 3, 184, 66, 143, 224, 145, 224, 81, 206, 163, 45, 63, 90, 168, 114, 59, 33, 159, 95, 28, 139, 123, 98, 125, 196, 15, 70, 194, 253, 54, 14, 109, 226, 71, 17, 161, 93, 186, 87, 244, 138, 20, 52, 123, 251, 26, 36, 17, 46, 52, 231, 232, 76, 31, 221, 84, 37, 216, 165, 212, 106, 197, 242, 98, 43, 39, 175, 254, 145, 190, 84, 118, 222, 187, 136, 120, 163, 236
};

unsigned char rng_count = 0;

unsigned char VK_GetRNG() {
	rng_count++;
	return vk_rng_table[rng_count];
};


const uint16_t VK_1_FRAMES = 0x1;
const uint16_t VK_2_FRAMES = 0x2;
const uint16_t VK_4_FRAMES = 0x4;
const uint16_t VK_8_FRAMES = 0x8;



//        Keen =====================================

vk_obj_ani VKA_keen_idle;

vk_obj_ani VKA_keen_walk_1,
			VKA_keen_walk_2,
			VKA_keen_walk_3,
			VKA_keen_walk_4;

vk_obj_ani VKA_keen_prejump_1,
			VKA_keen_prejump_2,
			VKA_keen_prejump_3,
			VKA_keen_prejump_4,
			VKA_keen_jump,
			VKA_keen_fall;

vk_obj_ani VKA_keen_shoot,
			VKA_keen_die_1,
			VKA_keen_die_2,
			VKA_keen_pogo_1,
			VKA_keen_pogo_2,
			VKA_keen_ice_1,
			VKA_keen_ice_2,
			VKA_keen_confused,
			VKA_keen_ice_break;


vk_obj_ani VKA_keen_idle = { 0, 4, VK_2_FRAMES, { 4<<8, 1<<8, 12<<8, 24<<8}, &VKA_keen_idle };

vk_obj_ani VKA_keen_walk_1 = { 1, 5, VK_2_FRAMES, { 4<<8, 1<<8, 12<<8, 24<<8}, &VKA_keen_walk_2 };
vk_obj_ani VKA_keen_walk_2 = { 2, 6, VK_2_FRAMES, { 4<<8, 1<<8, 12<<8, 24<<8}, &VKA_keen_walk_3 };
vk_obj_ani VKA_keen_walk_3 = { 3, 7, VK_2_FRAMES, { 4<<8, 1<<8, 12<<8, 24<<8}, &VKA_keen_walk_4 };
vk_obj_ani VKA_keen_walk_4 = { 2, 6, VK_2_FRAMES, { 4<<8, 1<<8, 12<<8, 24<<8}, &VKA_keen_walk_1 };

vk_obj_ani VKA_keen_prejump_1 = { 8, 14, VK_1_FRAMES, { 4<<8, 1<<8, 12<<8, 24<<8}, &VKA_keen_prejump_2 };
vk_obj_ani VKA_keen_prejump_2 = { 9, 15, VK_1_FRAMES, { 4<<8, 1<<8, 12<<8, 24<<8}, &VKA_keen_prejump_3 };
vk_obj_ani VKA_keen_prejump_3 = {10, 16, VK_1_FRAMES, { 4<<8, 1<<8, 12<<8, 24<<8}, &VKA_keen_prejump_4 };
vk_obj_ani VKA_keen_prejump_4 = {11, 17, VK_1_FRAMES, { 4<<8, 1<<8, 12<<8, 24<<8}, &VKA_keen_jump };

vk_obj_ani VKA_keen_jump = {12, 18, VK_1_FRAMES, { 4<<8, 1<<8, 12<<8, 24<<8}, &VKA_keen_fall };
vk_obj_ani VKA_keen_fall = {13, 19, VK_1_FRAMES, { 4<<8, 1<<8, 12<<8, 24<<8}, &VKA_keen_fall };

vk_obj_ani VKA_keen_shoot = {20, 21, VK_2_FRAMES, { 4<<8, 1<<8, 12<<8, 24<<8}, &VKA_keen_shoot };

vk_obj_ani VKA_keen_die_1 = {22, 22, VK_2_FRAMES, { 4<<8, 16<<8, 12<<8, 24<<8}, &VKA_keen_die_2 };
vk_obj_ani VKA_keen_die_2 = {23, 23, VK_2_FRAMES, { 4<<8, 16<<8, 12<<8, 24<<8}, &VKA_keen_die_1 };

vk_obj_ani VKA_keen_pogo_1 = {25, 27, VK_2_FRAMES, { 4<<8, 0<<8, 12<<8, 24<<8}, &VKA_keen_pogo_2 };
vk_obj_ani VKA_keen_pogo_2 = {24, 26, VK_2_FRAMES, { 4<<8, 0<<8, 12<<8, 24<<8}, &VKA_keen_pogo_2 };

vk_obj_ani VKA_keen_ice_1 = {28, 28, VK_2_FRAMES, { 4<<8, 1<<8, 12<<8, 24<<8}, &VKA_keen_ice_2 };
vk_obj_ani VKA_keen_ice_2 = {29, 29, VK_2_FRAMES, { 4<<8, 1<<8, 12<<8, 24<<8}, &VKA_keen_ice_1 };

vk_obj_ani VKA_keen_confused = {30, 30, VK_2_FRAMES, { 4<<8, 1<<8, 12<<8, 24<<8}, &VKA_keen_confused };
vk_obj_ani VKA_keen_ice_break = {31, 31, VK_2_FRAMES, { 4<<8, 1<<8, 12<<8, 24<<8}, &VKA_keen_idle };



int VKF_keen_init(vk_object *obj){
	// Set is jumping
	obj->var1 = 0x00;
	// Death counter
	obj->var2 = 0x00;
	// Set jump time
	obj->var3 = 0x00;
	return 0;
};

int VKF_keen_collide(vk_object *obj, vk_object *cobj){
	return 0;
};

int VKF_keen_die(vk_object *obj){
	if(obj->var2==0){
		obj->animation = &VKA_keen_die_1;
		// Set timer
		obj->var2 = 0x40;
		// Set position
		obj->var3 = 0x40;
		// Don't collide with the map any more
		obj->hitmap = 0;
		// Play sound
		VK_PlaySound(VKS_KEENDIESND);
	}

	return 0;
};

int VKF_keen_input(vk_object *obj){
	if(obj->animation == &VKA_keen_die_1 || obj->animation == &VKA_keen_die_2){
		return 0;
	}
	
	// Grab the input :D
	
	if(VK_CheckButton(GBA_BUTTON_RIGHT)){
		// Move right
		obj->vel_x += 0x40;
		if(obj->vel_x>0){
			obj->facing = 1;
		}
		if(obj->on_ground){
			if(obj->animation == &VKA_keen_idle){
				VK_SetObjAnimation(obj,&VKA_keen_walk_1);
			}
		}
	}else if(VK_CheckButton(GBA_BUTTON_LEFT)){
		// Move left
		obj->vel_x -= 0x40;
		if(obj->vel_x<0){
			obj->facing = 0;
		}
		if(obj->on_ground){
			if(obj->animation == &VKA_keen_idle){
				VK_SetObjAnimation(obj,&VKA_keen_walk_1);
			}
		}
	}else{
		if(obj->animation == &VKA_keen_walk_1 ||
			obj->animation == &VKA_keen_walk_2 ||
			obj->animation == &VKA_keen_walk_3 ||
			obj->animation == &VKA_keen_walk_4 ){
			VK_SetObjAnimation(obj,&VKA_keen_idle);
		}
	}
	if(VK_CheckButton(GBA_BUTTON_A)){
		// Jump
		if(obj->on_ground){
			if(obj->var1==0){
				if(obj->animation == &VKA_keen_idle ||
				obj->animation == &VKA_keen_walk_1 ||
				obj->animation == &VKA_keen_walk_2 ||
				obj->animation == &VKA_keen_walk_3 ||
				obj->animation == &VKA_keen_walk_4 ){
					VK_SetObjAnimation(obj,&VKA_keen_prejump_1);
				}
				
				if(obj->animation == &VKA_keen_pogo_2){
					VK_SetObjAnimation(obj,&VKA_keen_pogo_1);
				}
				obj->var1 = 1;
			}
		}
	}
	if(VK_ButtonUp() == (GBA_BUTTON_LSHOLDER)){
		// Pogo
		if(obj->animation==&VKA_keen_pogo_1 || obj->animation==&VKA_keen_pogo_2){
			VK_SetObjAnimation(obj,&VKA_keen_fall);
			obj->var1 = 0; // Reset this to 0
		}else{
			if(obj->on_ground){
				if(obj->var1==0){
					VK_SetObjAnimation(obj,&VKA_keen_pogo_1);
					obj->var1 = 1;
					VK_PlaySound(VKS_KEENPOGOSND);
				}
			}else{
				VK_SetObjAnimation(obj,&VKA_keen_pogo_2);
			}
		}
	}
	if(obj->animation == &VKA_keen_pogo_2){
		if(obj->var1){
			obj->var1 = 0;
			obj->var3 = 0x08;
			obj->vel_y = -0x280;
			if(VK_CheckButton(GBA_BUTTON_A)){
				obj->vel_y -= 0x20;
			}
			// Play jump sound
			VK_PlaySound(VKS_POGOJUMPSND);
		}else{
			// Only bounce if we land on ground
			if(obj->on_ground){
				if(obj->var1==0){
					VK_SetObjAnimation(obj,&VKA_keen_pogo_1);
					obj->var1 = 1;
					VK_PlaySound(VKS_KEENPOGOSND);
				}
			}
		}
	}
	
	if(obj->animation == &VKA_keen_jump){
		if(obj->var1){
			obj->var1 = 0;
			obj->var3 = 0x08;
			obj->vel_y = -0x80;
			if(VK_CheckButton(GBA_BUTTON_A)){
				obj->vel_y -= 0x100;
			}
			// Play jump sound
			VK_PlaySound(VKS_KEENJUMPSND);
		}
	}
	if(obj->var3>0){
		if(VK_CheckButton(GBA_BUTTON_A)){
			obj->vel_y -= 0x20;
		}
		obj->var3 -= 1;
	}
	return 0;
};

int VKF_keen_think(vk_object *obj){
	if(obj->animation == &VKA_keen_die_1 || obj->animation == &VKA_keen_die_2){
		if(obj->var2==1){
			// Fly away
			if(obj->var3){
				obj->pos_x -= 0x400;
				obj->pos_y -= 0x400;
				obj->var3 -= 0x1;
			}else{
				// Go back to world map
				VK_ReturnToWorldmap();
			}
		}else{
			obj->var2 -= 1;
		}
		return 0;
	}
	
	// vk_keen_obj is defined in VK_ObjectsEngine.c
	if(obj->hit_left||obj->hit_right){
		if(obj->animation == &VKA_keen_walk_1 ||
			obj->animation == &VKA_keen_walk_2 ||
			obj->animation == &VKA_keen_walk_3 ||
			obj->animation == &VKA_keen_walk_4 ){
			// Play the block sound
			if(obj->animation == &VKA_keen_walk_1){
				VK_PlaySound(VKS_KEENBLOKSND);
			}
			VK_SetObjAnimation(obj,&VKA_keen_idle);
		}
	}
	if(obj->hit_bottom || obj->on_ground){
		if(obj->animation == &VKA_keen_fall){
			VK_SetObjAnimation(obj,&VKA_keen_idle);		
			VK_PlaySound(VKS_KEENLANDSND); // play land sound
		}
	}else{
		if(obj->animation == &VKA_keen_idle ||
			obj->animation == &VKA_keen_walk_1 ||
			obj->animation == &VKA_keen_walk_2 ||
			obj->animation == &VKA_keen_walk_3 ||
			obj->animation == &VKA_keen_walk_4 ){
			
			VK_SetObjAnimation(obj,&VKA_keen_fall);		
			VK_PlaySound(VKS_PLUMMETSND); // play fall sound
		}
	}
	
	if(obj->animation == &VKA_keen_walk_1 ){
		// Play the walk sound
		VK_PlaySound(VKS_KEENWALKSND);
	}
	if(obj->animation == &VKA_keen_walk_3){
		// Play the walk sound
		VK_PlaySound(VKS_KEENBLOKSND);
	}

	// Move keen
	if(obj->var1==0){
		obj->pos_x += obj->vel_x;
		obj->pos_y += obj->vel_y;
		
		if(obj->vel_x>0x200){
			obj->vel_x = 0x200;
		}
		if(obj->vel_x<-0x200){
			obj->vel_x = -0x200;
		}
		
		obj->vel_y += 0x10;
		
		// Stop keen from going crazy
		if(obj->vel_y > 0x400){
			obj->vel_y = 0x400;
		}
		
		if(obj->animation == &VKA_keen_pogo_1 || obj->animation == &VKA_keen_pogo_2){
			if(obj->vel_x>0x40){
				obj->vel_x -= 0x10;
				// Stop the velocity from going too far
				if(obj->vel_x<0x40){
					obj->vel_x = 0x40;
				}
			}
			if(obj->vel_x<-0x40){
				obj->vel_x += 0x10;
				// Stop the velocity from going too far
				if(obj->vel_x>-0x40){
					obj->vel_x = -0x40;
				}
			}
		}else{
			if(obj->on_ground==0){
				if(obj->vel_x>0){
					obj->vel_x -= 0x10;
					// Stop the velocity from going too far
					if(obj->vel_x<0){
						obj->vel_x = 0;
					}
				}
				if(obj->vel_x<0){
					obj->vel_x += 0x10;
					// Stop the velocity from going too far
					if(obj->vel_x>0){
						obj->vel_x = 0;
					}
				}
			}
			if(obj->on_ground==1){
				if(obj->vel_x>0){
					obj->vel_x -= 0x30;
					// Stop the velocity from going too far
					if(obj->vel_x<0){
						obj->vel_x = 0;
					}
				}
				if(obj->vel_x<0){
					obj->vel_x += 0x30;
					// Stop the velocity from going too far
					if(obj->vel_x>0){
						obj->vel_x = 0;
					}
				}
			}
		}
	}
	
	obj->hit_bottom = 0;
	obj->hit_top = 0;
	obj->hit_right = 0;
	obj->hit_left = 0;

	return 0;
};




//       Enemeys =========================================

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


/*

void CVort1_think_yorp_walk() {
    if (!temp_sprite.velY) {
        if ((unsigned)(CVort_get_random()) < sprite_sync)
            temp_sprite.velY = -CVort_calc_jump_height(0x80);
        else if ((unsigned)(CVort_get_random()) < sprite_sync) {
            temp_sprite.think = &CVort1_think_yorp_look;
            temp_sprite.time = 0;
        }
    }
    if (temp_sprite.velX > 0)
        temp_sprite.frame = 0x34;
    else
        temp_sprite.frame = 0x36;
    temp_sprite.frame += (CVort_ptr_engine_getTicks() >> 4)&1;
    CVort_do_fall();
    int16_t currDelta = CVort_compute_sprite_delta();
    if (currDelta & 4)
        temp_sprite.velX = -0x3C;
    if (currDelta & 1)
        temp_sprite.velX = 0x3C;
}

void CVort1_think_yorp_look() {
    temp_sprite.velX = 0;
    temp_sprite.frame = ((CVort_ptr_engine_getTicks() >> 5)&3) + 0x30;
    temp_sprite.time += sprite_sync;
    if (temp_sprite.time >= 200) {
        if (temp_sprite.posX < sprites[0].posX)
            temp_sprite.velX = 0x3C;
        else
            temp_sprite.velX = -0x3C;
        temp_sprite.think = &CVort1_think_yorp_walk;
    }
    CVort_do_fall();
    CVort_compute_sprite_delta();
}
*/

int VKF_yorp_init(vk_object *obj){
	// Set next jump time
	obj->var3 = VK_GetRNG();
	return 0;
};

int VKF_yorp_collide(vk_object *obj, vk_object *cobj){
	return 0;
};

int VKF_yorp_think(vk_object *obj){
	// vk_keen_obj is defined in VK_ObjectsEngine.c

	if(obj->hit_left||obj->hit_right){
		// Make yorp think
		obj->var1 = 0;
	}
	
	if(obj->var1 == 0){
		if( (obj->animation == &VKA_yorp_walk_1) ||
			(obj->animation == &VKA_yorp_walk_2)){

			// Set look delay
			obj->var2 = (VK_2_FRAMES * 2);
			
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
	
	
	// Move the yorp
	obj->pos_x += obj->vel_x;
	obj->pos_y += obj->vel_y;
	
	if(obj->vel_x>0x80){
		obj->vel_x = 0x80;
	}
	if(obj->vel_x<-0x80){
		obj->vel_x = -0x80;
	}
	
	obj->vel_y += 0x10;
	
	/*
	if(obj->pos_y > 80<<8){
		obj->pos_y = 80<<8;
		obj->vel_y = 0;
	}*/
	
	return 0;
};
