
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

#include "VK_Headers.h"

GBA_IN_EWRAM vk_object vk_level_objects[VK_MAX_OBJECTS];
uint16_t vk_num_of_objects = 0;

vk_object *vk_keen_obj = NULL;

uint16_t vk_oupdate_tick = 0;


const int VK_END_SHIP_POS[] = {
	(6<<12), (5<<12), // Starting position
	(48<<12), (21<<12)-(8<<8), // Ending point (On top of earth)
};

vk_object * vk_endship_obj = NULL;
vk_object * vk_endpression_obj = NULL;


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


#include "actions/VKA_Keen.h"
#include "actions/VKA_MapKeen.h"
#include "actions/VKA_Shot.h"
#include "actions/VKA_Yorp.h"
#include "actions/VKA_Garg.h"
#include "actions/VKA_Vorticon.h"
#include "actions/VKA_Butler.h"
#include "actions/VKA_RoboTank.h"
#include "actions/VKA_Ice.h"
#include "actions/VKA_Chain.h"
#include "actions/VKA_EndShip.h"
#include "actions/VKA_Expression.h"

vk_ice_spawner VK_IceSpawners[32]; // Max of 32 spawners??????
int vk_ice_spawner_count = 0;

void VK_SpawnIceSpawner(int32_t x,int32_t y, uint16_t type){
	vk_ice_spawner *ispw = &VK_IceSpawners[vk_ice_spawner_count];
	
	ispw->x = x;
	ispw->y = y;
	ispw->timer = 0;
	
	ispw->vel_x = 0;
	ispw->vel_y = 0;
	ispw->type = type;

	switch(type){
		case 0:
			// E
			ispw->vel_x = 0x200;
		break;
		case 1:
			// NE
			ispw->vel_y = -0x200;
			ispw->vel_x = 0x200;
		break;
		case 2:
			// N
			ispw->vel_y = -0x200;
		break;
		case 3:
			// NW
			ispw->vel_y = -0x200;
			ispw->vel_x = -0x200;
		break;
		case 4:
			// W
			ispw->vel_x = -0x200;
		break;
		case 5:
			// SW
			ispw->vel_y = 0x200;
			ispw->vel_x = -0x200;
		break;
		case 6:
			// S
			ispw->vel_y = 0x200;
		break;
		case 7:
			// SE
			ispw->vel_y = 0x200;
			ispw->vel_x = 0x200;
		break;
	}
	vk_ice_spawner_count += 1;

	// Overwrite the first spawners????
	if(vk_ice_spawner_count >= 32){
		vk_ice_spawner_count = 0;
	}
};


void VK_SpawnShot(int32_t x,int32_t y,uint16_t dir, uint16_t type){
	vk_object *ptr = VK_CreateObject(252+type,x,y+(8<<8));
	if(dir==0){
		ptr->vel_x = -0x400;
	}else{
		ptr->vel_x = 0x400;
	}
};


vk_door_sprite VK_Doors[8]; // We can only open 8 doors at a time!
uint16_t vk_num_of_doors = 0;
uint16_t vk_door_count = 0;

const int VK_DOOR_GFX_OFFSET = 0x7000;

void VK_SetupDoors(){
	int i;
	for(i=0;i<8;i++){
		VK_Doors[i].animation = 0xFF;
		VK_Doors[i].tag = 120+i;
		GBA_RemakeSprite(VK_Doors[i].tag,0xF0,0xA0,GBA_SPR_16_32,VK_DOOR_GFX_OFFSET+(i<<6),GBA_SPRITE_ZTOP,-1);
		GBA_SET_SPRITE_TILES(VK_Doors[i].tag,0x380+(i<<4));
		GBA_UPDATE_SPRITE(VK_Doors[i].tag);

		GBA_ASM_MemSet16(GBA_SPRGFX_START+VK_DOOR_GFX_OFFSET+(i<<9),0x0000,128);
		GBA_ASM_MemSet16(GBA_SPRGFX_START+VK_DOOR_GFX_OFFSET+256+(i<<9),0x0000,128);
	}
};


void VK_SpawnDoor(int32_t x, int32_t y, uint16_t door_type, uint16_t t_tile, uint16_t b_tile){
	vk_door_sprite *door = &VK_Doors[vk_door_count];
	
	door->x = x;
	door->y = y;
	door->door_type = door_type;
	door->animation = 0;
	door->offset = (vk_door_count<<9);
	
	// Copy the graphics
//	GBA_DMA_Copy16(GBA_SPRGFX_START+ptr->s.spr_off[i][2]+bmpof, ptr->s.spr_data+gfxoff, ptr->s.spr_cw[i]<<2);
//	GBA_ASM_MemSet16(GBA_SPRGFX_START+VK_DOOR_GFX_OFFSET+door->offset,0x0404,1024);
//	GBA_ASM_MemSet16(GBA_SPRGFX_START+VK_DOOR_GFX_OFFSET+door->offset,0x0202,128);
//	GBA_ASM_MemSet16(GBA_SPRGFX_START+VK_DOOR_GFX_OFFSET+256+door->offset,0x0404,128);
	
	door->tt_off = ((t_tile>>3)*2048)+((t_tile%8)*128);
	door->bt_off = ((b_tile>>3)*2048)+((b_tile%8)*128);	

	// Top tile
	GBA_DMA_Copy32(GBA_SPRGFX_START+VK_DOOR_GFX_OFFSET+(door->offset),TILESET_data+door->tt_off,32);
	GBA_DMA_Copy32(GBA_SPRGFX_START+VK_DOOR_GFX_OFFSET+(door->offset)+128,TILESET_data+door->tt_off+1024,32);

	// Bottom tile
	GBA_DMA_Copy32(GBA_SPRGFX_START+VK_DOOR_GFX_OFFSET+(door->offset)+256,TILESET_data+door->bt_off,32);
	GBA_DMA_Copy32(GBA_SPRGFX_START+VK_DOOR_GFX_OFFSET+(door->offset)+384,TILESET_data+door->bt_off+1024,32);

	vk_door_count += 1;
	if(vk_door_count >= 8){
		vk_door_count = 0;
	}
};



