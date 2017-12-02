/*
 * Phases.c
 *
 *  Created on: Nov 17, 2017
 *      Author: Andres Hernandez
 */

#include "Display.h"
#include "Phases.h"
#include "I2CE2PROM.h"
#include "SnakeGame.h"
#include "Frames.h"
#include "Magnetometer.h"
#include "DirectionPicture.h"

#define POSITION_NAME		(0U)
#define POSITION_NUMBER 	(2000U)
#define POSITION_CONTACTS	(3000U)
#define POSITION_GAMERS		(3010U)
#define POSITION_SCORE		(4000U)
#define NEXT_POSITION		(100U)
#define FINAL_POSITION		(1000U)
#define REGISTER_EMPTY		('\0')
#define FULL_CONTACTS		(9U)

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

const uint8 ASCII_Alphabet[27] = {
		'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',32
};
const uint8 ASCII_Numbers[10] = {
		'0','1','2','3','4','5','6','7','8','9'
};

typedef enum{ASCII_CR = 13, ASCII_ESC = 27}ASCII_Special;
const uint8 ReadMsg[] = "AT+CMGR";
const uint8 SendMsg[] = "AT+CMGS";
const uint8 MsgScore[] = "Your score is: ";
const uint8 MsgPlay[] = "Play again?";
const uint8 MsgConfirm[] = "Yes / No";
const uint8 NameField[] = "Name: ";
const uint8 NumberField[] = "Number: ";
const uint8 MessageField[] = "Message: ";
const uint8 SaveNewContact[] = "Save";
const uint8 CancelEdit[] = "Cancel";
const uint8 EmptyContacts[] = "(Empty)";
const uint8 FullContacts[] = "Full!!";
const uint8 SelectBotton[] = "SEL";
const uint8 BackBotton[] = "BACK";
static uint32 CurrentAddress_Name = 0;
static uint32 CurrentAddress_Number = 0;
static uint32 NoContacts = 0;
static ModeContact_Type ModeContact = MODE_ADD;
static Wall_Type CurrentWallpaper = ANDROID_MENU;
static uint8 FlagChange_Wallpaper = TRUE;
static uint8 FlagSnakeInit = TRUE;
static FIFO_Type Name;
static FIFO_Type Message;

const StateWall_Type StateWallpaper[5] =
{
		{printChipMenu},
		{printHordaMenu},
		{printMusicMenu},
		{printPikachuMenu},
		{printAndroidMenu}
};

const StateWallMenu_Type StateMenus[5] =
{
		{printMessagesMain},
		{printContactsMain},
		{printSnakeMain},
		{printCompassMain},
		{printWallpaperMain}
};

const StateImages_Type StateImages[5] =
{
		{printChipFrame},
		{printHordaFrame},
		{printMusicFrame},
		{printPikachuFrame},
		{printAndroidFrame}
};

const StateShowContact_Type StateShowContacts[10] =
{
		{contactNumber1},
		{contactNumber2},
		{contactNumber3},
		{contactNumber4},
		{contactNumber5},
		{contactNumber6},
		{contactNumber7},
		{contactNumber8},
		{contactNumber9},
		{contactNumber10}
};

ShowContact_Type contactNumber1(void){

	uint8 counterChar;
	uint8 watchChar = 0;

	LCDNokia_gotoXY(1,0);
	LCDNokia_sendString((uint8*)NameField);
	LCDNokia_gotoXY(1,1);

	for(counterChar = 0; counterChar != 50; counterChar++){
		LCDNokia_sendChar(readMemory(POSITION_NAME + counterChar));
		watchChar = readMemory(POSITION_NAME + counterChar);
		if(watchChar == '\0'){counterChar = 50;}
	}

	LCDNokia_gotoXY(1,2);
	LCDNokia_sendString((uint8*)NumberField);
	LCDNokia_gotoXY(1,3);

	for(counterChar = 0; counterChar != 50; counterChar++){
		LCDNokia_sendChar(readMemory(POSITION_NUMBER + counterChar));
		watchChar = readMemory(POSITION_NUMBER + counterChar);
		if(watchChar == '\0'){counterChar = 50;}
	}

	LCDNokia_gotoXY(1,5);
	LCDNokia_sendString((uint8*)SelectBotton);
	LCDNokia_gotoXY(50,5);
	LCDNokia_sendString((uint8*)BackBotton);

	return (CONTACT1);
}

ShowContact_Type contactNumber2(void){

	uint8 counterChar;
	uint8 watchChar;

	LCDNokia_gotoXY(1,0);
	LCDNokia_sendString((uint8*)NameField);
	LCDNokia_gotoXY(1,1);

	for(counterChar = 0; watchChar != '\0'; counterChar++){
		LCDNokia_sendChar(readMemory((POSITION_NAME + NEXT_POSITION) + counterChar));
		watchChar = readMemory((POSITION_NAME + NEXT_POSITION) + counterChar);
	}

	LCDNokia_gotoXY(1,2);
	LCDNokia_sendString((uint8*)NumberField);
	LCDNokia_gotoXY(1,3);

	for(counterChar = 0; watchChar != '\0'; counterChar++){
		LCDNokia_sendChar(readMemory((POSITION_NUMBER + NEXT_POSITION) + counterChar));
		watchChar = readMemory((POSITION_NUMBER + NEXT_POSITION) + counterChar);
	}

	LCDNokia_gotoXY(1,5);
	LCDNokia_sendString((uint8*)SelectBotton);
	LCDNokia_gotoXY(50,5);
	LCDNokia_sendString((uint8*)BackBotton);

	return (CONTACT1);
}

