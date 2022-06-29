
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

#include "../sound/SOUND00.h"
#include "../sound/SOUND01.h"
#include "../sound/SOUND02.h"
#include "../sound/SOUND03.h"
#include "../sound/SOUND04.h"
#include "../sound/SOUND05.h"
#include "../sound/SOUND06.h"
#include "../sound/SOUND07.h"
#include "../sound/SOUND08.h"
#include "../sound/SOUND09.h"
#include "../sound/SOUND10.h"
#include "../sound/SOUND11.h"
#include "../sound/SOUND12.h"
#include "../sound/SOUND13.h"
#include "../sound/SOUND14.h"
#include "../sound/SOUND15.h"
#include "../sound/SOUND16.h"
#include "../sound/SOUND17.h"
#include "../sound/SOUND18.h"
#include "../sound/SOUND19.h"
#include "../sound/SOUND20.h"
#include "../sound/SOUND21.h"
#include "../sound/SOUND22.h"
#include "../sound/SOUND23.h"
#include "../sound/SOUND24.h"
#include "../sound/SOUND25.h"
#include "../sound/SOUND26.h"
#include "../sound/SOUND27.h"
#include "../sound/SOUND28.h"
#include "../sound/SOUND29.h"
#include "../sound/SOUND30.h"
#include "../sound/SOUND31.h"
#include "../sound/SOUND32.h"
#include "../sound/SOUND33.h"
#include "../sound/SOUND34.h"
#include "../sound/SOUND35.h"
#include "../sound/SOUND36.h"
#include "../sound/SOUND37.h"
#include "../sound/SOUND38.h"
#include "../sound/SOUND39.h"
#include "../sound/SOUND40.h"
#include "../sound/SOUND41.h"
#include "../sound/SOUND42.h"
#include "../sound/SOUND43.h"
#include "../sound/SOUND44.h"
#include "../sound/SOUND45.h"
#include "../sound/SOUND46.h"
#include "../sound/SOUND47.h"
#include "../sound/SOUND48.h"
#include "../sound/SOUND49.h"
#include "../sound/SOUND50.h"
#include "../sound/SOUND51.h"
#include "../sound/SOUND52.h"
#include "../sound/SOUND53.h"
#include "../sound/SOUND54.h"
#include "../sound/SOUND55.h"
#include "../sound/SOUND56.h"
#include "../sound/SOUND57.h"
#include "../sound/SOUND58.h"
#include "../sound/SOUND59.h"


volatile unsigned short * vk_sound_play_int_data = (unsigned short*)NULL;
volatile unsigned short vk_sound_play_loc = 0;
volatile unsigned short vk_sound_play_len = 0;
volatile unsigned short vk_sound_delay_tick = 0;
unsigned short vk_sound_priority = 0;
short vk_cur_sound_id = -1;


GBA_ARM void VK_SoundHandler(){
		
	/* disable interrupts for now and save current state of interrupt */
	*(volatile uint16_t*)GBA_INT_ENABLE = 0;

	vk_sound_delay_tick = 0;
	if(vk_sound_play_int_data != (unsigned short*)NULL){
		unsigned short fq = 0;
		if(vk_sound_play_int_data[vk_sound_play_loc] > 0){
			unsigned int f = vk_sound_play_int_data[vk_sound_play_loc];
			unsigned short fd = f/9;
			if(fd > 2048){
				fd = 2048;
			}
			if(fd<1){
				fd = 1;
			}
			fq = 2048-fd;

			//enable sound 1 to left and right
			*(volatile uint16_t*)GBA_SOUNDCNT_L |= GBA_SND_1L | GBA_SND_1R ;

			// set the frequency
			*(volatile uint16_t*)GBA_SOUND1_X = fq | GBA_RESET_SOUND;
		}else{
			//disable sound 1 to left and right
			*(volatile uint16_t*)GBA_SOUNDCNT_L &= ~(GBA_SND_1L | GBA_SND_1R);
		}

		vk_sound_play_loc ++;
		if(vk_sound_play_loc > vk_sound_play_len){
			// done
			vk_sound_play_loc = 0;
			
			vk_sound_play_int_data = (unsigned short*)NULL;
			vk_cur_sound_id = -1;
			vk_sound_priority = 0;
			
			
			//disable sound 1 to left and right
			*(volatile uint16_t*)GBA_SOUNDCNT_L &= ~(GBA_SND_1L | GBA_SND_1R);
		}
	}	

	// Make it update every 1/140 of a second
	*(volatile unsigned short*)GBA_TIMER2_DATA = 0xFF8C;

    // restore/enable interrupts 
    *(volatile uint16_t*)GBA_INT_ACK = GBA_INT_TIMER2;
	*(volatile uint16_t*)GBA_INT_ENABLE = 1;
};

