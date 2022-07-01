
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


//      Shot    =========================================

vk_obj_ani VKA_shot_keen,
			VKA_shot_enemy;

// --------------------------- R  L   # Frames    Collistion Box    Next Animation
vk_obj_ani VKA_shot_keen 		  = { 0, 0, VK_2_FRAMES, { 0<<8, 0<<8, 16<<8, 8<<8}, &VKA_shot_keen };
vk_obj_ani VKA_shot_enemy 		  = { 1, 1, VK_2_FRAMES, { 0<<8, 0<<8, 16<<8, 8<<8}, &VKA_shot_enemy };
vk_obj_ani VKA_shot_zap  = { 0, 0, VK_4_FRAMES, { 0<<8, 0<<8, 16<<8, 16<<8}, NULL };
vk_obj_ani VKA_shot_zot = { 1, 1, VK_4_FRAMES, { 0<<8, 0<<8, 16<<8, 16<<8}, NULL };


int VKF_shot_collide(vk_object *obj, vk_object *cobj){
	// We don't handle collisions here
	return 0;
};

int VKF_shot_think(vk_object *obj){
	if(obj->hit_left||obj->hit_right||obj->var1){
		if(obj->animation==&VKA_shot_keen){
			// Stop the shot
			obj->vel_x = 0x00;
			// Spawn Zap Zot pic
			vk_object *zapzotptr = VK_CreateObject(251,obj->pos_x,obj->pos_y-(4<<8));
			if(VK_GetRNG()>0x80){
				VK_SetObjAnimation(zapzotptr,&VKA_shot_zot);
			}
			// Kill the shot
			VK_SetObjAnimation(obj,NULL);
			// Play the sound
			VK_PlaySound(VKS_SHOTHIT);
		}
	}

	// Move the shot
	obj->pos_x += obj->vel_x;
	
	// HACK: You can make shots fall to the ground with this
//	obj->pos_y += obj->vel_y;
//	obj->vel_y += 0x10;
	
	return 0;
};
