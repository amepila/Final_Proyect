/*
 * Phases.c
 *
 *  Created on: Nov 17, 2017
 *      Author: Andres Hernandez
 */

#include "Display.h"
#include "Phases.h"
#include "States.h"
#include "I2CE2PROM.h"

#define INITIAL_POSITION	(0x2000U)
#define NEXT_POSITION		(0x0100U)
#define FINAL_POSITION		(0x1000U)
#define REGISTER_EMPTY		('\0')


/**ASCII table such as reference**/
typedef enum{
		ASCII_SPACE = 32,	ASCII_EXCLAMATION,	ASCII_QUOTATION,	ASCII_SHARP,		ASCII_DOLLAR,
		ASCII_PERCENT,		ASCII_AMPERSON,		ASCII_APOS,			ASCII_PAROPEN,		ASCII_PARCLOSE,
		ASCII_AST,			ASCII_PLUS,			ASCII_COMMA,		ASCII_MINUS,		ASCII_POINT,
		ASCII_DIAG,			ASCII_0,			ASCII_1,			ASCII_2,			ASCII_3,
		ASCII_4,			ASCII_5,			ASCII_6,			ASCII_7,			ASCII_8,
		ASCII_9,			ASCII_DOUBLEPOINT,	ASCII_COMMAPOINT,	ASCII_MINOR,		ASCII_EQUAL,
		ASCII_MAYOR,		ASCII_QUESTION,		ASCII_COMMAT,		ASCII_A,			ASCII_B,
		ASCII_C,			ASCII_D,			ASCII_E,			ASCII_F,			ASCII_G,
		ASCII_H,			ASCII_I,			ASCII_J,			ASCII_K,			ASCII_L,
		ASCII_M,			ASCII_N,			ASCII_O,			ASCII_P,			ASCII_Q,
		ASCII_R,			ASCII_S,			ASCII_T,			ASCII_U,			ASCII_V,
		ASCII_W,			ASCII_X,			ASCII_Y,			ASCII_Z,			ASCII_CORCHOPEN,
		ASCII_DIAG2,		ASCII_CORCHCLOSE,	ASCII_HAT,			ASCII_LOWBAR,		ASCII_GRAVE,
		ASCII_a,			ASCII_b,			ASCII_c,			ASCII_d,			ASCII_e,
		ASCII_f,			ASCII_g,			ASCII_h,			ASCII_i,			ASCII_j,
		ASCII_k,			ASCII_l,			ASCII_m,			ASCII_n,			ASCII_o,
		ASCII_p,			ASCII_q,			ASCII_r,			ASCII_s,			ASCII_t,
		ASCII_u,			ASCII_v,			ASCII_w,			ASCII_x,			ASCII_y,
		ASCII_z,			ASCII_BRACEOPEN,	ASCII_VERT,			ASCII_BRACECLOSE,	ASCII_TILDE
}ASCII_Code;

typedef enum{ASCII_CR = 13, ASCII_ESC = 27}ASCII_Special;
static uint32 AddressName;
static uint32 AddressNumber;
static uint32 NoContacts = 0;

void cleanContact(uint8 contact){
	uint8 counter;
	uint32 positionName;
	uint32 positionNumber;

	positionName = (contact * 100) - 100;
	for(counter = 0; counter < 10; counter++){
		writeMemory(positionName, REGISTER_EMPTY);
		E2PROMdelay(65000);
	}
	counter = 0;

	positionNumber = positionName + 2000;
	for(counter = 0; counter < 12; counter++){
		writeMemory(positionNumber, REGISTER_EMPTY);
		E2PROMdelay(65000);
	}
}


PhaseMainMenu_Type initialLoad(PhaseMainMenu_Type data){
	/**Create the variable with current data**/
	static PhaseMainMenu_Type currentMainMenu1;

	//Load The system and calibration of Compass
	//Frames LCD

	/**Set with the current state and phase**/
	currentMainMenu1.phaseState = GENERAL_VIEW;
	currentMainMenu1.stateMain = data.stateMain;

	return (currentMainMenu1);
}

