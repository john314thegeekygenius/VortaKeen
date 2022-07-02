
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
	// Interacted with map
	obj->click_map = 0x00;
	// Set is jumping
	obj->var1 = 0x00;
	// Death counter
	obj->var2 = 0x00;
	// Set jump time
	obj->var3 = 0x00;
	// Set is shooting
	obj->var4 = 0x00;
	return 0;
};

int VKF_keen_collide(vk_object *obj, vk_object *cobj){
	if(cobj->type == vko_shot_deadly){
		// Kill keen
		VKF_keen_die(obj);
		// Kill the shot
		cobj->var1 = 1;
	}
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
		// Remove a life
		vk_engine_gstate.numLives -= 1;
		// Play sound
		VK_PlaySound(VKS_KEENDIESND);
	}

	return 0;
};

int VKF_keen_input(vk_object *obj){
	// Don't take input if we can't hit the map???
	if(obj->hitmap==0){
		return 0;
	}
	
	// Grab the input :D
	
	if(vk_keen_input[0]){
		// Move right
		if(obj->on_ground!=3){
			if(obj->var4==0){
				// Unless on ice
				if(obj->on_ground==0){
					obj->vel_x += 0x20;
				}else{
					obj->vel_x += 0x40;
				}
				if(obj->vel_x>0){
					obj->facing = 1;
				}
				if(obj->on_ground){
					if(obj->animation == &VKA_keen_idle){
						VK_SetObjAnimation(obj,&VKA_keen_walk_1);
					}
				}
			}
		}else{
			if(obj->var4==0){
				if(obj->var1==0){
					if(obj->vel_x==0){
						obj->vel_x = 0x40;
						obj->facing = 1;
					}
					VK_SetObjAnimation(obj,&VKA_keen_idle);
				}
			}
		}
	}else if(vk_keen_input[1]){
		// Move left
		if(obj->on_ground!=3){
			if(obj->var4==0){
				// Unless on ice
				if(obj->on_ground==0){
					obj->vel_x -= 0x20;
				}else{
					obj->vel_x -= 0x40;
				}
				if(obj->vel_x<0){
					obj->facing = 0;
				}
				if(obj->on_ground){
					if(obj->animation == &VKA_keen_idle){
						VK_SetObjAnimation(obj,&VKA_keen_walk_1);
					}
				}
			}
		}else{
			if(obj->var4==0){
				if(obj->var1==0){
					if(obj->vel_x==0){
						obj->vel_x = -0x40;
						obj->facing = 0;
					}
					VK_SetObjAnimation(obj,&VKA_keen_idle);
				}
			}
		}
	}else{
		if(obj->animation == &VKA_keen_walk_1 ||
			obj->animation == &VKA_keen_walk_2 ||
			obj->animation == &VKA_keen_walk_3 ||
			obj->animation == &VKA_keen_walk_4 ){
				if(obj->vel_x==0x00){
					VK_SetObjAnimation(obj,&VKA_keen_idle);
				}
		}
	}
	if(vk_keen_input[4]){
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
	
	if(vk_keen_input[5]){
		// Shoot
		if(obj->var4 == 0){
			if(vk_engine_gstate.ammo){
				// Spawn shot
				VK_SpawnShot(obj->pos_x,obj->pos_y,obj->facing,0);
				// Remove ammo
				vk_engine_gstate.ammo -= 1;
				// Play sound
				VK_PlaySound(VKS_KEENFIRESND);
			}else{
				// Play sound
				VK_PlaySound(VKS_GUNCLICK);
			}
		}
		if(obj->var4==0 || obj->var4>0x1E){
			obj->var4 = 0x20;
		}
	}
	
	if(obj->var4){
		// Always set the animation to shooting
		VK_SetObjAnimation(obj,&VKA_keen_shoot);
		// Set is jumping
		obj->var1 = 0x00;
		// Set jump time
		obj->var3 = 0x00;

		obj->var4 -= 1;
	}
	
	if(obj->click_map==0){
		if(vk_keen_input[6]){
			if(vk_engine_gstate.gotPogo ){
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
			}else{
				// Bug?
				if(obj->on_ground){
					obj->vel_x = 0x00;
				}
			}
		}
	}
		
	if(obj->animation == &VKA_keen_pogo_2){
		if(obj->var1){
			obj->var1 = 0;
			obj->var3 = 0x0A;
			obj->vel_y = -0x380;
			if(VK_CheckButton(GBA_BUTTON_A)){
				obj->vel_y -= 0x80;
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
				obj->vel_y -= 0x140;
			}
			// Play jump sound
			VK_PlaySound(VKS_KEENJUMPSND);
		}
	}
	if(obj->var3>0){
		if(VK_CheckButton(GBA_BUTTON_A)){
			obj->vel_y -= 0x40;
		}
		obj->var3 -= 1;
	}
	return 0;
};

int VKF_keen_think(vk_object *obj){
	
	if(vk_engine_gstate.finished_level==1){
		obj->animation = &VKA_keen_walk_1;
		obj->facing = 1;
		vk_engine_gstate.finished_level = 2;
		obj->var4 = 0;
		// Don't collide with the map any more
		obj->hitmap = 0;
	}
	if(vk_engine_gstate.finished_level==2){
		obj->pos_x += 0x60;
		obj->var4 += 1;
		if(obj->var4 > 0x80){
			VK_ReturnToWorldmap();
			vk_engine_gstate.finished_level = 0;
		}
		return 0;
	}

	if(obj->animation == &VKA_keen_die_1 || obj->animation == &VKA_keen_die_2){
		if(obj->var2==1){
			// Fly away
			if(obj->var3){
				obj->pos_x -= 0x400;
				obj->pos_y -= 0x400;
				obj->var3 -= 0x1;
			}else{
				if(vk_engine_gstate.numLives == 0){
					VK_QuitGame();
					return 0;
				}else{
					// Go back to world map
					VK_ReturnToWorldmap();
					return 0;
				}
			}
		}else{
			obj->var2 -= 1;
		}
		return 0;
	}

	if(obj->hit_top ){
		VK_PlaySound(VKS_BUMPHEADSND);
	}
	
	// vk_keen_obj is defined in VK_ObjectsEngine.c
	if(obj->hit_left||obj->hit_right){
		if(obj->animation == &VKA_keen_walk_1 ||
			obj->animation == &VKA_keen_walk_2 ||
			obj->animation == &VKA_keen_walk_3 ||
			obj->animation == &VKA_keen_walk_4 ){
			// Play the block sound
			if(obj->animation == &VKA_keen_walk_2||obj->animation == &VKA_keen_walk_3||obj->animation == &VKA_keen_walk_4){
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
		if(obj->animation == &VKA_keen_shoot){
			if(obj->var4 == 0){
				VK_SetObjAnimation(obj,&VKA_keen_idle);
			}
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

		if(obj->animation == &VKA_keen_shoot){
			if(obj->var4 == 0){
				VK_SetObjAnimation(obj,&VKA_keen_fall);		
				VK_PlaySound(VKS_PLUMMETSND); // play fall sound
			}
		}
	}
	
	if(obj->animation == &VKA_keen_walk_1 || obj->animation == &VKA_keen_walk_3 ){
		// Play the walk sound
		VK_PlaySound(VKS_KEENWALKSND);
	}

	// Move keen
	if(obj->var1==0){
		
		// Stop keen from going crazy
		if(obj->vel_y < -0x400){
			obj->vel_y = -0x400;
		}
		if(obj->vel_y > 0x400){
			obj->vel_y = 0x400;
		}
		if(obj->vel_x>0x200){
			obj->vel_x = 0x200;
		}
		if(obj->vel_x<-0x200){
			obj->vel_x = -0x200;
		}

		obj->pos_x += obj->vel_x;
		obj->pos_y += obj->vel_y;
		
		
		obj->vel_y += 0x20;
		
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
			// Bug?
			if(VK_CheckButton(GBA_BUTTON_DOWN)){
				// Act like ice
				if(obj->on_ground){
					obj->on_ground = 2;
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
			if(obj->on_ground==2){
				
				if(obj->vel_x > 0x00 ){
					if(obj->vel_x > 0x80){
						obj->vel_x -= 0x20;
						// Stop the velocity from going too far
						if(obj->vel_x<0x80){
							obj->vel_x = 0x80;
						}
					}
				}

				if(obj->vel_x < 0x00 ){
					if(obj->vel_x < -0x80){
						obj->vel_x += 0x20;
						// Stop the velocity from going too far
						if(obj->vel_x>-0x80){
							obj->vel_x = -0x80;
						}
					}
				}
				if(obj->vel_x!=0x00){
					if(obj->animation == &VKA_keen_idle){
						VK_SetObjAnimation(obj,&VKA_keen_walk_1);
					}
				}
			}
			if(obj->on_ground==3){
				if(obj->vel_x > 0 ){
					obj->vel_x = 0x200;
				}
				if(obj->vel_x < 0){
					obj->vel_x = -0x200;
				}
				if(obj->animation == &VKA_keen_walk_1||
					obj->animation == &VKA_keen_walk_2||
					obj->animation == &VKA_keen_walk_3||
					obj->animation == &VKA_keen_walk_4){
					VK_SetObjAnimation(obj,&VKA_keen_idle);
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

