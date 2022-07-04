
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


//      Expression    =========================================

vk_obj_ani VKA_expression;

// --------------------------- R  L   # Frames    Collistion Box    Next Animation
vk_obj_ani VKA_expression = { 1, 0, VK_1_FRAMES, { 0<<8, 0<<8, 16<<8, 8<<8}, &VKA_expression };

int VKF_expression_init(vk_object *obj){
	// Animation function
	obj->var2 = 0;
	// Animation tick
	obj->var3 = 0;
	// Face right
	obj->facing = 1;
};

int VKF_expression_collide(vk_object *obj, vk_object *cobj){
	// Never collides with anything
	return 0;
};


int VKF_expression_think(vk_object *obj){
	
	return 0;
};
