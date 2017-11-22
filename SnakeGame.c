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
#define EDGE_UP			(0)
#define EDGE_DOWN		(6)
#define EDGE_LEFT		(0)
#define EDGE_RIGHT		(84)
#define EMPTY_BYTE_Y2	(0x00)
#define FULL_BYTE_Y2	(255)

static uint32 LenghtSnake;
static uint32 Score;
static uint32 Lives;
/**Axis X of snake position**/
static uint32 ComponentX[100];
/**Axis Y of snake position, is the number of line**/
static uint32 ComponentY1[100];
/**Second component of Y, is the address of each line**/
static uint32 ComponentY2[100];
/**Saves the current direction of snake**/
static Direction_Type CurrentDirection;

static uint32 ValueX;
static uint32 ValueY1;
static uint32 ValueY2;
static uint8 CounterBitY;

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

	ValueX = 14;
	ValueY1 = 4;
	ValueY2 = 1;
	CounterBitY = 0;

	LenghtSnake = 15;
	LCDNokia_gotoXY(30,4);
	CurrentDirection = DIRECTION_RIGHT;

	/**Save the initial coordinates**/
	for(counter = 0; counter < LenghtSnake; counter++){
		ComponentX[counter] = ValueX + counter;
		ComponentY1[counter] = ValueY1;
		ComponentY2[counter] = ValueY2;
	}
	/**Draw the snake**/
	for(counter = 0; counter < LenghtSnake; counter++){
		LCDNokia_gotoXY(ComponentX[counter],ComponentY1[counter]);
		LCDNokia_writeByte(LCD_DATA,ComponentY2[counter]);
	}
	return (TRUE);
}
uint8 foodGenerator(void){
}

Direction_Type moveUp(void){


	return (DIRECTION_UP);
}
Direction_Type moveDown(void){

	uint32 counter;
	static counterBit;

	CounterBitY++;
	ValueY2 |= 1<<CounterBitY;
	for(counter = 0; counter < LenghtSnake; counter++){
		if(ValueY2 == FULL_BYTE_Y2){
			ValueY2 = EMPTY_BYTE_Y2;
			ValueY1++;
		}
		ComponentX[counter] = ValueX;
		ComponentY1[counter] = ValueY1;
		ComponentY2[counter] |= ValueY2;
	}
	for(counter = 0; counter < LenghtSnake; counter++){
		LCDNokia_gotoXY(ComponentX[counter],ComponentY1[counter]);
		LCDNokia_writeByte(LCD_DATA,ComponentY2[counter]);
	}
	return (DIRECTION_DOWN);
}
Direction_Type moveLeft(void){

	uint32 counter;

	ValueX--;
	for(counter = 0; counter < LenghtSnake; counter++){
		ComponentX[counter] = ValueX + counter;
		ComponentY1[counter] = ValueY1;
		ComponentY2[counter] = ValueY2;
	}

	for(counter = 0; counter < LenghtSnake; counter++){
		LCDNokia_gotoXY(ComponentX[counter],ComponentY1[counter]);
		LCDNokia_writeByte(LCD_DATA,ComponentY2[counter]);
	}
	return (DIRECTION_LEFT);
}
Direction_Type moveRight(void){

	uint32 counter;

	ValueX++;
	for(counter = 0; counter < LenghtSnake; counter++){
		ComponentX[counter] = ValueX + counter;
		ComponentY1[counter] = ValueY1;
		ComponentY2[counter] = ValueY2;
	}
	for(counter = 0; counter < LenghtSnake; counter++){
		LCDNokia_gotoXY(ComponentX[counter],ComponentY1[counter]);
		LCDNokia_writeByte(LCD_DATA,ComponentY2[counter]);
	}
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
