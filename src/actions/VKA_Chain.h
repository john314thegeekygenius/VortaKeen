
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


//      Chain    =========================================

vk_obj_ani VKA_chain;

// --------------------------- R  L   # Frames    Collistion Box    Next Animation
vk_obj_ani VKA_chain = { 0, 0, VK_2_FRAMES, { 0<<8, 0<<8, 16<<8, 16<<8}, &VKA_chain };

int VKF_chain_init(vk_object *obj){
	// Was shot
	obj->var1 = 0;
	// Animation active / Y
	obj->var2 = 0;
	// Animation tick
	obj->var3 = 0;

};

int VKF_chain_collide(vk_object *obj, vk_object *cobj){
	int i;
	if(obj->var1 == 0){

		// Interact with shots
		if(cobj->type == vko_shot_friendly || cobj->type == vko_shot_deadly){
			// Kill the shot
			cobj->var1 = 1;
			// Set some vars
			obj->var1 = 1;
			obj->var2 = 0;
			obj->var3 = 0;
			// Kill the vorticon commander
			for(i=0;i<vk_num_of_objects;i++){
				vk_object * dobj = &vk_level_objects[i];
				if(dobj->type==vko_vorticon_commander){
					dobj->var1 = 0;
					VK_SetObjAnimation(dobj,&VKA_vorticon_die_1);
					VK_PlaySound(VKS_VORTSCREAM);
				}
			}
		}
	}
	return 0;
};


int VKF_chain_think(vk_object *obj){
	int x,y, sy;
	if(obj->var1){
		if(obj->var2 < 4){
			obj->var3 += 1;
			if(obj->var3 > 0x8){
				obj->var3 = 0;
				obj->var2 += 1;

				sy = (obj->pos_y>>12) + (obj->var2);
				for(y = sy+2; y >= sy; y--){
					for(x = (obj->pos_x>>12)-4; x <= (obj->pos_x>>12)+4; x++){
						vk_level_data[(y*vk_level_width)+x] = vk_level_data[((y-1)*vk_level_width)+x];
					}
				}
				VK_ForceLevelUpdate();

			}
		}
	}
	return 0;
};