PhaseMainMenu_Type generalView(PhaseMainMenu_Type data){

	/**Create the variable with current data**/
	static PhaseMainMenu_Type currentMainMenu2;

	/**Set with the current state and phase**/
	currentMainMenu2.phaseState = data.phaseState;
	currentMainMenu2.stateMain = data.stateMain;

	/**Detect when a key is pressed*/
	if(getUART0_flag()){
		/**Comparison between the pressed keys to continue with next main state**/
		if(getUART0_mailBox() == ASCII_1){currentMainMenu2.stateMain = CONTACTS;}
		if(getUART0_mailBox() == ASCII_2){currentMainMenu2.stateMain = MESSAGES;}
		if(getUART0_mailBox() == ASCII_3){currentMainMenu2.stateMain = SNAKE_GAME;}
		if(getUART0_mailBox() == ASCII_4){currentMainMenu2.stateMain = COMPASS;}
		if(getUART0_mailBox() == ASCII_5){currentMainMenu2.stateMain = WALLPAPER;}

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	/**Clear the mailbox**/
	clearUART0_mailbox();

	return (currentMainMenu2);
}

PhaseContacts_Type contactsMenu(PhaseContacts_Type data){

	/**Create the variable with current data**/
	static PhaseContacts_Type currentContacts1;

	/**Set with the current state and phase**/
	currentContacts1.phaseState = data.phaseState;
	currentContacts1.stateMain = data.stateMain;

	/**Detect when a key is pressed*/
	if(getUART0_flag()){
		/**Comparison between the pressed keys to continue with next main state**/
		if(getUART0_mailBox() == ASCII_1){currentContacts1.phaseState = VIEW_CONTACTS;}
		if(getUART0_mailBox() == ASCII_2){currentContacts1.phaseState = ADD_CONTACTS;}

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	/**Clear the mailbox**/
	clearUART0_mailbox();

	return (currentContacts1);
}

PhaseContacts_Type viewContacts(PhaseContacts_Type data){

	/**Create the variable with current data**/
	static PhaseContacts_Type currentContacts2;

	/**Set with the current state and phase**/
	currentContacts2.phaseState = data.phaseState;
	currentContacts2.stateMain = data.stateMain;

	/**Detect when a key is pressed*/
	if(getUART0_flag()){
		/**Comparison between the pressed keys to continue with next main state**/
		if(getUART0_mailBox() == ASCII_3){currentContacts2.phaseState = EDIT_CONTACTS;}

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	/**Clear the mailbox**/
	clearUART0_mailbox();

	return (currentContacts2);
}

PhaseContacts_Type addContacts(PhaseContacts_Type data){

	/**Create the variable with current data**/
	static PhaseContacts_Type currentContacts3;
	static uint8 flagField = 0;
	static uint8 counterSize_Name = 0;
	static uint8 counterSize_Number = 0;

	/**Set with the current state and phase**/
	currentContacts3.phaseState = data.phaseState;
	currentContacts3.stateMain = data.stateMain;

	/**Verifies if the registers are full**/
	if(readMemory((uint16)FINAL_POSITION) == REGISTER_EMPTY){
		/**Capture the Name field*/
		if((getUART0_flag()) && (flagField == 0)){

			currentContacts3.name[counterSize_Name] = getUART0_mailBox();
			counterSize_Name++;
			if(getUART0_mailBox() == ASCII_CR){
				flagField = 1;
				currentContacts3.sizeName = counterSize_Name - 1;
			}
			/**clear the reception flag*/
			setUART0_flag(FALSE);
		}

		/**Capture the Number field*/
		if((getUART0_flag()) && (flagField == 1)){

			currentContacts3.number[counterSize_Number] = getUART0_mailBox();
			counterSize_Number++;
			if(getUART0_mailBox() == ASCII_CR){
				flagField = 0;
				currentContacts3.sizeNumber = counterSize_Number - 1;
				currentContacts3.phaseState = SAVE_CONTACTS;
			}
			/**clear the reception flag*/
			setUART0_flag(FALSE);
		}
		/**Clear the mailbox**/
		clearUART0_mailbox();
	}else{
		//Message in display of registers are full
		//Press ESC to exit

		if(getUART0_flag()){
			if(getUART0_mailBox() == ASCII_ESC){
				currentContacts3.phaseState = VIEW_CONTACTS;
			}
			/**clear the reception flag*/
			setUART0_flag(FALSE);
		}
	}
	return (currentContacts3);
}

PhaseContacts_Type editContacts(PhaseContacts_Type data){

	/**Create the variable with current data**/
	static PhaseContacts_Type currentContacts4;

	/**Set with the current state and phase**/
	currentContacts4.phaseState = data.phaseState;
	currentContacts4.stateMain = data.stateMain;

	if(getUART0_flag()){
		if(getUART0_mailBox() == ASCII_0){cleanContact(1);}
		if(getUART0_mailBox() == ASCII_1){cleanContact(2);}
		if(getUART0_mailBox() == ASCII_2){cleanContact(3);}
		if(getUART0_mailBox() == ASCII_3){cleanContact(4);}
		if(getUART0_mailBox() == ASCII_4){cleanContact(5);}
		if(getUART0_mailBox() == ASCII_5){cleanContact(6);}
		if(getUART0_mailBox() == ASCII_6){cleanContact(7);}
		if(getUART0_mailBox() == ASCII_7){cleanContact(8);}
		if(getUART0_mailBox() == ASCII_8){cleanContact(9);}
		if(getUART0_mailBox() == ASCII_9){cleanContact(10);}

		currentContacts4.phaseState = ADD_CONTACTS;
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}

	return (currentContacts4);
}
PhaseContacts_Type saveContacts(PhaseContacts_Type data){

	/**Create the variable with current data**/
	static PhaseContacts_Type currentContacts5;
	static uint32 positionAddress_Name = 0;
	static uint32 positionAddress_Number = INITIAL_POSITION;
	uint8 counterSave;
	uint32 counterAddress = 0;

	/**Save the name into memory*/
	for(counterSave = data.sizeName; counterSave != 0; counterSave--){
		positionAddress_Name += counterAddress;
		writeMemory(positionAddress_Name,data.name[counterAddress]);
		counterAddress++;
		E2PROMdelay(65000);
	}
	AddressName = positionAddress_Name;
	positionAddress_Name += NEXT_POSITION;
	counterSave = 0;
	counterAddress = 0;

	/**Save the number into memory*/
	for(counterSave = data.sizeNumber; counterSave != 0; counterSave--){
		positionAddress_Number += counterAddress;
		writeMemory(positionAddress_Number,data.number[counterAddress]);
		counterAddress++;
		E2PROMdelay(65000);
	}
	NoContacts++;
	AddressNumber = positionAddress_Number;
	positionAddress_Number += NEXT_POSITION;
	currentContacts5.phaseState = CONTACT_MENU;
	currentContacts5.stateMain = CONTACTS;

	return (currentContacts5);
}