void VK_RenderDoors(){
	int i;
	int16_t cx,cy;
	for(i = 0; i < 8; i++){
		vk_door_sprite *door = &VK_Doors[i];
		if(vk_oupdate_tick >= 0x4){
			if(door->animation <= 0x20){
				// Move the graphics down
				
				if(door->animation){
					uint16_t anichunk = (((0x20-door->animation)%8)<<3) + (((0x20-door->animation)>>3)<<7);
					GBA_ASM_MemSet16(GBA_SPRGFX_START+VK_DOOR_GFX_OFFSET+(door->offset)+anichunk, 0x0000,4);
					GBA_ASM_MemSet16(GBA_SPRGFX_START+VK_DOOR_GFX_OFFSET+(door->offset)+anichunk+64, 0x0000,4);
				}

				// Animate it
				door->animation += 0x1;
			}
		}
		if(door->animation <= 0x20){
			// Render the door
			cx = (door->x>>8)-(vk_level_offsetx<<4);
			cy = (door->y>>8)-(vk_level_offsety<<4)+(door->animation)-(door->animation>0);
			cx -= vk_map_offsetx;
			cy -= vk_map_offsety;
			
			if(cx >= (-16) && cy >= (-32) && cx < (32<<3) && cy < (20<<3)){	
				GBA_SET_SPRITE_POSITION(door->tag,cx,cy);
			}else{
				GBA_SET_SPRITE_POSITION(door->tag,0xF0,0xA0);
			}
		}else{
			GBA_SET_SPRITE_POSITION(door->tag,0xF0,0xA0);
		}
		GBA_UPDATE_SPRITE(door->tag);
	}

};

void VK_KeenCollectPoint(uint16_t amount){
	// Add the score
	vk_engine_gstate.score += amount;
	if(vk_engine_gstate.score >= vk_engine_gstate.next_1up){
		vk_engine_gstate.next_1up += 20000;
		vk_engine_gstate.numLives += 1;
		// Play the sound
		VK_PlaySound(VKS_EXTRAMANSND);
	}else{
		// Play the sound
		VK_PlaySound(VKS_GOTBONUSSND);
	}
};

void VK_KeenCollectPart(){
	// Add the score
	vk_engine_gstate.score += 10000;
	if(vk_engine_gstate.score >= vk_engine_gstate.next_1up){
		vk_engine_gstate.next_1up += 20000;
		vk_engine_gstate.numLives += 1;
		// Play the sound
		VK_PlaySound(VKS_EXTRAMANSND);
	}
	// Play the sound
	VK_PlaySound(VKS_GOTPARTSND);
};


// Advanced collistion code for keen vs level
int VK_CollideMapKeenWLevel(vk_object *obj){
	if(obj->animation==NULL){
		return 0;
	}

	int32_t kleft = (obj->pos_x+obj->animation->cbox.left)>>8;
	int32_t kright = (obj->pos_x+obj->animation->cbox.right)>>8;
	int32_t ktop = (obj->pos_y+obj->animation->cbox.top)>>8;
	int32_t kbottom = (obj->pos_y+obj->animation->cbox.bottom)>>8;

	int32_t leftTile = kleft>>4;
	int32_t rightTile = kright>>4;
	int32_t topTile = ktop>>4;
	int32_t bottomTile = kbottom>>4;

	int16_t tileY,tileX,tile;

	
	obj->var1 = 0;
	
    for(tileY = topTile-1; tileY <= bottomTile; tileY++){
        for(tileX = leftTile-1; tileX <= rightTile; tileX++){
            tile = (tileY*vk_level_width)+tileX;
            if(tileX<0||tileY<0||tileX>=vk_level_width||tileY>=vk_level_height){
                continue;
            }
            tile += (vk_level_width*vk_level_height);
			
			// We don't care about that sprite tile
			if(vk_sprite_data[(tileY*(vk_level_width>>3))+(tileX>>3)] & vk_bithash[tileX%8]){
				continue;
			}
            

            if(vk_level_map[tile]&0x8000){
				// Stop keen
				if(VK_CheckButton( GBA_BUTTON_RSHOLDER|GBA_BUTTON_LSHOLDER)==(GBA_BUTTON_RSHOLDER|GBA_BUTTON_LSHOLDER)){
					continue;
				}
					
					// Collide with top
				if(kright > (tileX<<4)){
					if(kleft < (tileX<<4)+(16)){
						if(kbottom >= (tileY<<4)){
							if(kbottom <= (tileY<<4)+4){
								if(obj->vel_y > 0){
									obj->pos_y = (tileY<<12)-(obj->animation->cbox.bottom);
									obj->vel_y = 0;
									obj->on_ground = vk_level_tileinfo[(vk_level_data[tile]*6)+2];
									obj->hit_bottom = 1;
									// Reset the variables
									ktop = (obj->pos_y+obj->animation->cbox.top)>>8;
									kbottom = (obj->pos_y+obj->animation->cbox.bottom)>>8;
								}
							}
						}
					}
				}
				
				// Collide with bottom
				if(kright > (tileX<<4)){
					if(kleft < (tileX<<4)+(16)){
						if(ktop >= (tileY<<4)+16-4){
							if(ktop <= (tileY<<4)+(16)){
								if(obj->vel_y < 0){
									obj->pos_y = ((tileY<<12)+(16<<8))-obj->animation->cbox.top;
									obj->vel_y = 0;
									obj->hit_top = 1;
									// Reset the variables
									ktop = (obj->pos_y+obj->animation->cbox.top)>>8;
									kbottom = (obj->pos_y+obj->animation->cbox.bottom)>>8;
								}
							}
						}
					}
				}
				
				// Collide with right
				 if(kleft < (tileX<<4)+2){
					if(kright >= (tileX<<4)){
						if(ktop < (tileY<<4)+(16)){
							if(kbottom >(tileY<<4)){
								if(obj->vel_x > 0){
									obj->pos_x = (tileX<<12)-obj->animation->cbox.right;
									obj->vel_x = 0;
									obj->hit_left = 1;
									// Reset the variables
									kleft = (obj->pos_x+obj->animation->cbox.left)>>8;
									kright = (obj->pos_x+obj->animation->cbox.right)>>8;
								}
							}
						}
					}
				}

				  // Collide with left
				if(kright > (tileX<<4)){
					if(kleft <= (tileX<<4)+(16)){
						if(ktop < (tileY<<4)+(16)){
							if(kbottom > (tileY<<4)){
								if(obj->vel_x < 0){
									obj->pos_x = (tileX<<12)+(16<<8)-obj->animation->cbox.left;
									obj->vel_x = 0;
									obj->hit_right = 1;
								}
							}
						}
					}
				}
			}else
			if(vk_level_map[tile]&&vk_level_map[tile]!=255){
				if(kright > (tileX<<4)){
					if(kleft < (tileX<<4)+(16)){
						if(kbottom > (tileY<<4)){
							if(ktop < (tileY<<4)+16){
								obj->var1 = tile;
							}
						}
					}
				}
			}
		}
	}
};

