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


/**
 * \brief This data type define the data type in MAIN MENU
 */
typedef enum{INITIAL_LOAD,
			GENERAL_VIEW
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
/**
 * \brief This data type define the data type in CONTACTS
 */
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
	PhaseStateContacts_Type phaseState;
	States_MenuType stateMain;
}PhaseContacts_Type;

typedef PhaseContacts_Type(*fptrPhaseContacts)(PhaseContacts_Type);

typedef const struct PhaseContacts{
	PhaseContacts_Type(*PhaseContacts)(PhaseContacts_Type);
}PhasePtrContacts_Type;
/***********************************************************/

void cleanContact(uint8 contact);

PhaseMainMenu_Type initialLoad(PhaseMainMenu_Type data);
PhaseMainMenu_Type generalView(PhaseMainMenu_Type data);

PhaseContacts_Type contactsMenu(PhaseContacts_Type data);
PhaseContacts_Type viewContacts(PhaseContacts_Type data);
PhaseContacts_Type addContacts(PhaseContacts_Type data);
PhaseContacts_Type editContacts(PhaseContacts_Type data);
PhaseContacts_Type saveContacts(PhaseContacts_Type data);








#endif /* PHASES_H_ */
