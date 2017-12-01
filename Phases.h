/*
 * Phases.h
 *
 *  Created on: Nov 17, 2017
 *      Author: Andres Hernandez
 */

#ifndef PHASES_H_
#define PHASES_H_

#include "DataTypeDefinitions.h"
#include "MK64F12.h"
#include "SPI.h"
#include "GPIO.h"
#include "I2C.h"
#include "UART.h"
#include "States.h"
#include "LCDNokia5110.h"

/************************************************************/
/**
 * \brief This data type define the data type in MAIN MENU
 */
typedef enum{INITIAL_LOAD1,
			INITIAL_LOAD2,
			GENERAL_VIEW,
			VIEW_MENU
}PhaseStateMenu_Type;

typedef struct{
	PhaseStateMenu_Type phaseState;
	States_MenuType stateMain;
}PhaseMainMenu_Type;

typedef PhaseMainMenu_Type(*fptrPhaseMainMenu)(PhaseMainMenu_Type);

typedef const struct PhaseMainMenu{
	PhaseMainMenu_Type(*PhaseMainMenu)(PhaseMainMenu_Type);
}PhasePtrMainMenu_Type;
/***********************************************************/
/***********************************************************/
/**
 * \brief This data type define the data type in MESSAGES
 */

typedef enum{MESSAGES_MENU,
			MAILBOX_MSG,
			WRITE_NAME,
			WRITE_MESSAGES,
			SEND_MESSAGES,
			EXIT_MESSAGES
}PhaseStateMessages_Type;

typedef struct{
	PhaseStateMessages_Type phaseState;
	States_MenuType stateMain;
}PhaseMessages_Type;

typedef PhaseMessages_Type(*fptrPhaseMessages)(PhaseMessages_Type);

typedef const struct PhaseMessages{
	PhaseMessages_Type(*PhaseMessages)(PhaseMessages_Type);
}PhasePtrMessages_Type;
/**********************************************************/
/***********************************************************/
/**
 * \brief This data type define the data type in CONTACTS
 */
typedef enum{MODE_ADD,MODE_EDIT}ModeContact_Type;

typedef enum{CONTACT_MENU,
			VIEW_CONTACTS,
			ADD_CONTACTS,
			EDIT_CONTACTS,
			SAVE_CONTACTS
}PhaseStateContacts_Type;

typedef struct{
	uint8 name[30];
	uint8 number[15];
	uint8 sizeName;
	uint8 sizeNumber;
	uint8 noContact;
	PhaseStateContacts_Type phaseState;
	States_MenuType stateMain;
}PhaseContacts_Type;

typedef PhaseContacts_Type(*fptrPhaseContacts)(PhaseContacts_Type);

typedef const struct PhaseContacts{
	PhaseContacts_Type(*PhaseContacts)(PhaseContacts_Type);
}PhasePtrContacts_Type;
/***********************************************************/
/***********************************************************/

/**
 * \brief This data type define the data type in SNAKE GAME
 */

typedef enum{START_GAME,
			RUN_GAME,
			GAME_LOST,
			SHOW_SCORE,
			PLAY_AGAIN,
			EXIT_GAME
}PhaseStateSnake_Type;

typedef struct{
	uint8 score;
	uint8 lives;
	uint8 nameSize;
	PhaseStateSnake_Type phaseState;
	States_MenuType stateMain;
}PhaseSnake_Type;

typedef PhaseSnake_Type(*fptrPhaseSnake)(PhaseSnake_Type);

typedef const struct PhaseSnake{
	PhaseSnake_Type(*PhaseSnake)(PhaseSnake_Type);
}PhasePtrSnake_Type;
/***********************************************************/
/***********************************************************/
/**
 * \brief This data type define the data type in COMPASS
 */

typedef enum{SHOW_COMPASS,
			EXIT_COMPASS
}PhaseStateCompass_Type;

typedef struct{
	PhaseStateCompass_Type phaseState;
	States_MenuType stateMain;
}PhaseCompass_Type;

typedef PhaseCompass_Type(*fptrPhaseCompass)(PhaseCompass_Type);

typedef const struct PhaseCompass{
	PhaseCompass_Type(*PhaseCompass)(PhaseCompass_Type);
}PhasePtrCompass_Type;
/***********************************************************/
/***********************************************************/
/**
 * \brief This data type define the data type in WALLPAPER
 */

typedef enum{VIEW_WALLPAPER,
			EXIT_WALLPAPER
}PhaseStateWallpaper_Type;

typedef struct{
	PhaseStateWallpaper_Type phaseState;
	States_MenuType stateMain;
}PhaseWallpaper_Type;

typedef PhaseWallpaper_Type(*fptrPhaseWallpaper)(PhaseWallpaper_Type);

typedef const struct PhaseWallpaper{
	PhaseWallpaper_Type(*PhaseWallpaper)(PhaseWallpaper_Type);
}PhasePtrWallpaper_Type;
/**********************************************************/
/***********************************************************/

void cleanContact(uint8 contact);

PhaseMainMenu_Type initialLoad1(PhaseMainMenu_Type data);
PhaseMainMenu_Type initialLoad2(PhaseMainMenu_Type data);
PhaseMainMenu_Type generalView(PhaseMainMenu_Type data);
PhaseMainMenu_Type viewMenu(PhaseMainMenu_Type data);

PhaseMessages_Type messagesMenu(PhaseMessages_Type data);
PhaseMessages_Type mailBoxMSG(PhaseMessages_Type data);
PhaseMessages_Type writeName(PhaseMessages_Type data);
PhaseMessages_Type writeMessages(PhaseMessages_Type data);
PhaseMessages_Type sendMessages(PhaseMessages_Type data);
PhaseMessages_Type exitMessages(PhaseMessages_Type data);

PhaseContacts_Type contactsMenu(PhaseContacts_Type data);
PhaseContacts_Type viewContacts(PhaseContacts_Type data);
PhaseContacts_Type addContacts(PhaseContacts_Type data);
PhaseContacts_Type editContacts(PhaseContacts_Type data);
PhaseContacts_Type saveContacts(PhaseContacts_Type data);

PhaseSnake_Type startGame(PhaseSnake_Type data);
PhaseSnake_Type runGame(PhaseSnake_Type data);
PhaseSnake_Type gameLost(PhaseSnake_Type data);
PhaseSnake_Type showScore(PhaseSnake_Type data);
PhaseSnake_Type playAgain(PhaseSnake_Type data);
PhaseSnake_Type exitGame(PhaseSnake_Type data);

PhaseCompass_Type showCompass(PhaseCompass_Type data);
PhaseCompass_Type exitCompass(PhaseCompass_Type data);

PhaseWallpaper_Type viewWallpaper(PhaseWallpaper_Type data);
PhaseWallpaper_Type exitWallpaper(PhaseWallpaper_Type data);


#endif /* PHASES_H_ */