uint8_t vk_keen_collide_flag = 0;

// Advanced collistion code for keen vs level
int VK_CollideKeenWLevel(vk_object *obj){
	int i;
	if(obj->animation==NULL){
		return 0;
	}
	if(obj->animation == &VKA_keen_die_1 || obj->animation == &VKA_keen_die_2){
		return 0; // Stop keen from collecting things
	}
	if(vk_keen_collide_flag==0){
		vk_keen_collide_flag = 1;
		return;
	}
	vk_keen_collide_flag = 0;

	int32_t kleft = (obj->pos_x+obj->animation->cbox.left)>>8;
	int32_t kright = (obj->pos_x+obj->animation->cbox.right)>>8;
	int32_t ktop = (obj->pos_y+obj->animation->cbox.top)>>8;
	int32_t kbottom = (obj->pos_y+obj->animation->cbox.bottom)>>8;

	int32_t leftTile = kleft>>4;
	int32_t rightTile = kright>>4;
	int32_t topTile = ktop>>4;
	int32_t bottomTile = kbottom>>4;

	int16_t tileY,tileX,tile;


	
	obj->click_map = 0;
	
    for(tileY = topTile-1; tileY <= bottomTile; tileY++){
        for(tileX = leftTile-1; tileX <= rightTile; tileX++){
            tile = (tileY*vk_level_width)+tileX;
            if(tileX<0||tileY<0||tileX>=vk_level_width||tileY>=vk_level_height){
                continue;
            }
			if(kright > (tileX<<4)){
				if(kleft < (tileX<<4)+(16)){
					if(kbottom > (tileY<<4)){
						if(ktop < (tileY<<4)+16){
							// Collide with tile
							switch(vk_level_tileinfo[(vk_level_data[tile]*6)+1]){
								case 1:
									// Kill keen
									VKF_keen_die(obj);
								break;
								case 2:
									if(vk_engine_gstate.gotKeycardY && 
									vk_level_tileinfo[(vk_level_data[tile+vk_level_width]*6)+1]==2){
										vk_engine_gstate.gotKeycardY -= 1;
										// Spawn the door
										VK_SpawnDoor(tileX<<12,tileY<<12,0,vk_level_data[tile],vk_level_data[tile+vk_level_width]);
										// Set tile to door tile???
										vk_level_data[tile] = 0;
										vk_level_data[tile+vk_level_width] = 0;
										// Play the sound
										VK_PlaySound(VKS_DOOROPENSND);
									}else{
										
										if(kright > (tileX<<4)+8){
											obj->pos_x = (tileX<<12)+(16<<8)-obj->animation->cbox.left;
											obj->vel_x = -0x80;
										}else{
											obj->pos_x = (tileX<<12)-obj->animation->cbox.right;
											obj->vel_x = 0x80;
										}
									}
									break;
								case 3:
									if(vk_engine_gstate.gotKeycardR &&
									vk_level_tileinfo[(vk_level_data[tile+vk_level_width]*6)+1]==3){
										vk_engine_gstate.gotKeycardR -= 1;
										// Spawn the door
										VK_SpawnDoor(tileX<<12,tileY<<12,1,vk_level_data[tile],vk_level_data[tile+vk_level_width]);
										// Set tile to door tile???
										vk_level_data[tile] = 0;
										vk_level_data[tile+vk_level_width] = 0;
										// Play the sound
										VK_PlaySound(VKS_DOOROPENSND);
									}else{
										if(kright > (tileX<<4)+8){
											obj->pos_x = (tileX<<12)+(16<<8)-obj->animation->cbox.left;
											obj->vel_x = -0x80;
										}else{
											obj->pos_x = (tileX<<12)-obj->animation->cbox.right;
											obj->vel_x = 0x80;
										}
									}
									break;
								case 4:
									if(vk_engine_gstate.gotKeycardG &&
									vk_level_tileinfo[(vk_level_data[tile+vk_level_width]*6)+1]==4){
										vk_engine_gstate.gotKeycardG -= 1;
										// Spawn the door
										VK_SpawnDoor(tileX<<12,tileY<<12,2,vk_level_data[tile],vk_level_data[tile+vk_level_width]);
										// Set tile to door tile???
										vk_level_data[tile] = 0;
										vk_level_data[tile+vk_level_width] = 0;
										// Play the sound
										VK_PlaySound(VKS_DOOROPENSND);
									}else{
										if(kright > (tileX<<4)+8){
											obj->pos_x = (tileX<<12)+(16<<8)-obj->animation->cbox.left;
											obj->vel_x = -0x80;
										}else{
											obj->pos_x = (tileX<<12)-obj->animation->cbox.right;
											obj->vel_x = 0x80;
										}
									}
									break;
								case 5:
									// Door
									if(vk_engine_gstate.gotKeycardB &&
									vk_level_tileinfo[(vk_level_data[tile+vk_level_width]*6)+1]==5){
										vk_engine_gstate.gotKeycardB -= 1;
										// Spawn the door
										VK_SpawnDoor(tileX<<12,tileY<<12,3,vk_level_data[tile],vk_level_data[tile+vk_level_width]);
										// Set tile to door tile???
										vk_level_data[tile] = 0;
										vk_level_data[tile+vk_level_width] = 0;
										// Play the sound
										VK_PlaySound(VKS_DOOROPENSND);
									}else{
										if(kright > (tileX<<4)+8){
											obj->pos_x = (tileX<<12)+(16<<8)-obj->animation->cbox.left;
											obj->vel_x = -0x80;
										}else{
											obj->pos_x = (tileX<<12)-obj->animation->cbox.right;
											obj->vel_x = 0x80;
										}
									}
									break;
								break;
								case 6:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add the score
									VK_KeenCollectPoint(500);
									break;
								case 7:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add the score
									VK_KeenCollectPoint(100);
									break;
								case 8:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add the score
									VK_KeenCollectPoint(200);
									break;
								case 9:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add the score
									VK_KeenCollectPoint(1000);
									break;
								case 10:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add the score
									VK_KeenCollectPoint(5000);
									break;
								case 11:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add the joystick
									vk_engine_gstate.gotJoystick = 1;
									VK_KeenCollectPart();
									break;
								case 12:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add the battery
									vk_engine_gstate.gotBattery = 1;
									VK_KeenCollectPart();
									break;
								case 13:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add the vacuum
									vk_engine_gstate.gotVacuum = 1;
									VK_KeenCollectPart();
									break;
								case 14:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add the whiskey
									vk_engine_gstate.gotWhiskey = 1;
									VK_KeenCollectPart();
									break;
								case 15:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add ammo
									vk_engine_gstate.ammo += 5;
									// Play the sound
									VK_PlaySound(VKS_GOTITEMSND);
									break;
								case 16:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add ammo
									vk_engine_gstate.gotPogo = 1;
									// Play the sound
									VK_PlaySound(VKS_GOTITEMSND);
									break;
								case 17:
									// Exit the level
									if(obj->on_ground){
										// Kindof cheaty
										if(obj->animation == &VKA_keen_idle ||
											obj->animation == &VKA_keen_walk_1 ||
											obj->animation == &VKA_keen_walk_2 ||
											obj->animation == &VKA_keen_walk_3 ||
											obj->animation == &VKA_keen_walk_4 ){
											if(vk_engine_gstate.finished_level==0){
											
												// Place some tiles
												VK_RenderTile(tileX+2,tileY,vk_level_data[tile+2],1);
												VK_RenderTile(tileX+2,tileY+1,vk_level_data[tile+2+vk_level_width],1);
												VK_RenderTile(tileX+3,tileY,vk_level_data[tile+3],1);
												VK_RenderTile(tileX+3,tileY+1,vk_level_data[tile+3+vk_level_width],1);
												VK_RenderTile(tileX+4,tileY,vk_level_data[tile+4],1);
												VK_RenderTile(tileX+4,tileY+1,vk_level_data[tile+4+vk_level_width],1);
												
												// Stop the map from re-rendering
												VK_DisableLevelRendering();

												if(vk_engine_gstate.in_game>0&&vk_engine_gstate.in_game<=16){
													vk_engine_gstate.levelDone[vk_engine_gstate.in_game-1] = 1;
												}
												VK_PlaySound(14);
												
												vk_engine_gstate.gotKeycardY = 0;
												vk_engine_gstate.gotKeycardR = 0;
												vk_engine_gstate.gotKeycardG = 0;
												vk_engine_gstate.gotKeycardB = 0;

												vk_engine_gstate.finished_level = 1;
											}
										}
									}
									break;
								case 18:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add keycard
									vk_engine_gstate.gotKeycardY += 1; // HACK: Make it so we can collect more than 1 keycard at a time
									// Play the sound
									VK_PlaySound(VKS_GETCARDSND);
									break;
								case 19:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add keycard
									vk_engine_gstate.gotKeycardR += 1; // HACK: Make it so we can collect more than 1 keycard at a time
									// Play the sound
									VK_PlaySound(VKS_GETCARDSND);
									break;
								case 20:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add keycard
									vk_engine_gstate.gotKeycardG += 1; // HACK: Make it so we can collect more than 1 keycard at a time
									// Play the sound
									VK_PlaySound(VKS_GETCARDSND);
									break;
								case 21:
									// Remove the tile
									vk_level_data[tile] = 0;
									VK_ForceLevelUpdate();
									// Add keycard
									vk_engine_gstate.gotKeycardB += 1; // HACK: Make it so we can collect more than 1 keycard at a time
									// Play the sound
									VK_PlaySound(VKS_GETCARDSND);
									break;
								case 22:
									// Fix tile
									vk_level_data[tile] = vk_level_data[tile+vk_level_width]-1;
									
									VK_ForceLevelUpdate();
									// Stop all sound
									VK_StopSound();

									// Display message
									VK_DisplayMessage(vk_engine_gstate.in_game-1);
									
									break;
								case 233:
									if(vk_keen_input[6]){
										// Change the tile
										//vk_level_data[tile] = 0;
										VK_ForceLevelUpdate();
										// Turn the lights out / on
										VK_ToggleLights();
										// Play the sound
										VK_PlaySound(VKS_CLICKSND);
										// Tell keen we interacted
										obj->click_map = 1;
									}
									break;
								case  24:
									// Teleport Keen to secret area

									// VK_TELEPORT_DEST is defined in VK_LevelEngine.c
									vk_engine_gstate.posX = VK_TELEPORT_DEST[(2<<1)];
									vk_engine_gstate.posY = VK_TELEPORT_DEST[(2<<1)+1];

									vk_engine_gstate.viewportX = vk_engine_gstate.posX-(8<<12);
									vk_engine_gstate.viewportY = vk_engine_gstate.posY-(6<<12);

									if(vk_engine_gstate.in_game>0&&vk_engine_gstate.in_game<=16){
										vk_engine_gstate.levelDone[vk_engine_gstate.in_game-1] = 1;
									}
									
									vk_engine_gstate.gotKeycardY = 0;
									vk_engine_gstate.gotKeycardR = 0;
									vk_engine_gstate.gotKeycardG = 0;
									vk_engine_gstate.gotKeycardB = 0;

									VK_ReturnToWorldmap();

									vk_engine_gstate.posX = VK_TELEPORT_DEST[(2<<1)];
									vk_engine_gstate.posY = VK_TELEPORT_DEST[(2<<1)+1];

									vk_engine_gstate.viewportX = vk_engine_gstate.posX-(8<<12);
									vk_engine_gstate.viewportY = vk_engine_gstate.posY-(6<<12);
									
									vk_keen_obj->pos_x = vk_engine_gstate.posX;
									vk_keen_obj->pos_y = vk_engine_gstate.posY;

									vk_engine_gstate.teleporting = 0x20; // Timer
									vk_engine_gstate.teleporter_pos = ((VK_TELEPORT_DEST[(2<<1)+1]>>12)*vk_level_width) + (VK_TELEPORT_DEST[(2<<1)]>>12);
									vk_engine_gstate.teleporter = vk_level_data[vk_engine_gstate.teleporter_pos];

									VK_PlaySound(VKS_TELEPORTSND);
									
									// Animate the teleporter
									vk_level_data[vk_engine_gstate.teleporter_pos] = vk_num_of_tiles-8; // Ice is -4

									// Add the animation
									ck_update_positions[ck_number_of_updates][0] = (vk_engine_gstate.teleporter_pos%vk_level_width);
									ck_update_positions[ck_number_of_updates][1] = (vk_engine_gstate.teleporter_pos/vk_level_width);
									ck_number_of_updates += 1;
									return;
									break;
								case 25:
									if(vk_keen_input[6]){
										int8_t bx,by;
										// Set this
										VK_BRIDGE_TILE = vk_level_data[tile]+2;
										// Spawn bridge
										by = (vk_level_map[tile+(vk_level_width*vk_level_height)]>>8)&0xFF;
										bx = (vk_level_map[tile+(vk_level_width*vk_level_height)])&0xFF;
										VK_SpawnBridge(tile,bx,by);
										// Change the tile
										vk_level_data[tile] = vk_level_data[tile]+1;
										VK_ForceLevelUpdate();
										// Play the sound
										VK_PlaySound(VKS_CLICKSND);
										// Tell keen we interacted
										obj->click_map = 1;
									}
									break;
								case 26:
									if(vk_keen_input[6]){
										int8_t bx,by;
										// Set this
										VK_BRIDGE_TILE = vk_level_data[tile]+1;
										// Spawn bridge
										by = (vk_level_map[tile+(vk_level_width*vk_level_height)]>>8)&0xFF;
										bx = (vk_level_map[tile+(vk_level_width*vk_level_height)])&0xFF;
										VK_SpawnBridge(tile,bx,by);
										// Change the tile
										vk_level_data[tile] = vk_level_data[tile]+1;
										VK_ForceLevelUpdate();
										// Play the sound
										VK_PlaySound(VKS_CLICKSND);
									}
									break;
#ifdef VK_KEEN3
								case 27:
									// Give Keen an Ankh
									break;
								case 28:
									// Ammo clip (+n shots)
									break;
#endif
							}
						}
					}
				}
			}
		}
	}
	if(kleft < 32){
		obj->pos_x = (32<<8) - obj->animation->cbox.left;
		obj->vel_x = 0;
		obj->hit_left = 1;
	}
	if(kright > (vk_level_width<<4)-32){
		obj->pos_x = (((vk_level_width<<4)-32)<<8)-obj->animation->cbox.right;
		obj->vel_x = 0;
		obj->hit_right = 1;
	}
	if(ktop < 32){
		obj->pos_y = (32<<8) - obj->animation->cbox.top;
		obj->vel_y = 0;
	}
	if(kbottom > (vk_level_height<<4)){
		// Kill keen
		VKF_keen_die(obj);
	}
};


