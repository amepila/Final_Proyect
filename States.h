/*
 * Menu.h
 *
 *  Created on: Oct 15, 2017
 *      Author: Andres Hernandez
 */

#ifndef STATES_H_
#define STATES_H_

#include "DataTypeDefinitions.h"
#include "MK64F12.h"
#include "SPI.h"
#include "GPIO.h"
#include "I2C.h"
#include "UART.h"


/**
 * \brief This data type define the states of program
 */
typedef enum{
	MAIN_MENU,
	CONTACTS,
	MESSAGES,
	SNAKE_GAME,
	COMPASS,
	WALLPAPER
}States_MenuType;

/**
 * \brief This data type define the Lock Flags to general use
 */
typedef struct{
	uint32 flag1 : 1;
	uint32 flag2 : 1;
	uint32 flag3 : 1;
	uint32 flag4 : 1;
	uint32 flag5 : 1;
	uint32 flag6 : 1;
	uint32 flag7 : 1;
	uint32 flag8 : 1;
}LockFlags_Type;

/*Data type to main functions*/
typedef States_MenuType(*fptrState)(void);

/**Data type to machine state**/
typedef const struct State{
	States_MenuType (*stateFunction)(void);
}StateType;


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Main state of Main Menu
 	 \param[in]  void
 	 \return The next state
 */
States_MenuType stateMainMenu(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Main state of edit the contacts
 	 \param[in]  void
 	 \return The next state
 */
States_MenuType stateContacts(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Main state of the messages
 	 \param[in]  void
 	 \return The next state
 */
States_MenuType stateMessages(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Main state of the snake game
 	 \param[in]  void
 	 \return The next state
 */
States_MenuType stateSnakeGame(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Main state of compass
 	 \param[in]  void
 	 \return The next state
 */
States_MenuType stateCompass(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Main state of set the wallpaper
 	 \param[in]  void
 	 \return The next state
 */
States_MenuType stateWallpaper(void);

#endif /* STATES_H_ */
