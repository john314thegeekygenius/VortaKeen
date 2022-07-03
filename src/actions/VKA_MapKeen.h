
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


// Worldmap Keen =====================================

vk_obj_ani VKA_mapkeen_idle_h,
			VKA_mapkeen_idle_v;

vk_obj_ani VKA_mapkeen_walk_h1,
			VKA_mapkeen_walk_h2,
			VKA_mapkeen_walk_h3,
			VKA_mapkeen_walk_h4,
			VKA_mapkeen_walk_v1,
			VKA_mapkeen_walk_v2,
			VKA_mapkeen_walk_v3,
			VKA_mapkeen_walk_v4;

vk_obj_ani VKA_mapkeen_idle_h = { 0, 8, VK_2_FRAMES, { 2<<8, 1<<8, 10<<8, 16<<8}, &VKA_mapkeen_idle_h };
vk_obj_ani VKA_mapkeen_idle_v = { 4, 12, VK_2_FRAMES, { 2<<8, 1<<8, 10<<8, 16<<8}, &VKA_mapkeen_idle_v };

// Right / Left
vk_obj_ani VKA_mapkeen_walk_h1 = { 1, 9, VK_2_FRAMES, { 2<<8, 1<<8, 10<<8, 16<<8}, &VKA_mapkeen_walk_h2 };
vk_obj_ani VKA_mapkeen_walk_h2 = { 2, 10, VK_2_FRAMES, { 2<<8, 1<<8, 10<<8, 16<<8}, &VKA_mapkeen_walk_h3 };
vk_obj_ani VKA_mapkeen_walk_h3 = { 3, 11, VK_2_FRAMES, { 2<<8, 1<<8, 10<<8, 16<<8}, &VKA_mapkeen_walk_h4 };
vk_obj_ani VKA_mapkeen_walk_h4 = { 2, 10, VK_2_FRAMES, { 2<<8, 1<<8, 10<<8, 16<<8}, &VKA_mapkeen_walk_h1 };
// Up / Down
vk_obj_ani VKA_mapkeen_walk_v1 = { 5, 13, VK_2_FRAMES, { 2<<8, 1<<8, 10<<8, 16<<8}, &VKA_mapkeen_walk_v2 };
vk_obj_ani VKA_mapkeen_walk_v2 = { 6, 14, VK_2_FRAMES, { 2<<8, 1<<8, 10<<8, 16<<8}, &VKA_mapkeen_walk_v3 };
vk_obj_ani VKA_mapkeen_walk_v3 = { 7, 15, VK_2_FRAMES, { 2<<8, 1<<8, 10<<8, 16<<8}, &VKA_mapkeen_walk_v4 };
vk_obj_ani VKA_mapkeen_walk_v4 = { 6, 14, VK_2_FRAMES, { 2<<8, 1<<8, 10<<8, 16<<8}, &VKA_mapkeen_walk_v1 };



int VKF_mapkeen_init(vk_object *obj){
	// current tile
	obj->var1 = 0;

	return 0;
};

int VKF_mapkeen_collide(vk_object *obj, vk_object *cobj){
	return 0;
};