// Basic collistion code for object vs level
int VK_CollideObjWLevel(vk_object *obj){
	if(obj->animation==NULL){
		return 0;
	}

	int32_t kleft = (obj->pos_x+obj->animation->cbox.left)>>8;
	int32_t kright = (obj->pos_x+obj->animation->cbox.right)>>8;
	int32_t ktop = (obj->pos_y+obj->animation->cbox.top)>>8;
	int32_t kbottom = (obj->pos_y+obj->animation->cbox.bottom)>>8;

	int32_t leftTile = kleft>>4;
	int32_t rightTile = kright>>4;
	int32_t topTile = ktop>>4;
	int32_t bottomTile = kbottom>>4;


	int16_t tileY,tileX,tile;
	/*
	obj->hit_bottom = 0;
	obj->hit_top = 0;
	obj->hit_right = 0;
	obj->hit_left = 0;
	*/
	obj->on_ground = 0;
    
    for(tileY = topTile-1; tileY <= bottomTile; tileY++){
        for(tileX = leftTile-1; tileX <= rightTile; tileX++){
            tile = (tileY*vk_level_width)+tileX;
            if(tileX<0||tileY<0||tileX>=vk_level_width||tileY>=vk_level_height){
                continue;
            }
		
			// Collide with top
			if(vk_level_tileinfo[(vk_level_data[tile]*6)+2]){
				if(kright > (tileX<<4)){
					if(kleft < (tileX<<4)+(16)){
						if(kbottom >= (tileY<<4)){
							if(kbottom <= (tileY<<4)+(obj->vel_y>>8)){
								if(obj->vel_y > 0){
									obj->pos_y = (tileY<<12)-(obj->animation->cbox.bottom);
									obj->vel_y = 0;
									obj->on_ground = vk_level_tileinfo[(vk_level_data[tile]*6)+2];
									obj->hit_bottom = 1;
									// Reset the variables
									ktop = (obj->pos_y+obj->animation->cbox.top)>>8;
									kbottom = (obj->pos_y+obj->animation->cbox.bottom)>>8;
								}
							}
						}
					}
				}
			}
			
			// Collide with bottom
			if(vk_level_tileinfo[(vk_level_data[tile]*6)+4]){
				if(kright > (tileX<<4)){
					if(kleft < (tileX<<4)+(16)){
						if(ktop >= (tileY<<4)+16+(obj->vel_y>>8)){
							if(ktop <= (tileY<<4)+(16)){
								if(obj->vel_y < 0){
									obj->pos_y = ((tileY<<12)+(16<<8))-obj->animation->cbox.top;
									obj->vel_y = 0;
									obj->hit_top = 1;
									// Reset the variables
									ktop = (obj->pos_y+obj->animation->cbox.top)>>8;
									kbottom = (obj->pos_y+obj->animation->cbox.bottom)>>8;
								}
							}
						}
					}
				}
			}
			
			// Collide with right
			if(vk_level_tileinfo[(vk_level_data[tile]*6)+3]){
				 if(kleft < (tileX<<4)+2){
					if(kright >= (tileX<<4)){
						if(ktop < (tileY<<4)+(16)){
							if(kbottom >(tileY<<4)){
								if(obj->vel_x > 0){
									obj->pos_x = (tileX<<12)-obj->animation->cbox.right;
									obj->vel_x = 0;
									obj->hit_left = 1;
									// Reset the variables
									kleft = (obj->pos_x+obj->animation->cbox.left)>>8;
									kright = (obj->pos_x+obj->animation->cbox.right)>>8;
								}
							}
						}
					}
				}
			}

			  // Collide with left
			if(vk_level_tileinfo[(vk_level_data[tile]*6)+5]){
				if(kright > (tileX<<4)){
					if(kleft <= (tileX<<4)+(16)){
						if(ktop < (tileY<<4)+(16)){
							if(kbottom > (tileY<<4)){
								if(obj->vel_x < 0){
									obj->pos_x = (tileX<<12)+(16<<8)-obj->animation->cbox.left;
									obj->vel_x = 0;
									obj->hit_right = 1;
								}
							}
						}
					}
				}
			}
        }
    }
	return 0;
};