void VK_SetupSound(){
	GBA_InitAudio();
	GBA_SetSoundSqWav(1);

	*(volatile unsigned int*)GBA_REG_DISPCNT = GBA_MODE_3 | GBA_ENABLE_BG2;// | GBA_SHOW_BACK;

	// Setup interupt to play sounds

	*(volatile uint16_t*)GBA_INT_ENABLE = 0;
    *(volatile unsigned int *)GBA_INT_CALLBACK = (unsigned int) &VK_SoundHandler;
    *(volatile uint16_t*)GBA_INT_SELECT |= GBA_INT_TIMER2;
	*(volatile uint16_t*)GBA_INT_ENABLE = 1;

	// Make it update every 1/140 of a second
	*(volatile unsigned short*)GBA_TIMER2_DATA = 0xFF8C;
	*(volatile unsigned short*)GBA_TIMER2_CONTROL = GBA_TIMER_ENABLE | GBA_TIMER_FREQ_256 | GBA_TIMER_INTERUPT;

	vk_cur_sound_id = -1;
	vk_sound_priority = 0;
};


void VK_PlaySoundIRQ(unsigned short *data,unsigned short len, unsigned short priority){
	if(vk_sound_play_int_data==(unsigned short*)NULL || vk_sound_priority <= priority){
		vk_sound_play_int_data = (unsigned short*)data;
		vk_sound_priority = priority;
		vk_sound_play_len = len;
		vk_sound_play_loc = 0;
		vk_sound_delay_tick = 0;
	}

};

int VK_SoundDone(){
	if(vk_sound_play_int_data!=NULL){
		return 1+vk_sound_play_loc;
	}
	return 0;
};

