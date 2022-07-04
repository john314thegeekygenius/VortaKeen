
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


//      End Ship    =========================================

vk_obj_ani VKA_endship;

// --------------------------- R  L   # Frames    Collistion Box    Next Animation
vk_obj_ani VKA_endship = { 0, 1, VK_1_FRAMES, { 0<<8, 0<<8, 16<<8, 8<<8}, &VKA_endship };

int VKF_endship_init(vk_object *obj){
	// Animation function
	obj->var1 = 0;
	// Animation tick
	obj->var2 = 0;
	// Make it face right
	obj->facing = 1;

};

int VKF_endship_collide(vk_object *obj, vk_object *cobj){
	// Never collides with anything
	return 0;
};


int VKF_endship_think(vk_object *obj){	
	if(vk_engine_gstate.endsequence==2){
		// Animate the ship
		if(obj->var1==0){
			obj->var2 += 1;
			if(obj->var2>0x20){
				obj->var2 = 0;
				obj->var1 += 1;
			}
			obj->pos_y -= 0x100;
		}
		if(obj->var1==1){
			if(obj->pos_x < VK_END_SHIP_POS[2]){
				obj->pos_x += 0x100;
				if(obj->pos_x > VK_END_SHIP_POS[2]){
					obj->pos_x = VK_END_SHIP_POS[2];
				}
			}
			if(obj->pos_y < VK_END_SHIP_POS[3]){
				obj->pos_y += 0x60;
				if(obj->pos_y > VK_END_SHIP_POS[3]){
					obj->pos_y = VK_END_SHIP_POS[3];
				}
			}
			if(obj->pos_x == VK_END_SHIP_POS[2] && obj->pos_y == VK_END_SHIP_POS[3]){
				obj->var1 += 1;
			}
		}
		if(obj->var1==2){
			obj->var2 += 1;
			if(obj->var2>0x10){
				// Spawn question mark
				vk_endpression_obj->pos_x = obj->pos_x;
				vk_endpression_obj->pos_y = obj->pos_y-(16<<8);		
				vk_endpression_obj->facing = 1;	
			}
			if(obj->var2>0x80){
				obj->var2 = 0;
				obj->var1 += 1;
				// Hide it
				vk_endpression_obj->pos_x = obj->pos_x-0xC000;
				vk_endpression_obj->pos_y = obj->pos_y-(16<<8);	
				vk_endpression_obj->facing = 0;	
				
//				VK_HideObject(vk_endpression_obj);
			}
		}
		if(obj->var1==3){
			// Face left
			obj->facing = 0;
			// Move left
			obj->pos_x -= 0x100;
			obj->var2 += 1;
			if(obj->var2>0x60){
				obj->var2 = 0;
				obj->var1 += 1;
			}
		}
		if(obj->var1==4){
			// Spawn exclimation point
			vk_endpression_obj->pos_x = obj->pos_x;
			vk_endpression_obj->pos_y = obj->pos_y-(16<<8);		
			vk_endpression_obj->facing = 0;	

			obj->var2 += 1;
			if(obj->var2>0x80){
				obj->var2 = 0;
				obj->var1 += 1;
				// Hide it
				vk_endpression_obj->pos_x = obj->pos_x-0xC000;
				vk_endpression_obj->pos_y = obj->pos_y-(16<<8);	
//				VK_HideObject(vk_endpression_obj);
				obj->vel_y = -0x140;
			}
		}
		if(obj->var1==5){
			// Face right
			obj->facing = 1;

			// Move down
			obj->pos_y += obj->vel_y>>2;
			obj->vel_y += 0x10;
			// Move right
			obj->pos_x += 0x110;

			obj->var2 += 1;
			if(obj->var2>0x58){
				obj->var2 = 0;
				obj->var1 += 1;
				// For next jump
				obj->vel_y = -0x120;
			}
		}
	}
	if(vk_engine_gstate.endsequence==4){
		// Animate the ship
		if(obj->var1==7){
			// Face left
			obj->facing = 0;

			// Move up
			obj->pos_y += obj->vel_y>>1;
			obj->vel_y += 0x02;
			// Move left
			obj->pos_x -= 0x110;

			obj->var2 += 1;
			if(obj->var2>0x58){
				obj->var2 = 0;
				obj->var1 += 1;
				vk_engine_gstate.endsequence += 1;
			}
		}
	}
	return 0;
};