// Test if object is at edge
int VK_ObjAtEdge(vk_object *obj){
	if(obj->animation==NULL){
		return 0;
	}

	int32_t kleft = (obj->pos_x+obj->animation->cbox.left)>>8;
	int32_t kright = (obj->pos_x+obj->animation->cbox.right)>>8;
	int32_t kbottom = (obj->pos_y+obj->animation->cbox.bottom)>>8;

	int32_t leftTile = kleft>>4;
	int32_t rightTile = kright>>4;
	int32_t bottomTile = kbottom>>4;

	int16_t tile;

	obj->on_ground = 0;
    
	tile = (bottomTile*vk_level_width)+leftTile-1;
	if(!((leftTile-1)<0||bottomTile<0||(leftTile-1)>=vk_level_width||bottomTile>=vk_level_height)){
		if(!vk_level_tileinfo[(vk_level_data[tile]*6)+2]){
			return -1;
		}
	}

	tile = (bottomTile*vk_level_width)+rightTile;
	if(!(rightTile<0||bottomTile<0||rightTile>=vk_level_width||bottomTile>=vk_level_height)){
		if(!vk_level_tileinfo[(vk_level_data[tile]*6)+2]){
			return 1;
		}
	}

	return 0;
};


vk_object *VK_CreateObject(uint16_t sprite_id, int32_t x, int32_t y){
	vk_object * obj = &vk_level_objects[vk_num_of_objects];
	int i;

	for(i = 0; i < vk_num_of_objects; i++){
		if(vk_level_objects[i].animation == NULL){
			obj = &vk_level_objects[i];
			VK_RemoveSprite(obj->s); // In case we need to
			vk_num_of_objects -= 1;
			break;
		}
	}

	vk_num_of_objects+= 1;
	if(vk_num_of_objects >= VK_MAX_OBJECTS){
		vk_num_of_objects = VK_MAX_OBJECTS; // Uhhhh
		while(1){
			GBA_ASM_MemSet16(GBA_VRAM,0x0001,120*160);
		}; // Just hang
	}

	obj->s = NULL;
	obj->animation = NULL;
	obj->collide = NULL;
	obj->think = NULL;

	obj->vel_x = 0;
	obj->vel_y = 0;
	
	obj->hit_top = 0;
	obj->hit_bottom = 0;
	obj->hit_left = 0;
	obj->hit_right = 0;

	obj->var1 = 0;
	obj->var2 = 0;
	obj->var3 = 0;
	obj->var4 = 0;

	obj->frame_count = 0;

	switch(sprite_id){
		case 1:
			// Yorp
			obj->s = VK_CreateSprite(1);
			VK_SetObjAnimation(obj,&VKA_yorp_idle_1);
			obj->collide = &VKF_yorp_collide;
			obj->think = &VKF_yorp_think;
			obj->type = vko_yorp;
			obj->hitmap = 1;
			VKF_yorp_init(obj);
			break;
		case 2:
			// Garg
			obj->s = VK_CreateSprite(2);
			VK_SetObjAnimation(obj,&VKA_garg_idle_1);
			obj->collide = &VKF_garg_collide;
			obj->think = &VKF_garg_think;
			obj->type = vko_garg;
			obj->hitmap = 1;
			VKF_garg_init(obj);
			break;
		case 3:
			// Vorticon
			obj->s = VK_CreateSprite(3);
			VK_SetObjAnimation(obj,&VKA_vorticon_walk_1);
			obj->collide = &VKF_vorticon_collide;
			obj->think = &VKF_vorticon_think;
			obj->type = vko_vorticon;
			if(vk_level_id==VORTICON_COMMANDER_LEVEL){
				obj->type = vko_vorticon_commander;
			}
			obj->hitmap = 1;
			VKF_vorticon_init(obj);

		break;
		case 4:
			// Butler robot
			obj->s = VK_CreateSprite(4);
			VK_SetObjAnimation(obj,&VKA_butler_walk_1);
			obj->collide = &VKF_butler_collide;
			obj->think = &VKF_butler_think;
			obj->type = vko_butler;
			obj->hitmap = 1;
			VKF_robotank_init(obj);
			break;
		case 5:
			// Tank robot
			obj->s = VK_CreateSprite(5);
			VK_SetObjAnimation(obj,&VKA_robotank_roll_1);
			obj->collide = &VKF_robotank_collide;
			obj->think = &VKF_robotank_think;
			obj->type = vko_robotank;
			obj->hitmap = 1;
			VKF_robotank_init(obj);
			
		break;
		case 6:
		case 7:
		case 8:
		case 9:
			// Ice
		break;
		case 10:
			// Chain
			obj->s = VK_CreateSprite(10);
			VK_SetObjAnimation(obj,&VKA_chain);
			obj->collide = &VKF_chain_collide;
			obj->think = &VKF_chain_think;
			obj->type = vko_chain;
			obj->hitmap = 0;
			VKF_chain_init(obj);
			
		break;
		case 20:
			// End ship
			obj->s = VK_CreateSprite(20);
			VK_SetObjAnimation(obj,&VKA_endship);
			obj->collide = &VKF_endship_collide;
			obj->think = &VKF_endship_think;
			obj->type = vko_endship;
			obj->hitmap = 0;
			VKF_endship_init(obj);			
		break;
		case 21:
			// Expression
			obj->s = VK_CreateSprite(21);
			VK_SetObjAnimation(obj,&VKA_expression);
			obj->collide = &VKF_expression_collide;
			obj->think = &VKF_expression_think;
			obj->type = vko_expression;
			obj->hitmap = 0;
			VKF_expression_init(obj);			
		break;
		case 251:
			// Zap Zot obj
			obj->s = VK_CreateSprite(19);
			VK_SetObjAnimation(obj,&VKA_shot_zap);
			obj->collide = NULL;
			obj->think = NULL;
			obj->type = vko_shot_zapzot;
			obj->hitmap = 0;
		break;
		case 252:
			// Keen Shot
			obj->s = VK_CreateSprite(8);
			VK_SetObjAnimation(obj,&VKA_shot_keen);
			obj->collide = &VKF_shot_collide;
			obj->think = &VKF_shot_think;
			obj->type = vko_shot_friendly;
			obj->hitmap = 1;
			obj->var1 = 0;
		break;
		case 253:
			// Enemy Shot
			obj->s = VK_CreateSprite(9);
			VK_SetObjAnimation(obj,&VKA_shot_enemy);
			obj->collide = &VKF_shot_collide;
			obj->think = &VKF_shot_think;
			obj->type = vko_shot_deadly;
			obj->hitmap = 1;
			obj->var1 = 0;
			break;
		case 254:
			// World Map Commander Keen
			obj->s = VK_CreateSprite(254);
			// WARNING: This makes it so that only one keen sprite is playable
			vk_keen_obj = obj;
			VK_SetObjAnimation(obj,&VKA_mapkeen_idle_v);
			obj->collide = &VKF_mapkeen_collide;
			obj->think = &VKF_mapkeen_think;
			obj->type = vko_mapkeen;
			obj->hitmap = 1;
			VKF_mapkeen_init(obj);
		break;
		case 255:
			// Commander Keen
			obj->s = VK_CreateSprite(255);
			// WARNING: This makes it so that only one keen sprite is playable
			vk_keen_obj = obj;
			VK_SetObjAnimation(obj,&VKA_keen_idle);
			obj->collide = &VKF_keen_collide;
			obj->think = &VKF_keen_think;
			obj->type = vko_keen;
			obj->hitmap = 1;
			VKF_keen_init(obj);
			obj->on_ground = 1;
		break;
	};
	obj->pos_x = x - (obj->animation->cbox.left);
	obj->pos_y = y;// + (32<<8) - (obj->animation->cbox.bottom);

	obj->gfx_needs_update = 1;
	obj->frame_count = 0;

	return obj;
};

