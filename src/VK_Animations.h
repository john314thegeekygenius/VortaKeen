
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

const uint16_t VK_1_FRAMES = 0x10;
const uint16_t VK_2_FRAMES = 0x20;
const uint16_t VK_4_FRAMES = 0x40;
const uint16_t VK_8_FRAMES = 0x80;

vk_obj_ani VKA_yorp_idle_1,
			VKA_yorp_idle_2,
			VKA_yorp_idle_3,
			VKA_yorp_idle_4;

vk_obj_ani VKA_yorp_idle_1 = { 0, VK_2_FRAMES, &VKA_yorp_idle_2 };
vk_obj_ani VKA_yorp_idle_2 = { 1, VK_2_FRAMES, &VKA_yorp_idle_3 };
vk_obj_ani VKA_yorp_idle_3 = { 2, VK_2_FRAMES, &VKA_yorp_idle_4 };
vk_obj_ani VKA_yorp_idle_4 = { 3, VK_2_FRAMES, &VKA_yorp_idle_1 };


