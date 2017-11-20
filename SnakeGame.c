/*
 * Snake.c
 *
 *  Created on: Nov 17, 2017
 *      Author: Andres Hernandez
 */

#include "SnakeGame.h"
#include "SPI.h"
#include "LCDNokia5110.h"
#include "UART.h"
#include "PIT.h"

#define UP				(87)
#define DOWN			(83)
#define LEFT			(65)
#define RIGHT			(68)
#define BODY			(254)
#define SYSTEM_CLOCK	(60000000)

static uint32 EdgeUp = 0;
static uint32 EdgeDown = 6;
static uint32 EdgeLeft = 0;
static uint32 EdgeRight = 84;
static uint32 LenghtSnake;
static uint32 Score;
static uint32 Lives;
static Direction_Type CurrentDirection = DIRECTION_RIGHT;

const StateMove_Type StateMove[4] =
{
		{moveUp},
		{moveDown},
		{moveLeft},
		{moveRight}
};

void edgeGame(void){

	uint8 counter;

	/**Draw the Top edge*/
	for(counter = 0; counter < 84; counter++){
		LCDNokia_gotoXY(counter,0);
		LCDNokia_writeByte(LCD_DATA,0x01);
	}

	/**Draw the Down edge*/
	for(counter = 0; counter < 84; counter++){
		LCDNokia_gotoXY(counter,5);
		LCDNokia_writeByte(LCD_DATA,0x80);
	}

	/**Draw the Right edge*/
	for(counter = 0; counter < 6; counter++){
		LCDNokia_gotoXY(83,counter);
		LCDNokia_writeByte(LCD_DATA,0xFF);
		delay(40);
	}

	/**Draw the Left edge*/
	for(counter = 0; counter < 6; counter++){
		LCDNokia_gotoXY(0,counter);
		LCDNokia_writeByte(LCD_DATA,0xFF);
		delay(40);
	}
}

uint8 initialPosition(void){
	uint8 counter;

	LCDNokia_gotoXY(15,0);

	for(counter = 15; counter < 30; counter++){
		LCDNokia_gotoXY(counter,4);
		LCDNokia_writeByte(LCD_DATA,0x01);
	}
	return (TRUE);
}
uint8 foodGenerator(void){
}

Direction_Type moveUp(void){


	return (DIRECTION_UP);
}
Direction_Type moveDown(void){

	return (DIRECTION_DOWN);
}
Direction_Type moveLeft(void){

	return (DIRECTION_LEFT);
}
Direction_Type moveRight(void){

	return (DIRECTION_RIGHT);
}

uint8 directMove(Direction_Type direction){

	//PIT_delay(PIT_0,SYSTEM_CLOCK,1000);
	Direction_Type(*moveFunctions)(void);
	CurrentDirection = direction;

	moveFunctions = StateMove[CurrentDirection].stateMove;
	CurrentDirection = moveFunctions();
	return TRUE;
}

uint8 moveSnake(void){

	if(getUART0_flag()){
		if(getUART0_mailBox() == UP){CurrentDirection = DIRECTION_UP;}
		if(getUART0_mailBox() == DOWN){CurrentDirection = DIRECTION_DOWN;}
		if(getUART0_mailBox() == LEFT){CurrentDirection = DIRECTION_LEFT;}
		if(getUART0_mailBox() == RIGHT){CurrentDirection = DIRECTION_RIGHT;}

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	/**Clear the mailbox**/
	clearUART0_mailbox();
	directMove(CurrentDirection);
	return TRUE;
}

SnakeInfo_Type runSnake(void){

	static SnakeInfo_Type currentInfo;
	static uint8 flagDefault = FALSE;

	edgeGame();
	if(FALSE == flagDefault){
		initialPosition();
		Lives = 3;
		Score = 0;
		LenghtSnake = 15;
		flagDefault = TRUE;
		//Delay of 2 seconds
	}
	currentInfo.score = Score;
	currentInfo.lives = Lives;
	moveSnake();

	if(currentInfo.lives == 0){
		flagDefault = FALSE;
		Lives = 3;
		currentInfo.score = Score;
		Score = 0;
	}
	return (currentInfo);
}
