/*
 * Frames.h
 *
 *  Created on: Nov 22, 2017
 *      Author: Andres Hernandez
 */

#ifndef FRAMES_H_
#define FRAMES_H_

#include "DataTypeDefinitions.h"

typedef enum{CHIP_MENU,
			HORDA_MENU,
			MUSIC_MENU,
			PIKACHU_MENU,
			ANDROID_MENU
}Wall_Type;

typedef Wall_Type(*fptrStateWall)(void);

typedef const struct StateWall{
	Wall_Type(*stateWall)(void);
}StateWall_Type;

/**************************************************************************************************/
typedef enum{CHIP,
			HORDA,
			MUSIC,
			PIKACHU,
			ANDROID
}Images_Type;

typedef Images_Type(*fptrStateImages)(void);

typedef const struct StateImages{
	Images_Type(*stateImages)(void);
}StateImages_Type;

/*************************************************************************************************/
typedef enum{MESSAGES_MAIN,
			CONTACTS_MAIN,
			SNAKE_MAIN,
			COMPASS_MAIN,
			WALLPAPER_MAIN
}WallMenu_Type;

typedef WallMenu_Type(*fptrStateWallMenu)(void);

typedef const struct StateWallMenu{
	WallMenu_Type(*stateWallMenu)(void);
}StateWallMenu_Type;
/*************************************************************************************************/


uint8 printTestFrame(void);

uint8 printFrame1(void);

uint8 printFrame2(void);

uint8 printFrame3(void);

uint8 printFrame4(void);

uint8 printFrame5(void);

uint8 printFrame6(void);

uint8 printFrame7(void);

uint8 printFrame8(void);

Images_Type printChipFrame(void);

Images_Type printHordaFrame(void);

Images_Type printMusicFrame(void);

Images_Type printPikachuFrame(void);

Images_Type printAndroidFrame(void);

uint8 printSnakeGameFrame(void);

uint8 printGameOVer(void);

uint8 printStart(void);

uint8 printDiedGame(void);

Wall_Type printAndroidMenu(void);

Wall_Type printChipMenu(void);

Wall_Type printPikachuMenu(void);

Wall_Type printMusicMenu(void);

Wall_Type printHordaMenu(void);

WallMenu_Type printMessagesMain(void);

WallMenu_Type printContactsMain(void);

WallMenu_Type printSnakeMain(void);

WallMenu_Type printCompassMain(void);

WallMenu_Type printWallpaperMain(void);

uint8 printLoading(void);

uint8 printLoading2(void);


#endif /* FRAMES_H_ */
