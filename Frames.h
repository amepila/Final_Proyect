/*
 * Frames.h
 *
 *  Created on: Nov 22, 2017
 *      Author: Andres Hernandez
 */

#ifndef FRAMES_H_
#define FRAMES_H_

#include "DataTypeDefinitions.h"

typedef enum{TESTFRAME,		FRAME1,		FRAME2,		FRAME3,		FRAME4, 	FRAME5, 	FRAME6,		FRAME7,		FRAME8,
			CHIP,		HORDA,		MUSIC,		PIKACHU,	ANDROID,	SNAKEGAME,	GAMEOVER,	START,		DIEDGAME,
			ANDROID_MENU,			HORDA_MENU,				MUSIC_MENU,				PIKACHU_MENU,			CHIP_MENU,
			MESSAGES_MAIN,			CONTACTS_MAIN,			SNAKE_MAIN,				WALLPAPER_MENU,			COMPASS_MAIN
}Frame_Type;

typedef Frame_Type(*fptrStateFrame)(void);

typedef const struct StateFrame{
	Frame_Type(*stateFrame)(void);
}StateFrame_Type;

Frame_Type printTestFrame(void);

Frame_Type printFrame1(void);

Frame_Type printFrame2(void);

Frame_Type printFrame3(void);

Frame_Type printFrame4(void);

Frame_Type printFrame5(void);

Frame_Type printFrame6(void);

Frame_Type printFrame7(void);

Frame_Type printFrame8(void);

Frame_Type printChipFrame(void);

Frame_Type printHordaFrame(void);

Frame_Type printMusicFrame(void);

Frame_Type printPikachuFrame(void);

Frame_Type printAndroidFrame(void);

Frame_Type printSnakeGameFrame(void);

Frame_Type printGameOVer(void);

Frame_Type printStart(void);

Frame_Type printDiedGame(void);

Frame_Type printAndroidMenu(void);

Frame_Type printChipMenu(void);

Frame_Type printPikachuMenu(void);

Frame_Type printMusicMenu(void);

Frame_Type printHordaMenu(void);

Frame_Type printMessagesMain(void);

Frame_Type printContactsMain(void);

Frame_Type printSnakeMain(void);

Frame_Type printCompassMain(void);

Frame_Type printWallpaperMain(void);

uint8 printLoading(void);

uint8 printLoading2(void);


#endif /* FRAMES_H_ */
