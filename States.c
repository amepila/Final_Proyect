/*
 * Configurations.c
 *
 *  Created on: Nov 2, 2017
 *      Author: Andres Hernandez
 */

#include "Display.h"
#include "States.h"
#include "Phases.h"

const PhasePtrMainMenu_Type phasesMainMenu[4] =
{
		{initialLoad1},
		{initialLoad2},
		{generalView},
		{viewMenu}
};
const PhasePtrContacts_Type phasesContacts[5] =
{
		{contactsMenu},
		{viewContacts},
		{addContacts},
		{editContacts},
		{saveContacts}
};
const PhasePtrSnake_Type phasesSnake[6] =
{
		{startGame},
		{runGame},
		{gameLost},
		{showScore},
		{playAgain},
		{exitGame}
};
const PhasePtrCompass_Type phasesCompass[2] =
{
		{showCompass},
		{exitCompass}
};
const PhasePtrWallpaper_Type phasesWallpaper[2] =
{
		{viewWallpaper},
		{exitWallpaper}
};

States_MenuType stateMainMenu(void){

	static PhaseStateMenu_Type phase = INITIAL_LOAD1;
	static PhaseMainMenu_Type phaseMain;
	PhaseMainMenu_Type(*mainMenuFunctions)(PhaseMainMenu_Type);
	phaseMain.stateMain = MAIN_MENU;

	mainMenuFunctions = phasesMainMenu[phase].PhaseMainMenu;
	phaseMain = mainMenuFunctions(phaseMain);

	phase = phaseMain.phaseState;
	return (phaseMain.stateMain);
}

States_MenuType stateContacts(void){

	static PhaseStateContacts_Type phase = CONTACT_MENU;
	static PhaseContacts_Type phaseContacts;
	PhaseContacts_Type(*contactsFunctions)(PhaseContacts_Type);
	phaseContacts.stateMain = CONTACTS;

	contactsFunctions = phasesContacts[phase].PhaseContacts;
	phaseContacts = contactsFunctions(phaseContacts);

	phase = phaseContacts.phaseState;
	return (phaseContacts.stateMain);
}

States_MenuType stateMessages(void){

	States_MenuType MainState = MESSAGES;


	return (MainState);
}

States_MenuType stateSnakeGame(void){

	static PhaseStateSnake_Type phase = START_GAME;
	static PhaseSnake_Type phaseSnake;
	PhaseSnake_Type(*snakeFunctions)(PhaseSnake_Type);
	phaseSnake.stateMain = SNAKE_GAME;

	snakeFunctions = phasesSnake[phase].PhaseSnake;
	phaseSnake = snakeFunctions(phaseSnake);

	phase = phaseSnake.phaseState;
	return (phaseSnake.stateMain);
}

States_MenuType stateCompass(void){

	static PhaseStateCompass_Type phase = SHOW_COMPASS;
	static PhaseCompass_Type phaseCompass;
	PhaseCompass_Type(*compassFunctions)(PhaseCompass_Type);
	phaseCompass.stateMain = COMPASS;

	compassFunctions = phasesCompass[phase].PhaseCompass;
	phaseCompass = compassFunctions(phaseCompass);

	phase = phaseCompass.phaseState;
	return (phaseCompass.stateMain);
}

States_MenuType stateWallpaper(void){

	static PhaseStateWallpaper_Type phase = VIEW_WALLPAPER;
	static PhaseWallpaper_Type phaseWallpaper;
	PhaseWallpaper_Type(*wallpaperFunctions)(PhaseWallpaper_Type);
	phaseWallpaper.stateMain = WALLPAPER;

	wallpaperFunctions = phasesWallpaper[phase].PhaseWallpaper;
	phaseWallpaper = wallpaperFunctions(phaseWallpaper);

	phase = phaseWallpaper.phaseState;
	return (phaseWallpaper.stateMain);
}