ShowContact_Type contactNumber3(void){

	uint8 counterChar;
	uint8 watchChar;

	LCDNokia_gotoXY(1,0);
	LCDNokia_sendString((uint8*)NameField);
	LCDNokia_gotoXY(1,1);

	for(counterChar = 0; watchChar != '\0'; counterChar++){
		LCDNokia_sendChar(readMemory((POSITION_NAME + (2*NEXT_POSITION)) + counterChar));
		watchChar = readMemory((POSITION_NAME + (2*NEXT_POSITION)) + counterChar);
	}

	LCDNokia_gotoXY(1,2);
	LCDNokia_sendString((uint8*)NumberField);
	LCDNokia_gotoXY(1,3);

	for(counterChar = 0; watchChar != '\0'; counterChar++){
		LCDNokia_sendChar(readMemory((POSITION_NUMBER + (2*NEXT_POSITION)) + counterChar));
		watchChar = readMemory((POSITION_NUMBER + (2*NEXT_POSITION)) + counterChar);
	}

	LCDNokia_gotoXY(1,5);
	LCDNokia_sendString((uint8*)SelectBotton);
	LCDNokia_gotoXY(50,5);
	LCDNokia_sendString((uint8*)BackBotton);

	return (CONTACT1);
}

ShowContact_Type contactNumber4(void){

	uint8 counterChar;
	uint8 watchChar;

	LCDNokia_gotoXY(1,0);
	LCDNokia_sendString((uint8*)NameField);
	LCDNokia_gotoXY(1,1);

	for(counterChar = 0; watchChar != '\0'; counterChar++){
		LCDNokia_sendChar(readMemory((POSITION_NAME + (3*NEXT_POSITION)) + counterChar));
		watchChar = readMemory((POSITION_NAME + (3*NEXT_POSITION)) + counterChar);
	}

	LCDNokia_gotoXY(1,2);
	LCDNokia_sendString((uint8*)NumberField);
	LCDNokia_gotoXY(1,3);

	for(counterChar = 0; watchChar != '\0'; counterChar++){
		LCDNokia_sendChar(readMemory((POSITION_NUMBER + (3*NEXT_POSITION)) + counterChar));
		watchChar = readMemory((POSITION_NUMBER + (3*NEXT_POSITION)) + counterChar);
	}

	LCDNokia_gotoXY(1,5);
	LCDNokia_sendString((uint8*)SelectBotton);
	LCDNokia_gotoXY(50,5);
	LCDNokia_sendString((uint8*)BackBotton);

	return (CONTACT1);
}

ShowContact_Type contactNumber5(void){

	uint8 counterChar;
	uint8 watchChar;

	LCDNokia_gotoXY(1,0);
	LCDNokia_sendString((uint8*)NameField);
	LCDNokia_gotoXY(1,1);

	for(counterChar = 0; watchChar != '\0'; counterChar++){
		LCDNokia_sendChar(readMemory((POSITION_NAME + (4*NEXT_POSITION)) + counterChar));
		watchChar = readMemory((POSITION_NAME + (4*NEXT_POSITION)) + counterChar);
	}

	LCDNokia_gotoXY(1,2);
	LCDNokia_sendString((uint8*)NumberField);
	LCDNokia_gotoXY(1,3);

	for(counterChar = 0; watchChar != '\0'; counterChar++){
		LCDNokia_sendChar(readMemory((POSITION_NUMBER + (4*NEXT_POSITION)) + counterChar));
		watchChar = readMemory((POSITION_NUMBER + (4*NEXT_POSITION)) + counterChar);
	}

	LCDNokia_gotoXY(1,5);
	LCDNokia_sendString((uint8*)SelectBotton);
	LCDNokia_gotoXY(50,5);
	LCDNokia_sendString((uint8*)BackBotton);

	return (CONTACT1);
}

ShowContact_Type contactNumber6(void){

	uint8 counterChar;
	uint8 watchChar;

	LCDNokia_gotoXY(1,0);
	LCDNokia_sendString((uint8*)NameField);
	LCDNokia_gotoXY(1,1);

	for(counterChar = 0; watchChar != '\0'; counterChar++){
		LCDNokia_sendChar(readMemory((POSITION_NAME + (5*NEXT_POSITION)) + counterChar));
		watchChar = readMemory((POSITION_NAME + (5*NEXT_POSITION)) + counterChar);
	}

	LCDNokia_gotoXY(1,2);
	LCDNokia_sendString((uint8*)NumberField);
	LCDNokia_gotoXY(1,3);

	for(counterChar = 0; watchChar != '\0'; counterChar++){
		LCDNokia_sendChar(readMemory((POSITION_NUMBER + (5*NEXT_POSITION)) + counterChar));
		watchChar = readMemory((POSITION_NUMBER + (5*NEXT_POSITION)) + counterChar);
	}

	LCDNokia_gotoXY(1,5);
	LCDNokia_sendString((uint8*)SelectBotton);
	LCDNokia_gotoXY(50,5);
	LCDNokia_sendString((uint8*)BackBotton);

	return (CONTACT1);
}