void VK_RemoveObjects(){
	int i;
	VK_ClearSprites();

	vk_oupdate_tick = 0;
	vk_num_of_objects = 0;

	vk_door_count = 0;
	VK_SetupDoors();
	
	vk_ice_spawner_count = 0;

};

void VK_SetObjAnimation(vk_object *obj,vk_obj_ani *animation){
	obj->frame_count = 0;
	obj->animation = animation;
	if(animation!=NULL){
		if(obj->facing==0){
			obj->s->s.spr_gfx_ani = obj->animation->gfxoff_l;
		}else{
			obj->s->s.spr_gfx_ani = obj->animation->gfxoff_r;
		}
		obj->gfx_needs_update = 1;
	}else{
		VK_RemoveSprite(obj->s);
		obj->s = NULL;
	}
};

int VK_ObjInObj(vk_object *o1,vk_object *o2){
	if(o1->animation==NULL || o2->animation == NULL){
		return 0; // Bad animation so no collision
	}
	if(o2->pos_x + o2->animation->cbox.right > o1->pos_x + o1->animation->cbox.left){
		if(o2->pos_x + o2->animation->cbox.left < o1->pos_x + o1->animation->cbox.right){
			if(o2->pos_y + o2->animation->cbox.bottom > o1->pos_y + o1->animation->cbox.top){
				if(o2->pos_y + o2->animation->cbox.top < o1->pos_y + o1->animation->cbox.bottom){
					return 1;
				}
			}
		}
	}
	return 0;
};

