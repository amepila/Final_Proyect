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

typedef enum{ASCII_CR = 13, ASCII_ESC = 27}ASCII_Special;
const uint8 MsgScore[] = "Your score is: ";
const uint8 MsgPlay[] = "Play again?";
const uint8 MsgConfirm[] = "Yes / No";
const uint8 NameField[] = "Name: ";
const uint8 NumberField[] = "Number: ";
const uint8 SaveNewContact[] = "Save";
const uint8 CancelEdit[] = "Cancel";
static uint32 CurrentAddress_Name;
static uint32 CurrentAddress_Number;
static uint32 NoContacts;
static ModeContact_Type ModeContact = MODE_ADD;
static Wall_Type CurrentWallpaper = ANDROID_MENU;
static uint8 FlagChange_Wallpaper = TRUE;
static uint8 FlagSnakeInit = TRUE;

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

	//printLoading();
	magCalibration();

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

PhaseMessages_Type writeName(PhaseMessages_Type data){

	static PhaseMessages_Type currentMessages1;
	static uint8 counterChar = 0;
	static uint8 flagLock = TRUE;

	currentMessages1.phaseState = WRITE_NAME;
	currentMessages1.stateMain = MESSAGES;

	if(TRUE == flagLock){
		LCDNokia_gotoXY(1,1);
		LCDNokia_sendString((uint8*)NameField);
		LCDNokia_gotoXY(2,1);
		flagLock = FALSE;
	}

	if(getUART0_flag()){
		if(counterChar < 15){
			if(getUART0_mailBox() == ASCII_CR){
				currentMessages1.phaseState = WRITE_MESSAGES;
				counterChar = 0;
				flagLock = TRUE;
			}
			if(getUART0_mailBox() != ASCII_CR){
				pushFIFO_1(getUART0_mailBox());
				LCDNokia_sendChar(getUART0_mailBox());
			}
			if(getUART0_mailBox() == ASCII_ESC){
				currentMessages1.stateMain = MAIN_MENU;
				counterChar = 0;
				flagLock = TRUE;
			}
			counterChar++;
		}
		if(counterChar >= 15){
			currentMessages1.phaseState = WRITE_MESSAGES;
			counterChar = 0;
			flagLock = TRUE;
		}
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	/**Clear the mailbox**/
	clearUART0_mailbox();

	return(currentMessages1);
}
PhaseMessages_Type writeMessages(PhaseMessages_Type data){

	static PhaseMessages_Type currentMessages2;
	static uint8 counterChar = 0;

	currentMessages2.phaseState = WRITE_MESSAGES;
	currentMessages2.stateMain = MESSAGES;

	if(getUART0_flag()){
		pushFIFO_1(getUART0_mailBox());
		counterChar++;
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	/**Clear the mailbox**/
	clearUART0_mailbox();

	return(currentMessages2);
}
PhaseMessages_Type sendMessages(PhaseMessages_Type data){

	static PhaseMessages_Type currentMessages3;

	currentMessages3.phaseState = SEND_MESSAGES;
	currentMessages3.stateMain = MESSAGES;

	return(currentMessages3);
}
PhaseMessages_Type exitMessages(PhaseMessages_Type data){

	static PhaseMessages_Type currentMessages4;


	return(currentMessages4);
}

PhaseContacts_Type contactsMenu(PhaseContacts_Type data){

	/**Create the variable with current data**/
	static PhaseContacts_Type currentContacts1;

	/**Set with the current state and phase**/
	currentContacts1.phaseState = data.phaseState;
	currentContacts1.stateMain = data.stateMain;

	//Frame of ContactMenu

	/**Detect when a key is pressed*/
	if(getUART0_flag()){
		/**Comparison between the pressed keys to continue with next main state**/
		if(getUART0_mailBox() == ASCII_1){currentContacts1.phaseState = VIEW_CONTACTS;}
		if(getUART0_mailBox() == ASCII_2){currentContacts1.phaseState = ADD_CONTACTS;}
		if(getUART0_mailBox() == ASCII_ESC){currentContacts1.stateMain = MAIN_MENU;}

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

	//Frame of edit or back

	if(getUART0_flag()){
		if(getUART0_mailBox() == ASCII_E){currentContacts2.phaseState = EDIT_CONTACTS;}
		if(getUART0_mailBox() == ASCII_e){currentContacts2.phaseState = EDIT_CONTACTS;}
		if(getUART0_mailBox() == ASCII_ESC){currentContacts2.phaseState = CONTACT_MENU;}

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
	currentContacts3.noContact = NoContacts;

	//Enter name and number

	/**Verifies if the registers are full**/
	if(NoContacts != FULL_CONTACTS){
		/**Capture the Name field*/
		if((getUART0_flag()) && (flagField == 0)){

			currentContacts3.name[counterSize_Name] = getUART0_mailBox();
			counterSize_Name++;
			if(getUART0_mailBox() == ASCII_CR){
				flagField = 1;
				currentContacts3.sizeName = counterSize_Name;
				counterSize_Name = 0;
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
				currentContacts3.sizeNumber = counterSize_Number;
				currentContacts3.phaseState = SAVE_CONTACTS;
				counterSize_Number = 0;
			}
			/**clear the reception flag*/
			setUART0_flag(FALSE);
		}
	}else{
		//Message in display of registers are full
		//Press ESC to exit

		if(getUART0_flag()){
			if(getUART0_mailBox() == ASCII_ESC){
				currentContacts3.phaseState = CONTACT_MENU;
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

	//Frame to show the contact list and button to edit it

	if(getUART0_flag()){
		if(getUART0_mailBox() == ASCII_0){cleanContact(1); currentContacts4.noContact = 1;}
		if(getUART0_mailBox() == ASCII_1){cleanContact(2); currentContacts4.noContact = 2;}
		if(getUART0_mailBox() == ASCII_2){cleanContact(3); currentContacts4.noContact = 3;}
		if(getUART0_mailBox() == ASCII_3){cleanContact(4); currentContacts4.noContact = 4;}
		if(getUART0_mailBox() == ASCII_4){cleanContact(5); currentContacts4.noContact = 5;}
		if(getUART0_mailBox() == ASCII_5){cleanContact(6); currentContacts4.noContact = 6;}
		if(getUART0_mailBox() == ASCII_6){cleanContact(7); currentContacts4.noContact = 7;}
		if(getUART0_mailBox() == ASCII_7){cleanContact(8); currentContacts4.noContact = 8;}
		if(getUART0_mailBox() == ASCII_8){cleanContact(9); currentContacts4.noContact = 9;}
		if(getUART0_mailBox() == ASCII_9){cleanContact(10); currentContacts4.noContact = 10;}

		ModeContact = MODE_EDIT;
		currentContacts4.phaseState = ADD_CONTACTS;
		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	/**Clear the mailbox**/
	clearUART0_mailbox();

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

	/**Save the name into memory*/
	for(counterSave = data.sizeName; counterSave != 0; counterSave--){
		positionAddress_Name += counterAddress;
		writeMemory(positionAddress_Name,data.name[counterAddress]);
		counterAddress++;
		E2PROMdelay(65000);
	}
	NoContacts++;
	counterSave = 0;
	counterAddress = 0;
	CurrentAddress_Name = NoContacts*NEXT_POSITION;

	/**Save the number into memory*/
	for(counterSave = data.sizeNumber; counterSave != 0; counterSave--){
		positionAddress_Number += counterAddress;
		writeMemory(positionAddress_Number,data.number[counterAddress]);
		counterAddress++;
		E2PROMdelay(65000);
	}
	ModeContact = MODE_ADD;
	writeMemory(POSITION_CONTACTS,(REGISTER_EMPTY + NoContacts));
	CurrentAddress_Number = CurrentAddress_Name + POSITION_NUMBER;
	currentContacts5.phaseState = CONTACT_MENU;
	currentContacts5.stateMain = CONTACTS;

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
		//LCDNokia_writeByte(LCD_CMD, 0xB1);
		flagContrast = FALSE;
	}

	startCompass();

	if(getUART0_flag()){
		if(getUART0_mailBox() == ASCII_ESC){
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
