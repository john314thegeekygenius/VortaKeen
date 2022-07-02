
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


//      Butler    =========================================

vk_obj_ani VKA_butler_walk_1,
			VKA_butler_walk_2,
			VKA_butler_walk_3,
			VKA_butler_walk_4;

vk_obj_ani VKA_butler_turn_1,
			VKA_butler_turn_2;


// --------------------------- R  L   # Frames    Collistion Box    Next Animation
vk_obj_ani VKA_butler_walk_1 = { 0, 4, VK_2_FRAMES, { 3<<8, 3<<8, 13<<8, 16<<8}, &VKA_butler_walk_2 };
vk_obj_ani VKA_butler_walk_2 = { 1, 5, VK_2_FRAMES, { 3<<8, 3<<8, 13<<8, 16<<8}, &VKA_butler_walk_3 };
vk_obj_ani VKA_butler_walk_3 = { 2, 6, VK_2_FRAMES, { 3<<8, 3<<8, 13<<8, 16<<8}, &VKA_butler_walk_4 };
vk_obj_ani VKA_butler_walk_4 = { 3, 7, VK_2_FRAMES, { 3<<8, 3<<8, 13<<8, 16<<8}, &VKA_butler_walk_1 };

vk_obj_ani VKA_butler_turn_1 = { 8, 8, VK_2_FRAMES, { 3<<8, 3<<8, 13<<8, 16<<8}, &VKA_butler_turn_2 };
vk_obj_ani VKA_butler_turn_2 = { 9, 9, VK_2_FRAMES, { 3<<8, 3<<8, 13<<8, 16<<8}, &VKA_butler_walk_1 };



int VKF_butler_init(vk_object *obj){
	return 0;
};

int VKF_butler_collide(vk_object *obj, vk_object *cobj){
	
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


int VKF_butler_think(vk_object *obj){
	// vk_keen_obj is defined in VK_ObjectsEngine.c

	if(obj->hit_left){
		obj->facing = 0;
		obj->hit_left = 0;
		
		// Stop the can
		obj->vel_x = 0;

		// Make the can turn arround
		VK_SetObjAnimation(obj,&VKA_butler_turn_1);

	}
	if(obj->hit_right){
		obj->facing = 1;

		obj->hit_right = 0;

		// Stop the can
		obj->vel_x = 0;

		// Make the can turn arround
		VK_SetObjAnimation(obj,&VKA_butler_turn_1);
	}

	if( (obj->animation == &VKA_butler_walk_1) || 
		(obj->animation == &VKA_butler_walk_2) ||
		(obj->animation == &VKA_butler_walk_3) ||
		(obj->animation == &VKA_butler_walk_4) ){
		int edge = VK_ObjAtEdge(obj);
		if(obj->facing==0){
			if(edge==-1){
				obj->facing = 1;

				// Stop the can
				obj->vel_x = 0x00;

				// Make the can turn arround
				VK_SetObjAnimation(obj,&VKA_butler_turn_1);
			}
		}else{
			if(edge==1){
				obj->facing = 0;

				// Stop the can
				obj->vel_x = 0x00;

				// Make the tank can arround
				VK_SetObjAnimation(obj,&VKA_butler_turn_1);
			}
		}
	}
	
	if(obj->animation == &VKA_butler_walk_1){
		if(obj->facing==1){
			obj->vel_x = 0x80;
		}
		if(obj->facing==0){
			obj->vel_x = -0x80;
		}
	}
	
	// Move the can
	obj->pos_x += obj->vel_x;
	obj->pos_y += obj->vel_y;
	
	obj->vel_y += 0x10;
	
	return 0;
};