void VK_RenderObjects(){
	int i, e, cx, cy;

	vk_oupdate_tick += 1;
	
	// Render the doors
	VK_RenderDoors();
	
	// Run the spawners
	for(i = 0; i < vk_ice_spawner_count; i++){
		vk_ice_spawner *ispw = &VK_IceSpawners[i];
		ispw->timer += 1;
		if(ispw->timer > 0x40){
			ispw->timer = 0;
			// Spawn cube if keen is near
			if(ispw->y > vk_keen_obj->pos_y-(256<<8) && ispw->y < vk_keen_obj->pos_y+(256<<8)){
				if(ispw->x > vk_keen_obj->pos_x-(256<<8) && ispw->x < vk_keen_obj->pos_x+(256<<8)){

					// Spawn the cube
					vk_object * obj = VK_CreateObject(6,ispw->x-(12<<8),ispw->y);
					if(ispw->type >= 3){
						obj->pos_x -= (8<<8);
					}
					obj->s = VK_CreateSprite(6);
					VK_SetObjAnimation(obj,&VKA_icecube);
					obj->collide = &VKF_icecube_collide;
					obj->think = &VKF_icecube_think;
					obj->type = vko_icecube;
					obj->hitmap = 1; // Yes we collide
					// Set the velocitys
					obj->vel_x = ispw->vel_x;
					obj->vel_y = ispw->vel_y;
					// Play the firing sound
					VK_PlaySound(VKS_CANNONFIRE);
				}
			}
		}
	}

	int objlen = vk_num_of_objects;
	
	for(i=0;i<objlen;i++){
		vk_object * obj = &vk_level_objects[i];
				
		if(obj!=NULL){
					
			if(obj->animation == NULL){
				if(obj->s!=NULL){
					VK_RemoveSprite(obj->s);
					obj->s = NULL;
				}
				continue;
			}
			
			if(vk_oupdate_tick >= 0x4){
				if(obj->animation!=NULL){
					obj->frame_count += 1;

					if(obj->frame_count >= obj->animation->ticks){
						VK_SetObjAnimation(obj,obj->animation->next);
					}
				}
			}

			// Only update the object if near keen
			if( (obj->pos_y > vk_keen_obj->pos_y-(256<<8) && obj->pos_y < vk_keen_obj->pos_y+(256<<8)) &&
				(obj->pos_x > vk_keen_obj->pos_x-(256<<8) && obj->pos_x < vk_keen_obj->pos_x+(256<<8)) ){


					if(obj->hitmap){
						VK_CollideObjWLevel(obj);
					}

					if(obj->collide!=NULL){
						if(VK_ObjInObj(obj,vk_keen_obj)){
							obj->collide(obj,vk_keen_obj);
						}

						// Collide all objects with shots
						
						if(obj->type == vko_shot_friendly || obj->type == vko_shot_deadly){
							for(e=0;e<vk_num_of_objects;e++){
								vk_object *obj2 = &vk_level_objects[e];
								if(obj2->type == vko_shot_friendly || obj2->type == vko_shot_deadly){
									continue;
								}
								if(e==i) continue;
								if(VK_ObjInObj(obj,obj2)){
									if(obj2->collide!=NULL){
										obj2->collide(obj2,obj);
									}
								}
							}
						}
					}

					if(obj->think!=NULL){
						obj->think(obj);
					}
					
					if(obj->animation==NULL){
						continue;
					}

					cx = (obj->pos_x>>8)-(vk_level_offsetx<<4);
					cy = (obj->pos_y>>8)-(vk_level_offsety<<4);
					cx -= vk_map_offsetx;
					cy -= vk_map_offsety;
					
					if(obj->s){
						if(cx >= (0-obj->s->s.spr_width) && cy >= (0-obj->s->s.spr_height) && cx < (32<<3) && cy < (20<<3)){
							if(obj->gfx_needs_update){
								obj->gfx_needs_update = 0;
								VK_SetSpriteGraphics(obj->s);
							}
						}
						obj->s->x = cx;
						obj->s->y = cy;
					}
					VK_RenderSprite(obj->s);

				}else{
					if(obj->type == vko_shot_friendly || obj->type == vko_shot_deadly || 
					obj->type == vko_icecube || obj->type == vko_iceshard){
						// Delete the object
						VK_SetObjAnimation(obj,NULL);
					}
				}
		}else{
			// Reposition the sprite
		//	VK_RenderSprite(obj->s);
		}
	}


	/*

	VK_TextX = 2;
	VK_TextY = 2;
	VK_Print("C:");
	VK_TextX = 4;
	VK_Print(VK_Iota16(vk_num_of_objects));

	VK_TextX = 2;
	VK_TextY = 2;
	VK_Print("X:");
	VK_TextX = 4;
	VK_Print(VK_Iota16(vk_keen_obj->pos_x));

	VK_TextX = 2;
	VK_TextY = 3;
	VK_Print("Y:");
	VK_TextX = 4;
	VK_Print(VK_Iota16(vk_keen_obj->pos_y));

	VK_TextX = 2;
	VK_TextY = 4;
	VK_Print("WX:");
	VK_TextX = 6;
	VK_Print(VK_Iota16(vk_engine_gstate.posX));

	VK_TextX = 2;
	VK_TextY = 5;
	VK_Print("WY:");
	VK_TextX = 6;
	VK_Print(VK_Iota16(vk_engine_gstate.posY));
	*/
	

	if(vk_oupdate_tick >= 0x4){
		vk_oupdate_tick = 0;
	}

};

void VK_HideObject(vk_object *obj){
	VK_HideSprite(obj->s);
};

void VK_HideObjects(){
	GBA_HideSprites();
};