ShowContact_Type contactNumber7(void){

	uint8 counterChar;
	uint8 watchChar;

	LCDNokia_gotoXY(1,0);
	LCDNokia_sendString((uint8*)NameField);
	LCDNokia_gotoXY(1,1);

	for(counterChar = 0; watchChar != '\0'; counterChar++){
		LCDNokia_sendChar(readMemory((POSITION_NAME + (6*NEXT_POSITION)) + counterChar));
		watchChar = readMemory((POSITION_NAME + (6*NEXT_POSITION)) + counterChar);
	}

	LCDNokia_gotoXY(1,2);
	LCDNokia_sendString((uint8*)NumberField);
	LCDNokia_gotoXY(1,3);

	for(counterChar = 0; watchChar != '\0'; counterChar++){
		LCDNokia_sendChar(readMemory((POSITION_NUMBER + (6*NEXT_POSITION)) + counterChar));
		watchChar = readMemory((POSITION_NUMBER + (6*NEXT_POSITION)) + counterChar);
	}

	LCDNokia_gotoXY(1,5);
	LCDNokia_sendString((uint8*)SelectBotton);
	LCDNokia_gotoXY(50,5);
	LCDNokia_sendString((uint8*)BackBotton);

	return (CONTACT1);
}

ShowContact_Type contactNumber8(void){

	uint8 counterChar;
	uint8 watchChar;

	LCDNokia_gotoXY(1,0);
	LCDNokia_sendString((uint8*)NameField);
	LCDNokia_gotoXY(1,1);

	for(counterChar = 0; watchChar != '\0'; counterChar++){
		LCDNokia_sendChar(readMemory((POSITION_NAME + (7*NEXT_POSITION)) + counterChar));
		watchChar = readMemory((POSITION_NAME + (7*NEXT_POSITION)) + counterChar);
	}

	LCDNokia_gotoXY(1,2);
	LCDNokia_sendString((uint8*)NumberField);
	LCDNokia_gotoXY(1,3);

	for(counterChar = 0; watchChar != '\0'; counterChar++){
		LCDNokia_sendChar(readMemory((POSITION_NUMBER + (7*NEXT_POSITION)) + counterChar));
		watchChar = readMemory((POSITION_NUMBER + (7*NEXT_POSITION)) + counterChar);
	}

	LCDNokia_gotoXY(1,5);
	LCDNokia_sendString((uint8*)SelectBotton);
	LCDNokia_gotoXY(50,5);
	LCDNokia_sendString((uint8*)BackBotton);

	return (CONTACT1);
}

ShowContact_Type contactNumber9(void){

	uint8 counterChar;
	uint8 watchChar;

	LCDNokia_gotoXY(1,0);
	LCDNokia_sendString((uint8*)NameField);
	LCDNokia_gotoXY(1,1);

	for(counterChar = 0; watchChar != '\0'; counterChar++){
		LCDNokia_sendChar(readMemory((POSITION_NAME + (8*NEXT_POSITION)) + counterChar));
		watchChar = readMemory((POSITION_NAME + (8*NEXT_POSITION)) + counterChar);
	}

	LCDNokia_gotoXY(1,2);
	LCDNokia_sendString((uint8*)NumberField);
	LCDNokia_gotoXY(1,3);

	for(counterChar = 0; watchChar != '\0'; counterChar++){
		LCDNokia_sendChar(readMemory((POSITION_NUMBER + (8*NEXT_POSITION)) + counterChar));
		watchChar = readMemory((POSITION_NUMBER + (8*NEXT_POSITION)) + counterChar);
	}

	LCDNokia_gotoXY(1,5);
	LCDNokia_sendString((uint8*)SelectBotton);
	LCDNokia_gotoXY(50,5);
	LCDNokia_sendString((uint8*)BackBotton);

	return (CONTACT1);
}

ShowContact_Type contactNumber10(void){

	uint8 counterChar;
	uint8 watchChar;

	LCDNokia_gotoXY(1,0);
	LCDNokia_sendString((uint8*)NameField);
	LCDNokia_gotoXY(1,1);

	for(counterChar = 0; counterChar != 50; counterChar++){
		LCDNokia_sendChar(readMemory((POSITION_NAME + (9*NEXT_POSITION)) + counterChar));
		watchChar = readMemory((POSITION_NAME + (9*NEXT_POSITION)) + counterChar);
		if(watchChar == '\0'){counterChar = 50;}
	}

	LCDNokia_gotoXY(1,2);
	LCDNokia_sendString((uint8*)NumberField);
	LCDNokia_gotoXY(1,3);

	for(counterChar = 0; watchChar != '\0'; counterChar++){
		LCDNokia_sendChar(readMemory((POSITION_NUMBER + (9*NEXT_POSITION)) + counterChar));
		watchChar = readMemory((POSITION_NUMBER + (9*NEXT_POSITION)) + counterChar);
	}

	LCDNokia_gotoXY(1,5);
	LCDNokia_sendString((uint8*)SelectBotton);
	LCDNokia_gotoXY(50,5);
	LCDNokia_sendString((uint8*)BackBotton);

	return (CONTACT1);
}

void cleanContact(uint8 contact){
	uint8 counter;
	uint32 positionName;
	uint32 positionNumber;

	positionName = (contact * NEXT_POSITION) - NEXT_POSITION;
	for(counter = 0; counter < 10; counter++){
		writeMemory(positionName, REGISTER_EMPTY);
		E2PROMdelay(65000);
	}

	positionNumber = positionName + POSITION_NUMBER;
	for(counter = 0; counter < 12; counter++){
		writeMemory(positionNumber, REGISTER_EMPTY);
		E2PROMdelay(65000);
	}
}

PhaseMainMenu_Type initialLoad1(PhaseMainMenu_Type data){

	/**Create the variable with current data**/
	static PhaseMainMenu_Type currentMainMenu1;
	magCalibration();

	LCDNokia_clear();
	/**Set with the current state and phase**/
	currentMainMenu1.phaseState = INITIAL_LOAD2;
	currentMainMenu1.stateMain = data.stateMain;

	return (currentMainMenu1);
}

