/*
 * Configurations.c
 *
 *  Created on: Nov 2, 2017
 *      Author: Andres Hernandez
 */

#include "Display.h"
#include "States.h"
#include "Phases.h"

const PhasePtrMainMenu_Type phasesMainMenu[3] =
{
		{initialLoad1},
		{initialLoad2},
		{generalView}
};
const PhasePtrContacts_Type phasesContacts[5] =
{
		{contactsMenu},
		{viewContacts},
		{addContacts},
		{editContacts},
		{saveContacts}
};
const PhasePtrSnake_Type phasesSnake[3] =
{
		{startGame},
		{runGame},
		{exitGame}
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
	PhaseSnake_Type(*contactsSnake)(PhaseSnake_Type);
	phaseSnake.stateMain = SNAKE_GAME;

	contactsSnake = phasesSnake[phase].PhaseSnake;
	phaseSnake = contactsSnake(phaseSnake);

	phase = phaseSnake.phaseState;
	return (phaseSnake.stateMain);
}

States_MenuType stateCompass(void){

	States_MenuType MainState = COMPASS;


	return (MainState);
}

States_MenuType stateWallpaper(void){

	States_MenuType MainState = WALLPAPER;


	return (MainState);
}