int VKF_mapkeen_input(vk_object *obj){
	
	// Grab the input :D
			
	if(VK_CheckButton(GBA_BUTTON_RIGHT)){
		// Move right
		obj->vel_x = 0x200;
		obj->facing = 1;
		if(!(VK_CheckButton(GBA_BUTTON_UP)) && !(VK_CheckButton(GBA_BUTTON_DOWN))){
			if(obj->animation == &VKA_mapkeen_walk_v1||obj->animation == &VKA_mapkeen_walk_v2){
				VK_SetObjAnimation(obj,&VKA_mapkeen_walk_h1);
			}
			if(obj->animation == &VKA_mapkeen_idle_v||obj->animation == &VKA_mapkeen_idle_h){
				VK_SetObjAnimation(obj,&VKA_mapkeen_walk_h1);
			}
		}

	}else if(VK_CheckButton(GBA_BUTTON_LEFT)){
		// Move left
		obj->vel_x = -0x200;
		obj->facing = 0;

		if(!(VK_CheckButton(GBA_BUTTON_UP)) && !(VK_CheckButton(GBA_BUTTON_DOWN))){
			if(obj->animation == &VKA_mapkeen_walk_v1||obj->animation == &VKA_mapkeen_walk_v2){
				VK_SetObjAnimation(obj,&VKA_mapkeen_walk_h1);
			}
			if(obj->animation == &VKA_mapkeen_idle_v||obj->animation == &VKA_mapkeen_idle_h){
				VK_SetObjAnimation(obj,&VKA_mapkeen_walk_h1);
			}
		}

	}else{
		if(obj->animation == &VKA_mapkeen_walk_h1||obj->animation == &VKA_mapkeen_walk_h2){
			VK_SetObjAnimation(obj,&VKA_mapkeen_idle_h);
		}
	}

	if(VK_CheckButton(GBA_BUTTON_DOWN)){
		// Move down
		obj->vel_y = 0x200;
		obj->facing = 1;
		if(obj->animation == &VKA_mapkeen_walk_h1||obj->animation == &VKA_mapkeen_walk_h2){
			VK_SetObjAnimation(obj,&VKA_mapkeen_walk_v1);
		}
		if(obj->animation == &VKA_mapkeen_idle_v||obj->animation == &VKA_mapkeen_idle_h){
			VK_SetObjAnimation(obj,&VKA_mapkeen_walk_v1);
		}

	}else if(VK_CheckButton(GBA_BUTTON_UP)){
		// Move up
		obj->vel_y = -0x200;
		obj->facing = 0;

		if(obj->animation == &VKA_mapkeen_walk_h1||obj->animation == &VKA_mapkeen_walk_h2){
			VK_SetObjAnimation(obj,&VKA_mapkeen_walk_v1);
		}
		if(obj->animation == &VKA_mapkeen_idle_v||obj->animation == &VKA_mapkeen_idle_h){
			VK_SetObjAnimation(obj,&VKA_mapkeen_walk_v1);
		}
	}else{
		if(obj->animation == &VKA_mapkeen_walk_v1||obj->animation == &VKA_mapkeen_walk_v2){
			VK_SetObjAnimation(obj,&VKA_mapkeen_idle_v);
		}
	}

	
	if(VK_CheckButton(GBA_BUTTON_A)){
		if(obj->var1>0){
			uint16_t tile = vk_level_map[obj->var1];
			if(tile > 0 && tile <= 16){
				// Store current position
				vk_engine_gstate.posX = obj->pos_x;
				vk_engine_gstate.posY = obj->pos_y;
				vk_engine_gstate.viewportX = vk_viewport_x;
				vk_engine_gstate.viewportY = vk_viewport_y;
				// Enter level
				vk_engine_gstate.level_to_load = tile;
				VK_PlaySound(2);
				VK_FadeOut();
				// We just faded out
				vk_engine_gstate.faded = 1;
				while(VK_SoundDone());
				return 0;
			}
			
			if(tile == 0x14){
				// Ship dialog
				VK_MissingParts();
				return 0;
			}
			
			if((tile & 0x20)==0x20){
				uint16_t dest_teleporter = (tile&0x3) - 1; // Subtract 1 because we need to index at 0
				uint16_t teleporter_type = (tile&0xC);
				uint16_t teleport_tile_off = obj->var1;
				
				vk_engine_gstate.teleporting = 0x40;
				vk_engine_gstate.teleporter_pos = obj->var1-(vk_level_height*vk_level_width);
				vk_engine_gstate.teleporter = vk_level_data[vk_engine_gstate.teleporter_pos];

				VK_PlaySound(VKS_TELEPORTSND);
				
				// Animate the teleporter
				if(teleporter_type){
					// It's ice
					vk_level_data[vk_engine_gstate.teleporter_pos] = vk_num_of_tiles-4;
				}else{
					vk_level_data[vk_engine_gstate.teleporter_pos] = vk_num_of_tiles-8;
				}

				// Add the animation
				ck_update_positions[ck_number_of_updates][0] = (vk_engine_gstate.teleporter_pos%vk_level_width);
				ck_update_positions[ck_number_of_updates][1] = (vk_engine_gstate.teleporter_pos/vk_level_width);
				ck_number_of_updates += 1;

				VK_ForceLevelUpdate();

				// VK_TELEPORT_DEST is defined in VK_LevelEngine.c
				obj->pos_x = VK_TELEPORT_DEST[dest_teleporter<<1];
				obj->pos_y = VK_TELEPORT_DEST[(dest_teleporter<<1)+1];
			}
		}
	}
	return 0;
};

int VKF_mapkeen_think(vk_object *obj){
	
	if(obj->hit_left||obj->hit_right||obj->hit_top||obj->hit_bottom){
		// Play the block sound
		if(obj->animation == &VKA_mapkeen_walk_h2||obj->animation == &VKA_mapkeen_walk_h4 ||
		obj->animation == &VKA_mapkeen_walk_v2||obj->animation == &VKA_mapkeen_walk_v4){
			VK_PlaySound(VKS_WLDBLOCKSND);
		}
	}else{
		if(obj->animation == &VKA_mapkeen_walk_h1|| obj->animation == &VKA_mapkeen_walk_h3||
			obj->animation == &VKA_mapkeen_walk_v1 || obj->animation == &VKA_mapkeen_walk_v3){
			// Play the walk sound
			VK_PlaySound(VKS_WLDWALKSND);
		}
	}

	// Move keen
	
	// Stop keen from going crazy
	if(obj->vel_y < -0x200){
		obj->vel_y = -0x200;
	}
	if(obj->vel_y > 0x200){
		obj->vel_y = 0x200;
	}
	if(obj->vel_x>0x200){
		obj->vel_x = 0x200;
	}
	if(obj->vel_x<-0x200){
		obj->vel_x = -0x200;
	}

	obj->pos_x += obj->vel_x;
	obj->pos_y += obj->vel_y;
	
	if(obj->vel_x>0){
		obj->vel_x -= 0x80;
		if(obj->vel_x<0){
			obj->vel_x = 0;
		}
	}
	if(obj->vel_x<0){
		obj->vel_x += 0x80;
		if(obj->vel_x>0){
			obj->vel_x = 0;
		}
	}
	if(obj->vel_y>0){
		obj->vel_y -= 0x80;
		if(obj->vel_y<0){
			obj->vel_y = 0;
		}
	}
	if(obj->vel_y<0){
		obj->vel_y += 0x80;
		if(obj->vel_y>0){
			obj->vel_y = 0;
		}
	}
	
		
	obj->hit_bottom = 0;
	obj->hit_top = 0;
	obj->hit_right = 0;
	obj->hit_left = 0;

	return 0;
};

