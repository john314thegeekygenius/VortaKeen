
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


int main(){

	VK_SetupSound();

	VK_SetupVideo();
	
	VK_MainEngine();
		
	// Loop until the end of time.
	while(1){};

	return 0;
};

