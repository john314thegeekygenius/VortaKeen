
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

#ifndef __VK_OBJECTS_ENGINE__
#define __VK_OBJECTS_ENGINE__


// Normal keen can't seem to handle more than 64
#define VK_MAX_OBJECTS 256

typedef struct vk_box {
	int16_t left;
	int16_t top;
	int16_t right;
	int16_t bottom;	
}vk_box;

typedef struct vk_obj_ani {
	uint16_t gfxoff_r;
	uint16_t gfxoff_l;
	uint16_t ticks;
	vk_box cbox;
	struct vk_obj_ani *next;
}vk_obj_ani;

typedef enum {
	vko_none 	= 0,
	vko_mapkeen = 1,
	vko_keen 	= 2,
	vko_yorp 	= 3,
	vko_garg 	= 4,
	vko_vorticon = 5,
	vko_vorticon_commander = 15,
	vko_butler = 6,
	vko_robotank = 7,
	vko_icecube = 8,
	vko_iceshard = 9,
	vko_chain = 10,
	vko_endship = 20,
	vko_expression = 21,
	vko_shot_friendly = 50,
	vko_shot_deadly = 51,
	vko_shot_zapzot = 52,
}vk_obj_type;


typedef struct vk_object {
	int32_t pos_x;
	int32_t pos_y;
	int16_t vel_x;
	int16_t vel_y;
	uint8_t facing; // 0 - left  1 - right
	uint8_t hit_left, hit_right, hit_top, hit_bottom; // Which side of keen hit
	uint8_t hitmap; // 0 - none  1 - all
	
	uint8_t on_ground; 
						/*
							0 	Fall through
							1 	Solid
							2 	Slippery
							3 	Ice 
						*/
	uint16_t click_map; // did keen interact with the level

	uint16_t var1;
	uint16_t var2;
	uint16_t var3;
	uint16_t var4;
	
	vk_obj_type type; // What kind of object is it
	
	// Animation Stuff
	uint16_t gfx_needs_update;
	uint16_t frame_count;
	vk_obj_ani *animation;
	
	// Logic stuff
	 int (*collide)(struct vk_object*obj,struct vk_object*cobj);
	 int (*think)(struct vk_object*obj);

	// Sprite stuff
	vk_sprite *s;
}vk_object;

typedef struct vk_door_sprite{
	int32_t x;
	int32_t y;
	uint16_t door_type;
	uint16_t animation;	
	uint16_t offset;
	uint16_t tag;

	uint32_t tt_off;
	uint32_t bt_off;
}vk_door_sprite;


typedef struct vk_ice_spawner{
	int32_t x;
	int32_t y;
	uint16_t vel_x;
	uint16_t vel_y;
	uint16_t timer;
	uint16_t type;
}vk_ice_spawner;


extern const int VK_END_SHIP_POS[];
extern vk_object * vk_endship_obj ;
extern vk_object * vk_endpression_obj ;



void VK_SetupDoors();
void VK_SpawnDoor(int32_t x, int32_t y, uint16_t door_type, uint16_t t_tile, uint16_t b_tile);

extern vk_object *vk_keen_obj;

void VK_SpawnShot(int32_t x,int32_t y,uint16_t dir, uint16_t type);

int VK_ObjAtEdge(vk_object *obj);
vk_object *VK_CreateObject(uint16_t sprite_id, int32_t x, int32_t y);

void VK_SetObjAnimation(vk_object *obj,vk_obj_ani *animation);
void VK_RemoveObjects();
void VK_RenderObjects();
void VK_HideObjects();

void VK_HideObject(vk_object *obj);

#endif
