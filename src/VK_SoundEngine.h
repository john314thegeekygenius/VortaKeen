
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

#ifndef __VK_SOUND_ENG__
#define __VK_SOUND_ENG__


GBA_ARM void VK_SoundHandler();
void VK_SetupSound();
void VK_PlaySoundIRQ(unsigned short *data,unsigned short len, unsigned short priority);
void VK_PlaySound(unsigned short soundid);
int VK_SoundDone();

#endif