void VK_PlaySound(unsigned short soundid){
	unsigned short *sndptr = NULL;
	unsigned short sndlen = 0;
	unsigned short sndprio = 0;

	// Don't play the same sound again?
	//if(vk_cur_sound_id == soundid)
	//	return;
			
	switch(soundid){
		case 0:
			sndptr = VK_SOUND_00_data;
			sndlen = VK_SOUND_00_length;
			sndprio = VK_SOUND_00_PRIO;
		break;
		case 1:
			sndptr = VK_SOUND_01_data;
			sndlen = VK_SOUND_01_length;
			sndprio = VK_SOUND_01_PRIO;
		break;
		case 2:
			sndptr = VK_SOUND_02_data;
			sndlen = VK_SOUND_02_length;
			sndprio = VK_SOUND_02_PRIO;
		break;
		case 3:
			sndptr = VK_SOUND_03_data;
			sndlen = VK_SOUND_03_length;
			sndprio = VK_SOUND_03_PRIO;
		break;
		case 4:
			sndptr = VK_SOUND_04_data;
			sndlen = VK_SOUND_04_length;
			sndprio = VK_SOUND_04_PRIO;
		break;
		case 5:
			sndptr = VK_SOUND_05_data;
			sndlen = VK_SOUND_05_length;
			sndprio = VK_SOUND_05_PRIO;
		break;
		case 6:
			sndptr = VK_SOUND_06_data;
			sndlen = VK_SOUND_06_length;
			sndprio = VK_SOUND_06_PRIO;
		break;
		case 7:
			sndptr = VK_SOUND_07_data;
			sndlen = VK_SOUND_07_length;
			sndprio = VK_SOUND_07_PRIO;
		break;
		case 8:
			sndptr = VK_SOUND_08_data;
			sndlen = VK_SOUND_08_length;
			sndprio = VK_SOUND_08_PRIO;
		break;
		case 9:
			sndptr = VK_SOUND_09_data;
			sndlen = VK_SOUND_09_length;
			sndprio = VK_SOUND_09_PRIO;
		break;
		case 10:
			sndptr = VK_SOUND_10_data;
			sndlen = VK_SOUND_10_length;
			sndprio = VK_SOUND_10_PRIO;
		break;
		case 11:
			sndptr = VK_SOUND_11_data;
			sndlen = VK_SOUND_11_length;
			sndprio = VK_SOUND_11_PRIO;
		break;
		case 12:
			sndptr = VK_SOUND_12_data;
			sndlen = VK_SOUND_12_length;
			sndprio = VK_SOUND_12_PRIO;
		break;
		case 13:
			sndptr = VK_SOUND_13_data;
			sndlen = VK_SOUND_13_length;
			sndprio = VK_SOUND_13_PRIO;
		break;
		case 14:
			sndptr = VK_SOUND_14_data;
			sndlen = VK_SOUND_14_length;
			sndprio = VK_SOUND_14_PRIO;
		break;
		case 15:
			sndptr = VK_SOUND_15_data;
			sndlen = VK_SOUND_15_length;
			sndprio = VK_SOUND_15_PRIO;
		break;
		case 16:
			sndptr = VK_SOUND_16_data;
			sndlen = VK_SOUND_16_length;
			sndprio = VK_SOUND_16_PRIO;
		break;
		case 17:
			sndptr = VK_SOUND_17_data;
			sndlen = VK_SOUND_17_length;
			sndprio = VK_SOUND_17_PRIO;
		break;
		case 18:
			sndptr = VK_SOUND_18_data;
			sndlen = VK_SOUND_18_length;
			sndprio = VK_SOUND_18_PRIO;
		break;
		case 19:
			sndptr = VK_SOUND_19_data;
			sndlen = VK_SOUND_19_length;
			sndprio = VK_SOUND_19_PRIO;
		break;
		case 20:
			sndptr = VK_SOUND_20_data;
			sndlen = VK_SOUND_20_length;
			sndprio = VK_SOUND_20_PRIO;
		break;
		case 21:
			sndptr = VK_SOUND_21_data;
			sndlen = VK_SOUND_21_length;
			sndprio = VK_SOUND_21_PRIO;
		break;
		case 22:
			sndptr = VK_SOUND_22_data;
			sndlen = VK_SOUND_22_length;
			sndprio = VK_SOUND_22_PRIO;
		break;
		case 23:
			sndptr = VK_SOUND_23_data;
			sndlen = VK_SOUND_23_length;
			sndprio = VK_SOUND_23_PRIO;
		break;
		case 24:
			sndptr = VK_SOUND_24_data;
			sndlen = VK_SOUND_24_length;
			sndprio = VK_SOUND_24_PRIO;
		break;
		case 25:
			sndptr = VK_SOUND_25_data;
			sndlen = VK_SOUND_25_length;
			sndprio = VK_SOUND_25_PRIO;
		break;
		case 26:
			sndptr = VK_SOUND_26_data;
			sndlen = VK_SOUND_26_length;
			sndprio = VK_SOUND_26_PRIO;
		break;
		case 27:
			sndptr = VK_SOUND_27_data;
			sndlen = VK_SOUND_27_length;
			sndprio = VK_SOUND_27_PRIO;
		break;
		case 28:
			sndptr = VK_SOUND_28_data;
			sndlen = VK_SOUND_28_length;
			sndprio = VK_SOUND_28_PRIO;
		break;
		case 29:
			sndptr = VK_SOUND_29_data;
			sndlen = VK_SOUND_29_length;
			sndprio = VK_SOUND_29_PRIO;
		break;
		case 30:
			sndptr = VK_SOUND_30_data;
			sndlen = VK_SOUND_30_length;
			sndprio = VK_SOUND_30_PRIO;
		break;
		case 31:
			sndptr = VK_SOUND_31_data;
			sndlen = VK_SOUND_31_length;
			sndprio = VK_SOUND_31_PRIO;
		break;
		case 32:
			sndptr = VK_SOUND_32_data;
			sndlen = VK_SOUND_32_length;
			sndprio = VK_SOUND_32_PRIO;
		break;
		case 33:
			sndptr = VK_SOUND_33_data;
			sndlen = VK_SOUND_33_length;
			sndprio = VK_SOUND_33_PRIO;
		break;
		case 34:
			sndptr = VK_SOUND_34_data;
			sndlen = VK_SOUND_34_length;
			sndprio = VK_SOUND_34_PRIO;
		break;
		case 35:
			sndptr = VK_SOUND_35_data;
			sndlen = VK_SOUND_35_length;
			sndprio = VK_SOUND_35_PRIO;
		break;
		case 36:
			sndptr = VK_SOUND_36_data;
			sndlen = VK_SOUND_36_length;
			sndprio = VK_SOUND_36_PRIO;
		break;
		case 37:
			sndptr = VK_SOUND_37_data;
			sndlen = VK_SOUND_37_length;
			sndprio = VK_SOUND_37_PRIO;
		break;
		case 38:
			sndptr = VK_SOUND_38_data;
			sndlen = VK_SOUND_38_length;
			sndprio = VK_SOUND_38_PRIO;
		break;
		case 39:
			sndptr = VK_SOUND_39_data;
			sndlen = VK_SOUND_39_length;
			sndprio = VK_SOUND_39_PRIO;
		break;
		case 40:
			sndptr = VK_SOUND_40_data;
			sndlen = VK_SOUND_40_length;
			sndprio = VK_SOUND_40_PRIO;
		break;
		case 41:
			sndptr = VK_SOUND_41_data;
			sndlen = VK_SOUND_41_length;
			sndprio = VK_SOUND_41_PRIO;
		break;
		case 42:
			sndptr = VK_SOUND_42_data;
			sndlen = VK_SOUND_42_length;
			sndprio = VK_SOUND_42_PRIO;
		break;
		case 43:
			sndptr = VK_SOUND_43_data;
			sndlen = VK_SOUND_43_length;
			sndprio = VK_SOUND_43_PRIO;
		break;
		case 44:
			sndptr = VK_SOUND_44_data;
			sndlen = VK_SOUND_44_length;
			sndprio = VK_SOUND_44_PRIO;
		break;
		case 45:
			sndptr = VK_SOUND_45_data;
			sndlen = VK_SOUND_45_length;
			sndprio = VK_SOUND_45_PRIO;
		break;
		case 46:
			sndptr = VK_SOUND_46_data;
			sndlen = VK_SOUND_46_length;
			sndprio = VK_SOUND_46_PRIO;
		break;
		case 47:
			sndptr = VK_SOUND_47_data;
			sndlen = VK_SOUND_47_length;
			sndprio = VK_SOUND_47_PRIO;
		break;
		case 48:
			sndptr = VK_SOUND_48_data;
			sndlen = VK_SOUND_48_length;
			sndprio = VK_SOUND_48_PRIO;
		break;
		case 49:
			sndptr = VK_SOUND_49_data;
			sndlen = VK_SOUND_49_length;
			sndprio = VK_SOUND_49_PRIO;
		break;
		case 50:
			sndptr = VK_SOUND_50_data;
			sndlen = VK_SOUND_50_length;
			sndprio = VK_SOUND_50_PRIO;
		break;
		case 51:
			sndptr = VK_SOUND_51_data;
			sndlen = VK_SOUND_51_length;
			sndprio = VK_SOUND_51_PRIO;
		break;
		case 52:
			sndptr = VK_SOUND_52_data;
			sndlen = VK_SOUND_52_length;
			sndprio = VK_SOUND_52_PRIO;
		break;
		case 53:
			sndptr = VK_SOUND_53_data;
			sndlen = VK_SOUND_53_length;
			sndprio = VK_SOUND_53_PRIO;
		break;
		case 54:
			sndptr = VK_SOUND_54_data;
			sndlen = VK_SOUND_54_length;
			sndprio = VK_SOUND_54_PRIO;
		break;
		case 55:
			sndptr = VK_SOUND_55_data;
			sndlen = VK_SOUND_55_length;
			sndprio = VK_SOUND_55_PRIO;
		break;
		case 56:
			sndptr = VK_SOUND_56_data;
			sndlen = VK_SOUND_56_length;
			sndprio = VK_SOUND_56_PRIO;
		break;
		case 57:
			sndptr = VK_SOUND_57_data;
			sndlen = VK_SOUND_57_length;
			sndprio = VK_SOUND_57_PRIO;
		break;
		case 58:
			sndptr = VK_SOUND_58_data;
			sndlen = VK_SOUND_58_length;
			sndprio = VK_SOUND_58_PRIO;
		break;
		case 59:
			sndptr = VK_SOUND_59_data;
			sndlen = VK_SOUND_59_length;
			sndprio = VK_SOUND_59_PRIO;
		break;
	}

	if(vk_sound_priority > sndprio) 
		return;

	vk_cur_sound_id = soundid;
	
	VK_PlaySoundIRQ(sndptr, sndlen, sndprio);
};


void VK_StopSound(){
	vk_sound_play_loc = 0;
	
	vk_sound_play_int_data = (unsigned short*)NULL;
	vk_cur_sound_id = -1;
	vk_sound_priority = 0;
	
	
	//disable sound 1 to left and right
	*(volatile uint16_t*)GBA_SOUNDCNT_L &= ~(GBA_SND_1L | GBA_SND_1R);
};
