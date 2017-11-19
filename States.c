/*
 * Configurations.c
 *
 *  Created on: Nov 2, 2017
 *      Author: Andres Hernandez
 */

#include "Display.h"
#include "States.h"
#include "Phases.h"

const PhasePtrMainMenu_Type phasesMainMenu[2] =
{
		{generalView},
		{initialLoad}
};
const PhasePtrContacts_Type phasesContacts[5] =
{
		{contactsMenu},
		{viewContacts},
		{addContacts},
		{editContacts},
		{saveContacts}
};

States_MenuType stateMainMenu(void){

	static PhaseStateMenu_Type phase = INITIAL_LOAD;
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

	States_MenuType MainState = SNAKE_GAME;


	return (MainState);
}

States_MenuType stateCompass(void){

	States_MenuType MainState = COMPASS;


	return (MainState);
}

States_MenuType stateWallpaper(void){

	States_MenuType MainState = WALLPAPER;


	return (MainState);
}
