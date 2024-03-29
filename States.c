/*
 * Configurations.c
 *
 *  Created on: Nov 2, 2017
 *      Author: Andres Hernandez
 */

#include "States.h"
#include "Phases.h"

/**Pointer to functions of Main Menu*/
const PhasePtrMainMenu_Type phasesMainMenu[4] =
{
		{initialLoad1},
		{initialLoad2},
		{generalView},
		{viewMenu}
};

/**Pointer to functions of Messages*/
const PhasePtrMessages_Type phasesMessages[4] =
{
		{writeNumber},
		{writeMessages},
		{sendMessages},
		{exitMessages}
};

/**Pointer to functions of Contacts*/
const PhasePtrContacts_Type phasesContacts[4] =
{
		{contactsMenu},
		{viewContacts},
		{addContacts},
		{saveContacts}
};

/**Pointer to functions of Snake*/
const PhasePtrSnake_Type phasesSnake[6] =
{
		{startGame},
		{runGame},
		{gameLost},
		{showScore},
		{playAgain},
		{exitGame}
};

/**Pointer to functions of Compass*/
const PhasePtrCompass_Type phasesCompass[2] =
{
		{showCompass},
		{exitCompass}
};

/**Pointer to functions of Wallpapers*/
const PhasePtrWallpaper_Type phasesWallpaper[2] =
{
		{viewWallpaper},
		{exitWallpaper}
};

States_MenuType stateMainMenu(void){

	/**Initialization of state machine of Main Menu**/
	static PhaseStateMenu_Type phase = INITIAL_LOAD1;
	static PhaseMainMenu_Type phaseMain;
	PhaseMainMenu_Type(*mainMenuFunctions)(PhaseMainMenu_Type);
	phaseMain.stateMain = MAIN_MENU;

	/**Run the state machine and update the state**/
	mainMenuFunctions = phasesMainMenu[phase].PhaseMainMenu;
	phaseMain = mainMenuFunctions(phaseMain);

	/**Verified if there a change flow of program**/
	phase = phaseMain.phaseState;
	return (phaseMain.stateMain);
}

States_MenuType stateContacts(void){

	/**Initialization of state machine of Contacts**/
	static PhaseStateContacts_Type phase = CONTACT_MENU;
	static PhaseContacts_Type phaseContacts;
	PhaseContacts_Type(*contactsFunctions)(PhaseContacts_Type);
	phaseContacts.stateMain = CONTACTS;

	/**Run the state machine and update the state**/
	contactsFunctions = phasesContacts[phase].PhaseContacts;
	phaseContacts = contactsFunctions(phaseContacts);

	/**Verified if there a change flow of program**/
	phase = phaseContacts.phaseState;
	return (phaseContacts.stateMain);
}

States_MenuType stateMessages(void){

	/**Initialization of state machine of Messages**/
	static PhaseStateMessages_Type phase = WRITE_NUMBER;
	static PhaseMessages_Type phaseMessages;
	PhaseMessages_Type(*messagesFunctions)(PhaseMessages_Type);
	phaseMessages.stateMain = MESSAGES;

	/**Run the state machine and update the state**/
	messagesFunctions = phasesMessages[phase].PhaseMessages;
	phaseMessages = messagesFunctions(phaseMessages);

	/**Verified if there a change flow of program**/
	phase = phaseMessages.phaseState;
	return (phaseMessages.stateMain);
}

States_MenuType stateSnakeGame(void){

	/**Initialization of state machine of Snake game**/
	static PhaseStateSnake_Type phase = START_GAME;
	static PhaseSnake_Type phaseSnake;
	PhaseSnake_Type(*snakeFunctions)(PhaseSnake_Type);
	phaseSnake.stateMain = SNAKE_GAME;

	/**Run the state machine and update the state**/
	snakeFunctions = phasesSnake[phase].PhaseSnake;
	phaseSnake = snakeFunctions(phaseSnake);

	/**Verified if there a change flow of program**/
	phase = phaseSnake.phaseState;
	return (phaseSnake.stateMain);
}

States_MenuType stateCompass(void){

	/**Initialization of state machine of Compass**/
	static PhaseStateCompass_Type phase = SHOW_COMPASS;
	static PhaseCompass_Type phaseCompass;
	PhaseCompass_Type(*compassFunctions)(PhaseCompass_Type);
	phaseCompass.stateMain = COMPASS;

	/**Run the state machine and update the state**/
	compassFunctions = phasesCompass[phase].PhaseCompass;
	phaseCompass = compassFunctions(phaseCompass);

	/**Verified if there a change flow of program**/
	phase = phaseCompass.phaseState;
	return (phaseCompass.stateMain);
}

States_MenuType stateWallpaper(void){

	/**Initialization of state machine of Wallpaper**/
	static PhaseStateWallpaper_Type phase = VIEW_WALLPAPER;
	static PhaseWallpaper_Type phaseWallpaper;
	PhaseWallpaper_Type(*wallpaperFunctions)(PhaseWallpaper_Type);
	phaseWallpaper.stateMain = WALLPAPER;

	/**Run the state machine and update the state**/
	wallpaperFunctions = phasesWallpaper[phase].PhaseWallpaper;
	phaseWallpaper = wallpaperFunctions(phaseWallpaper);

	/**Verified if there a change flow of program**/
	phase = phaseWallpaper.phaseState;
	return (phaseWallpaper.stateMain);
}