PhaseMainMenu_Type initialLoad2(PhaseMainMenu_Type data){

	/**Create the variable with current data**/
	static PhaseMainMenu_Type currentMainMenu1;

	//NoContacts = Convert_wordASCIItoDATA(readMemory(POSITION_CONTACTS));

	/**Set with the current state and phase**/
	currentMainMenu1.phaseState = GENERAL_VIEW;
	currentMainMenu1.stateMain = data.stateMain;

	return (currentMainMenu1);
}

PhaseMainMenu_Type generalView(PhaseMainMenu_Type data){

	/**Create the variable with current data**/
	static PhaseMainMenu_Type currentMainMenu2;

	/**Set with the current state and phase**/
	currentMainMenu2.phaseState = GENERAL_VIEW;
	currentMainMenu2.stateMain = MAIN_MENU;

	if(TRUE == FlagChange_Wallpaper){
		LCDNokia_clear();
		Wall_Type(*wallpaper)(void);
		wallpaper = StateWallpaper[CurrentWallpaper].stateWall;
		wallpaper();
		FlagChange_Wallpaper = FALSE;
	}
	/**Detect when a key is pressed*/
	if(getUART0_flag()){
		/**Comparison between the pressed keys to continue with next main state**/
		if(getUART0_mailBox() == ASCII_M){
			currentMainMenu2.phaseState = VIEW_MENU;
		}
		if(getUART0_mailBox() == ASCII_m){
			currentMainMenu2.phaseState = VIEW_MENU;
		}

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	/**Clear the mailbox**/
	clearUART0_mailbox();

	return (currentMainMenu2);
}

PhaseMainMenu_Type viewMenu(PhaseMainMenu_Type data){

	/**Create the variable with current data**/
	static PhaseMainMenu_Type currentMainMenu2;
	static uint8 flagChange_Menu = TRUE;
	static WallMenu_Type currentMenu = MESSAGES_MAIN;
	static sint8 counterMenu = 1;

	/**Set with the current state and phase**/
	currentMainMenu2.phaseState = data.phaseState;
	currentMainMenu2.stateMain = data.stateMain;

	if(TRUE == flagChange_Menu){
		WallMenu_Type(*menus)(void);
		menus = StateMenus[currentMenu].stateWallMenu;
		currentMenu = menus();
		flagChange_Menu = FALSE;
	}
	/**Detect when a key is pressed*/
	if(getUART0_flag()){
		/**Comparison between the pressed keys to continue with next main state**/
		if((getUART0_mailBox() == ASCII_D) || (getUART0_mailBox() == ASCII_d)){
			flagChange_Menu = TRUE;
			counterMenu++;
			if(counterMenu > 5){counterMenu = 1;}
		}
		if((getUART0_mailBox() == ASCII_A) || (getUART0_mailBox() == ASCII_a)){
			if(counterMenu == 0){counterMenu = 5;}
			else{counterMenu--;}
			flagChange_Menu = TRUE;
		}
		if((getUART0_mailBox() == ASCII_B) || (getUART0_mailBox() == ASCII_b)){
			flagChange_Menu = TRUE;
			FlagChange_Wallpaper = TRUE;
			currentMainMenu2.phaseState = GENERAL_VIEW;
		}
		if((getUART0_mailBox() == ASCII_S) || (getUART0_mailBox() == ASCII_s)){
			if(currentMenu == MESSAGES_MAIN){currentMainMenu2.stateMain = MESSAGES;}
			if(currentMenu == CONTACTS_MAIN){currentMainMenu2.stateMain = CONTACTS;}
			if(currentMenu == SNAKE_MAIN){
				currentMainMenu2.stateMain = SNAKE_GAME;
				FlagSnakeInit = TRUE;
			}
			if(currentMenu == COMPASS_MAIN){currentMainMenu2.stateMain = COMPASS;}
			if(currentMenu == WALLPAPER_MAIN){currentMainMenu2.stateMain = WALLPAPER;}

			flagChange_Menu = TRUE;
			counterMenu = 1;
		}
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	/**Clear the mailbox**/
	clearUART0_mailbox();

	if(counterMenu == 1){currentMenu = MESSAGES_MAIN;}
	if(counterMenu == 2){currentMenu = CONTACTS_MAIN;}
	if(counterMenu == 3){currentMenu = SNAKE_MAIN;}
	if(counterMenu == 4){currentMenu = COMPASS_MAIN;}
	if(counterMenu == 5){currentMenu = WALLPAPER_MAIN;}

	return (currentMainMenu2);
}
PhaseMessages_Type messagesMenu(PhaseMessages_Type data){

	static PhaseMessages_Type currentMessages1;
	static sint8 counterMenu = 1;
	static uint8 flagChange_Menu = TRUE;

	currentMessages1.phaseState = MESSAGES_MENU;
	currentMessages1.stateMain = MESSAGES;

	if(TRUE == flagChange_Menu){
		if(counterMenu == 1){printDiedGame();}
		if(counterMenu == 2){printTestFrame();}
		flagChange_Menu = FALSE;
	}
	/**Detect when a key is pressed*/
	if(getUART0_flag()){
		/**Comparison between the pressed keys to continue with next main state**/
		if((getUART0_mailBox() == ASCII_D) || (getUART0_mailBox() == ASCII_d)){
			flagChange_Menu = TRUE;
			counterMenu++;
			if(counterMenu > 2){counterMenu = 1;}
		}
		if((getUART0_mailBox() == ASCII_A) || (getUART0_mailBox() == ASCII_a)){
			if(counterMenu == 0){counterMenu = 1;}
			else{counterMenu--;}
			flagChange_Menu = TRUE;
		}
		if((getUART0_mailBox() == ASCII_B) || (getUART0_mailBox() == ASCII_b)){
			flagChange_Menu = TRUE;
			currentMessages1.stateMain = MAIN_MENU;
		}
		if((getUART0_mailBox() == ASCII_S) || (getUART0_mailBox() == ASCII_s)){
			if(counterMenu == 1){currentMessages1.phaseState = WRITE_NAME;}
			if(counterMenu == 2){currentMessages1.phaseState = MAILBOX_MSG;}
			LCDNokia_clear();
			flagChange_Menu = TRUE;
			counterMenu = 1;
		}
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	/**Clear the mailbox**/
	clearUART0_mailbox();

	return (currentMessages1);
}
PhaseMessages_Type mailBoxMSG(PhaseMessages_Type data){

	static PhaseMessages_Type currentMessages2;

	return (currentMessages2);
}

PhaseMessages_Type writeName(PhaseMessages_Type data){

	static PhaseMessages_Type currentMessages3;
	static uint8 counter;
	static uint8 counterChar = 0;
	static uint8 flagLock = TRUE;

	currentMessages3.phaseState = WRITE_NAME;
	currentMessages3.stateMain = MESSAGES;

	if(TRUE == flagLock){
		LCDNokia_gotoXY(1,0);
		LCDNokia_sendString((uint8*)NameField);
		LCDNokia_gotoXY(1,1);
		flagLock = FALSE;
	}

	if(getUART0_flag()){
		for(counter = 0; counter < 10; counter++){
			if(ASCII_Numbers[counter] == getUART0_mailBox()){
				pushFIFO_0(getUART0_mailBox());
				LCDNokia_sendChar(getUART0_mailBox());
			}
		}
		if(getUART0_mailBox() == ASCII_CR){
			Name = popFIFO_0();
			currentMessages3.phaseState = WRITE_MESSAGES;
			counterChar = 0;
			clearFIFO_0();
			flagLock = TRUE;
		}
		if(getUART0_mailBox() == ASCII_ESC){
			clearFIFO(Name);
			clearFIFO_0();
			currentMessages3.stateMain = MAIN_MENU;
			counterChar = 0;
			flagLock = TRUE;
		}
		counterChar++;
	/**clear the reception flag*/
	setUART0_flag(FALSE);
	}
	/**Clear the mailbox**/
	clearUART0_mailbox();

	return(currentMessages3);
}
PhaseMessages_Type writeMessages(PhaseMessages_Type data){

	static PhaseMessages_Type currentMessages4;
	static uint8 counter;
	static uint8 counterChar = 0;
	static uint8 flagLock = TRUE;

	currentMessages4.phaseState = WRITE_MESSAGES;
	currentMessages4.stateMain = MESSAGES;

	if(TRUE == flagLock){
		LCDNokia_gotoXY(1,2);
		LCDNokia_sendString((uint8*)MessageField);
		LCDNokia_gotoXY(1,3);
		flagLock = FALSE;
	}

	if(getUART0_flag()){
		for(counter = 0; counter < 27; counter++){
			if(ASCII_Alphabet[counter] == getUART0_mailBox()){
				pushFIFO_0(getUART0_mailBox());
				LCDNokia_sendChar(getUART0_mailBox());
			}
		}
		if(getUART0_mailBox() == ASCII_CR){
			Message = popFIFO_0();
			currentMessages4.phaseState = SEND_MESSAGES;
			counterChar = 0;
			clearFIFO_0();
			flagLock = TRUE;
		}
		if(getUART0_mailBox() == ASCII_ESC){
			clearFIFO(Name);
			clearFIFO(Message);
			clearFIFO_0();
			currentMessages4.stateMain = MAIN_MENU;
			counterChar = 0;
			flagLock = TRUE;
		}
		counterChar++;

	/**clear the reception flag*/
	setUART0_flag(FALSE);
	}
	/**Clear the mailbox**/
	clearUART0_mailbox();

	return(currentMessages4);
}
PhaseMessages_Type sendMessages(PhaseMessages_Type data){

	static PhaseMessages_Type currentMessages5;

	currentMessages5.phaseState = SEND_MESSAGES;
	currentMessages5.stateMain = MESSAGES;


	//UART_putString(UART_1,&SendMsg);


	return(currentMessages5);
}
PhaseMessages_Type exitMessages(PhaseMessages_Type data){

	static PhaseMessages_Type currentMessages6;

	currentMessages6.stateMain = MAIN_MENU;
	clearFIFO(Name);
	clearFIFO(Message);

	return(currentMessages6);
}

PhaseContacts_Type contactsMenu(PhaseContacts_Type data){

	static PhaseContacts_Type currentContacts1;
	static sint8 counterMenu = 1;
	static uint8 flagChange_Menu = TRUE;

	currentContacts1.phaseState = CONTACT_MENU;
	currentContacts1.stateMain = CONTACTS;

	if(TRUE == flagChange_Menu){
		if(counterMenu == 1){printDiedGame();}
		if(counterMenu == 2){printTestFrame();}
		flagChange_Menu = FALSE;
	}
	/**Detect when a key is pressed*/
	if(getUART0_flag()){
		/**Comparison between the pressed keys to continue with next main state**/
		if((getUART0_mailBox() == ASCII_D) || (getUART0_mailBox() == ASCII_d)){
			flagChange_Menu = TRUE;
			counterMenu++;
			if(counterMenu > 2){counterMenu = 1;}
		}
		if((getUART0_mailBox() == ASCII_A) || (getUART0_mailBox() == ASCII_a)){
			if(counterMenu == 0){counterMenu = 1;}
			else{counterMenu--;}
			flagChange_Menu = TRUE;
		}
		if((getUART0_mailBox() == ASCII_B) || (getUART0_mailBox() == ASCII_b)){
			flagChange_Menu = TRUE;
			currentContacts1.stateMain = MAIN_MENU;
		}
		if((getUART0_mailBox() == ASCII_S) || (getUART0_mailBox() == ASCII_s)){
			if(counterMenu == 1){currentContacts1.phaseState = VIEW_CONTACTS;}
			if(counterMenu == 2){currentContacts1.phaseState = ADD_CONTACTS;}
			LCDNokia_clear();
			flagChange_Menu = TRUE;
			counterMenu = 1;
		}
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
	static uint8 flagChange_Contact = TRUE;
	static ShowContact_Type currentFriend = CONTACT1;
	static sint8 counterMenu = 1;

	/**Set with the current state and phase**/
	currentContacts2.phaseState = data.phaseState;
	currentContacts2.stateMain = data.stateMain;

	if(NoContacts == 0){
		LCDNokia_gotoXY(20,2);
		LCDNokia_sendString((uint8*)EmptyContacts);
		LCDNokia_gotoXY(50,5);
		LCDNokia_sendString((uint8*)BackBotton);
	}
	if(NoContacts > 0){
		if(TRUE == flagChange_Contact){
			ShowContact_Type(*showCont)(void);
			showCont = StateShowContacts[currentFriend].stateShowContact;
			currentFriend = showCont();
			flagChange_Contact = FALSE;
		}
	}
	if(getUART0_flag()){
		if(NoContacts != 0){
			if((getUART0_mailBox() == ASCII_D) || (getUART0_mailBox() == ASCII_d)){
				flagChange_Contact = TRUE;
				counterMenu++;
				if(counterMenu > NoContacts){counterMenu = 1;}
			}
			if((getUART0_mailBox() == ASCII_A) || (getUART0_mailBox() == ASCII_a)){
				if(counterMenu == 0){counterMenu = 1;}
				else{counterMenu--;}
				flagChange_Contact = TRUE;
			}
			if((getUART0_mailBox() == ASCII_B) || (getUART0_mailBox() == ASCII_b)){
				flagChange_Contact = TRUE;
				counterMenu = 1;
				currentContacts2.phaseState = CONTACT_MENU;
			}
			if((getUART0_mailBox() == ASCII_E) || (getUART0_mailBox() == ASCII_e)){
				if(counterMenu == 1){currentContacts2.noContact = 1;}
				if(counterMenu == 2){currentContacts2.noContact = 2;}
				if(counterMenu == 3){currentContacts2.noContact = 3;}
				if(counterMenu == 4){currentContacts2.noContact = 4;}
				if(counterMenu == 5){currentContacts2.noContact = 5;}
				if(counterMenu == 6){currentContacts2.noContact = 6;}
				if(counterMenu == 7){currentContacts2.noContact = 7;}
				if(counterMenu == 8){currentContacts2.noContact = 8;}
				if(counterMenu == 9){currentContacts2.noContact = 9;}
				if(counterMenu == 10){currentContacts2.noContact = 10;}
				currentContacts2.phaseState = EDIT_CONTACTS;
				LCDNokia_clear();
				flagChange_Contact = TRUE;
				counterMenu = 1;
			}
		}
		if(getUART0_mailBox() == ASCII_B){
			flagChange_Contact = TRUE;
			LCDNokia_clear();
			counterMenu = 1;
			currentContacts2.phaseState = CONTACT_MENU;
		}
		if(getUART0_mailBox() == ASCII_b){
			flagChange_Contact = TRUE;
			LCDNokia_clear();
			counterMenu = 1;
			currentContacts2.phaseState = CONTACT_MENU;
		}
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
	static uint8 counter;
	static uint8 counterSize_Name = 0;
	static uint8 counterSize_Number = 0;
	static uint8 flagLock = TRUE;
	static uint8 flagLock2 = TRUE;

	/**Set with the current state and phase**/
	currentContacts3.phaseState = data.phaseState;
	currentContacts3.stateMain = data.stateMain;
	currentContacts3.noContact = data.noContact;

	if(TRUE == flagLock){
		if(flagField == 0){
			LCDNokia_gotoXY(1,0);
			LCDNokia_sendString((uint8*)NameField);
			LCDNokia_gotoXY(1,1);
		}
		if(flagField == 1){
			LCDNokia_gotoXY(1,2);
			LCDNokia_sendString((uint8*)NumberField);
			LCDNokia_gotoXY(1,3);
		}
		flagLock = FALSE;
	}

	/**Verifies if the registers are full**/
	if(NoContacts != FULL_CONTACTS){
		/**Capture the Name field*/
		if((getUART0_flag()) && (flagField == 0)){

			for(counter = 0; counter < 27; counter++){
				if(ASCII_Alphabet[counter] == getUART0_mailBox()){
					currentContacts3.name[counterSize_Name] = getUART0_mailBox();
					LCDNokia_sendChar(getUART0_mailBox());
					counterSize_Name++;
				}
			}
			if(getUART0_mailBox() == ASCII_CR){
				flagField = 1;
				currentContacts3.sizeName = counterSize_Name;
				counterSize_Name = 0;
				flagLock = TRUE;
			}
			/**clear the reception flag*/
			setUART0_flag(FALSE);
		}

		/**Capture the Number field*/
		if((getUART0_flag()) && (flagField == 1)){
			for(counter = 0; counter < 10; counter++){
				if(ASCII_Numbers[counter] == getUART0_mailBox()){
					LCDNokia_sendChar(getUART0_mailBox());
					currentContacts3.number[counterSize_Number] = getUART0_mailBox();
					counterSize_Number++;
				}
			}

			if(getUART0_mailBox() == ASCII_CR){
				flagField = 0;
				currentContacts3.sizeNumber = counterSize_Number;
				currentContacts3.phaseState = SAVE_CONTACTS;
				counterSize_Number = 0;
				flagLock = TRUE;
			}
			/**clear the reception flag*/
			setUART0_flag(FALSE);
		}
	}else{

		if(TRUE == flagLock2){
			if(flagField == 1){
				LCDNokia_gotoXY(45,2);
				LCDNokia_sendString((uint8*)FullContacts);
			}
			flagLock2 = FALSE;
		}
		if(getUART0_flag()){
			if(getUART0_mailBox() == ASCII_ESC){
				currentContacts3.phaseState = CONTACT_MENU;
				flagLock2 = TRUE;
			}
			/**clear the reception flag*/
			setUART0_flag(FALSE);
		}
	}
	/**Clear the mailbox**/
	clearUART0_mailbox();
	return (currentContacts3);
}

PhaseContacts_Type editContacts(PhaseContacts_Type data){

	/**Create the variable with current data**/
	static PhaseContacts_Type currentContacts4;

	/**Set with the current state and phase**/
	currentContacts4.phaseState = data.phaseState;
	currentContacts4.stateMain = data.stateMain;

	cleanContact(data.noContact);

	ModeContact = MODE_EDIT;
	currentContacts4.phaseState = ADD_CONTACTS;

	return (currentContacts4);
}
PhaseContacts_Type saveContacts(PhaseContacts_Type data){

	/**Create the variable with current data**/
	static PhaseContacts_Type currentContacts5;
	uint32 positionAddress_Name;
	uint32 positionAddress_Number;
	uint8 counterSave;
	uint32 counterAddress = 0;

	if(ModeContact == MODE_EDIT){ positionAddress_Name = (data.noContact*NEXT_POSITION);}
	if(ModeContact == MODE_ADD){ positionAddress_Name = (NoContacts*NEXT_POSITION) + NEXT_POSITION;}
	positionAddress_Number = positionAddress_Name + POSITION_NUMBER;

	if(ModeContact == MODE_ADD){
		NoContacts++;
		CurrentAddress_Name = NoContacts*NEXT_POSITION;
		CurrentAddress_Number = CurrentAddress_Name + POSITION_NUMBER;
	}

	/**Save the name into memory*/
	for(counterSave = data.sizeName; counterSave != 0; counterSave--){
		positionAddress_Name += counterAddress;
		writeMemory(positionAddress_Name,data.name[counterAddress]);
		counterAddress++;
	}
	counterAddress = 0;

	/**Save the number into memory*/
	for(counterSave = data.sizeNumber; counterSave != 0; counterSave--){
		positionAddress_Number += counterAddress;
		writeMemory(positionAddress_Number,data.number[counterAddress]);
		counterAddress++;
	}

	ModeContact = MODE_ADD;
	writeMemory(POSITION_CONTACTS,(REGISTER_EMPTY + NoContacts));
	currentContacts5.phaseState = CONTACT_MENU;
	currentContacts5.stateMain = CONTACTS;
	LCDNokia_clear();

	return (currentContacts5);
}

PhaseSnake_Type startGame(PhaseSnake_Type data){

	/**Create the variable with current data**/
	static PhaseSnake_Type currentSnake1;
	static uint8 flagSnakeInit = TRUE;

	currentSnake1.phaseState = START_GAME;
	currentSnake1.stateMain = SNAKE_GAME;

	/**Show the menu of the game**/
	if(TRUE == flagSnakeInit){
		printSnakeGameFrame();
		flagSnakeInit = FALSE;
	}

	/**Press any key to continue**/
	if(getUART0_flag()){
		if(getUART0_mailBox() == ASCII_SPACE){
			currentSnake1.phaseState = RUN_GAME;
		}
		if(getUART0_mailBox() == ASCII_ESC){
			currentSnake1.stateMain = MAIN_MENU;
			FlagChange_Wallpaper = TRUE;
		}
		flagSnakeInit = TRUE;
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	/**Clear the mailbox**/
	clearUART0_mailbox();

	return (currentSnake1);
}
PhaseSnake_Type runGame(PhaseSnake_Type data){

	/**Create the variable with current data**/
	static PhaseSnake_Type currentSnake2;
	static SnakeInfo_Type infoGame;

	currentSnake2.phaseState = RUN_GAME;
	currentSnake2.stateMain = SNAKE_GAME;

	infoGame = runSnake();

	if(infoGame.lives == 0){
		currentSnake2.lives = infoGame.lives;
		currentSnake2.score = infoGame.score;
		currentSnake2.phaseState = GAME_LOST;
	}
	return (currentSnake2);
}
PhaseSnake_Type gameLost(PhaseSnake_Type data){

	static PhaseSnake_Type currentSnake3;
	static uint8 flagGameOver = TRUE;

	currentSnake3.phaseState = GAME_LOST;
	currentSnake3.stateMain = SNAKE_GAME;
	currentSnake3.lives = data.lives;
	currentSnake3.score = data.score;


	if(TRUE == flagGameOver){
		clearScore();
		LCDNokia_clear();
		printGameOVer();
		flagGameOver = FALSE;
	}
	if(getUART0_flag()){
		currentSnake3.phaseState = SHOW_SCORE;
		flagGameOver = TRUE;
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	/**Clear the mailbox**/
	clearUART0_mailbox();

	return (currentSnake3);
}
PhaseSnake_Type showScore(PhaseSnake_Type data){

	static PhaseSnake_Type currentSnake4;
	static uint8 flagClear = TRUE;

	currentSnake4.phaseState = SHOW_SCORE;
	currentSnake4.stateMain = SNAKE_GAME;
	currentSnake4.lives = data.lives;
	currentSnake4.score = data.score;

	if(TRUE == flagClear){
		LCDNokia_clear();
		LCDNokia_gotoXY(7,3);
		LCDNokia_sendString((uint8*)MsgScore);
		LCDNokia_gotoXY(32,4);
		LCDNokia_printValue(currentSnake4.score);
		flagClear = FALSE;
	}
	if(getUART0_flag()){
		currentSnake4.phaseState = PLAY_AGAIN;
		flagClear = TRUE;
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	/**Clear the mailbox**/
	clearUART0_mailbox();

	return (currentSnake4);

}
PhaseSnake_Type playAgain(PhaseSnake_Type data){

	static PhaseSnake_Type currentSnake5;
	static uint8 flagClear = TRUE;

	currentSnake5.phaseState = PLAY_AGAIN;
	currentSnake5.stateMain = SNAKE_GAME;

	if(TRUE == flagClear){
		LCDNokia_clear();
		LCDNokia_gotoXY(7,3);
		LCDNokia_sendString((uint8*)MsgPlay);
		LCDNokia_gotoXY(15,4);
		LCDNokia_sendString((uint8*)MsgConfirm);
		flagClear = FALSE;
	}
	if(getUART0_flag()){
		if((getUART0_mailBox() == ASCII_Y) || (getUART0_mailBox() == ASCII_y)){
			currentSnake5.phaseState = RUN_GAME;
		}
		if((getUART0_mailBox() == ASCII_N) || (getUART0_mailBox() == ASCII_n)){
			currentSnake5.phaseState = EXIT_GAME;
		}
		flagClear = TRUE;
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	/**Clear the mailbox**/
	clearUART0_mailbox();

	return (currentSnake5);
}
PhaseSnake_Type exitGame(PhaseSnake_Type data){

	/**Create the variable with current data**/
	static PhaseSnake_Type currentSnake6;

	LCDNokia_clear();
	currentSnake6.stateMain = MAIN_MENU;

	return (currentSnake6);
}

PhaseCompass_Type showCompass(PhaseCompass_Type data){

	/**Create the variable with current data**/
	static PhaseCompass_Type currentCompass1;
	static uint8 flagContrast = TRUE;

	currentCompass1.phaseState = SHOW_COMPASS;
	currentCompass1.stateMain = COMPASS;

	if(TRUE == flagContrast){
		writeI2CDevice(0x3A,0x2A,0x35);
		flagContrast = FALSE;
	}
	startCompass();

	if(getUART0_flag()){
		if(getUART0_mailBox() == ASCII_ESC){
			writeI2CDevice(0x3A,0x2A,0x00);
			currentCompass1.phaseState = EXIT_COMPASS;
		}
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	/**Clear the mailbox**/
	clearUART0_mailbox();

	return(currentCompass1);
}
PhaseCompass_Type exitCompass(PhaseCompass_Type data){

	/**Create the variable with current data**/
	static PhaseCompass_Type currentCompass2;
	LCDNokia_writeByte(LCD_CMD, 0xBB);
	LCDNokia_clear();
	currentCompass2.stateMain = MAIN_MENU;

	return(currentCompass2);
}

PhaseWallpaper_Type viewWallpaper(PhaseWallpaper_Type data){

	static PhaseWallpaper_Type currentWallpaper1;
	static sint8 counterMenu = 1;
	static uint8 flagChange_Wall = TRUE;
	static Images_Type currentImage = CHIP;

	currentWallpaper1.phaseState = VIEW_WALLPAPER;
	currentWallpaper1.stateMain = WALLPAPER;

	if(TRUE == flagChange_Wall){
		Images_Type(*images)(void);
		images = StateImages[currentImage].stateImages;
		currentImage = images();
		flagChange_Wall= FALSE;
	}

	if(getUART0_flag()){
		if((getUART0_mailBox() == ASCII_D) || (getUART0_mailBox() == ASCII_d)){
			flagChange_Wall = TRUE;
			counterMenu++;
			if(counterMenu > 5){counterMenu = 1;}
		}
		if((getUART0_mailBox() == ASCII_A) || (getUART0_mailBox() == ASCII_a)){
			if(counterMenu == 0){counterMenu = 5;}
			else{counterMenu--;}
			flagChange_Wall = TRUE;
		}
		if((getUART0_mailBox() == ASCII_B) || (getUART0_mailBox() == ASCII_b)){
			flagChange_Wall = TRUE;
			currentWallpaper1.phaseState = EXIT_WALLPAPER;
			counterMenu = 1;
		}
		if((getUART0_mailBox() == ASCII_S) || (getUART0_mailBox() == ASCII_s)){
			counterMenu--;
			CurrentWallpaper = (Wall_Type)counterMenu;
			flagChange_Wall = TRUE;
			currentWallpaper1.phaseState = EXIT_WALLPAPER;
			counterMenu = 1;
		}
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	clearUART0_mailbox();

	if(counterMenu == 1){currentImage = CHIP;}
	if(counterMenu == 2){currentImage = HORDA;}
	if(counterMenu == 3){currentImage = MUSIC;}
	if(counterMenu == 4){currentImage = PIKACHU;}
	if(counterMenu == 5){currentImage = ANDROID;}

	return(currentWallpaper1);
}
PhaseWallpaper_Type exitWallpaper(PhaseWallpaper_Type data){

	static PhaseWallpaper_Type currentWallpaper2;

	currentWallpaper2.stateMain = MAIN_MENU;
	FlagChange_Wallpaper = TRUE;

	return(currentWallpaper2);
}
