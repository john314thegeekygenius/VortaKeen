
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


//      Ice Cube    =========================================

vk_obj_ani VKA_icecube;
vk_obj_ani VKA_iceshard;

// --------------------------- R  L   # Frames    Collistion Box    Next Animation
vk_obj_ani VKA_icecube = { 0, 0, VK_2_FRAMES, { 4<<8, 4<<8, 15<<8, 15<<8}, &VKA_icecube };

vk_obj_ani VKA_iceshard = { 0, 0, 0x10, { 0<<8, 0<<8, 8<<8, 8<<8}, NULL };



int VKF_iceshard_collide(vk_object *obj, vk_object *cobj){
	// Don't Do Anything!
	return 0;
};


int VKF_iceshard_think(vk_object *obj){

	obj->pos_x += obj->vel_x;
	obj->pos_y += obj->vel_y;

	return 0;
};

int VKF_icecube_collide(vk_object *obj, vk_object *cobj){
	// Interact with keen 

	if(cobj->type == vko_keen){
		// Remove the cube
		VK_SetObjAnimation(obj,NULL);
		// Freeze Keen!
		VK_SetObjAnimation(cobj,&VKA_keen_ice_1);
		// Shoot keen into the sky
		cobj->vel_y = obj->vel_y;
		cobj->vel_x = obj->vel_x;
		// Set the timer
		cobj->var2 = 0x00;
		// Set this to 0
		cobj->var1 = 0;
		// Play FREEZE sound
		VK_PlaySound(VKS_KEENCICLE);
	}
	return 0;
};


int VKF_icecube_think(vk_object *obj){

	if(obj->hit_top || obj->hit_bottom || obj->hit_left || obj->hit_right){
		// Shatter the ice cube

		// Spawn the cubes
		vk_object * cobj;

		// Up Right
		cobj = VK_CreateObject(6,obj->pos_x,obj->pos_y);
		cobj->s = VK_CreateSprite(7);
		VK_SetObjAnimation(cobj,&VKA_iceshard);
		cobj->collide = &VKF_iceshard_collide;
		cobj->think = &VKF_iceshard_think;
		cobj->type = vko_iceshard;
		cobj->hitmap = 0; // don't collide
		// Set the velocities
		cobj->vel_x = 0x200;
		cobj->vel_y = -0x200;

		// Down Right
		cobj = VK_CreateObject(6,obj->pos_x,obj->pos_y);
		cobj->s = VK_CreateSprite(7);
		VK_SetObjAnimation(cobj,&VKA_iceshard);
		cobj->collide = &VKF_iceshard_collide;
		cobj->think = &VKF_iceshard_think;
		cobj->type = vko_iceshard;
		cobj->hitmap = 0; // don't collide
		// Set the velocities
		cobj->vel_x = 0x200;
		cobj->vel_y = 0x200;
		
		// Up Left
		cobj = VK_CreateObject(6,obj->pos_x,obj->pos_y);
		cobj->s = VK_CreateSprite(7);
		VK_SetObjAnimation(cobj,&VKA_iceshard);
		cobj->collide = &VKF_iceshard_collide;
		cobj->think = &VKF_iceshard_think;
		cobj->type = vko_iceshard;
		cobj->hitmap = 0; // don't collide
		// Set the velocities
		cobj->vel_x = -0x200;
		cobj->vel_y = -0x200;

		// Down Left
		cobj = VK_CreateObject(6,obj->pos_x,obj->pos_y);
		cobj->s = VK_CreateSprite(7);
		VK_SetObjAnimation(cobj,&VKA_iceshard);
		cobj->collide = &VKF_iceshard_collide;
		cobj->think = &VKF_iceshard_think;
		cobj->type = vko_iceshard;
		cobj->hitmap = 0; // don't collide
		// Set the velocities
		cobj->vel_x = -0x200;
		cobj->vel_y = 0x200;


		VK_PlaySound(VKS_CHUNKSMASH);

		VK_SetObjAnimation(obj,NULL);
	}

	obj->pos_x += obj->vel_x;
	obj->pos_y += obj->vel_y;

	return 0;
};

