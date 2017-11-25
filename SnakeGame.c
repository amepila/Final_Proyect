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
#include "RNGA.h"

#define UP				(87)
#define DOWN			(83)
#define LEFT			(65)
#define RIGHT			(68)
#define BODY			(254)
#define TIME			(500000)
#define INITIAL_TIME	(1000000)
#define EDGE_UP			(0)
#define EDGE_DOWN		(6)
#define EDGE_LEFT		(0)
#define EDGE_RIGHT		(84)
#define EMPTY_BIT_Y2	(0x00)
#define FULL_BYTE_Y2	(0xFF)
#define MIN_LINES_Y1	(0)
#define MAX_LINES_Y1	(6)
#define LIMIT_UPCOUNT	(0)
#define LIMIT_DOWNCOUNT	(7)
#define BIT_MSB_Y2		(128)
#define BIT_LSB_Y2		(1)
#define LIMIT_RIGHT		(84)
#define LIMIT_LEFT		(0)

/**Current lenght of Snake**/
static uint32 LenghtSnake;
/**Number of points in the game**/
static uint32 Score;
/**Number of lives in the game**/
static uint32 Lives;
/**Axis X of snake position**/
static uint32 ComponentX[100];
/**Axis Y of snake position, is the number of line**/
static uint32 ComponentY1[100];
/**Second component of Y, is the address of each line**/
static uint32 ComponentY2[100];
/**Saves the current direction of snake**/
static Direction_Type CurrentDirection;
/**Value of axis X**/
static sint32 ValueX;
/**Value of number line of axis Y**/
static sint32 ValueY1;
/**Value of number of pixel of each line in axis Y**/
static sint32 ValueY2;
/**Mask value to Value Y2**/
static sint32 CounterBitY;
/**Previous move of the snake**/
static Direction_Type PreMove;

/**Array of pointers to functions that saves the snake moves**/
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


	return TRUE;
}

Direction_Type moveUp(void){

	/**General counter**/
	uint32 counter;
	/**Last value of X**/
	uint32 lastValueX;

	/**Save the captured value in Y1-axis**/
	lastValueX = ComponentX[LenghtSnake - 1];

	/**Loop to clear the previous snake to print the new snake**/
	for(counter = 0; counter < LenghtSnake; counter++){
		LCDNokia_gotoXY(ComponentX[counter],ComponentY1[counter]);
		LCDNokia_writeByte(LCD_DATA, 0x00);
	}
	/**Displace the snake to next position removing the first position**/
	for(counter = 0; counter < LenghtSnake; counter++){
		ComponentX[counter] = ComponentX[counter + 1];
		ComponentY1[counter] = ComponentY1[counter + 1];
		ComponentY2[counter] = ComponentY2[counter + 1];
	}
	/**Increment the value of X-axis**/
	CounterBitY--;
	/**Reset CounterBit if the snake reaches the limit in Y2**/
	if(CounterBitY < LIMIT_UPCOUNT){
		/**Counter bit is reseted**/
		CounterBitY = LIMIT_DOWNCOUNT;
		/**Value of Y2-axis is set to 128**/
		ValueY2 = BIT_MSB_Y2;
		/**Decrement the Y1-axis if Y2 is full**/
		ValueY1--;
		/**Reset Y1 is set at 6 if the limit is reached**/
		if(ValueY1 < MIN_LINES_Y1){ValueY1 = MAX_LINES_Y1;}
	}
	/**Decrement the position to Y2**/
	ValueY2 |= 1<<CounterBitY;

	/**Redefined the last position with the new values**/
	ComponentX[LenghtSnake - 1] = lastValueX;
	ComponentY1[LenghtSnake - 1] = ValueY1;
	ComponentY2[LenghtSnake - 1] = ValueY2;

	/**Print the new snake in the current position**/
	for(counter = 0; counter < LenghtSnake; counter++){
		LCDNokia_gotoXY(ComponentX[counter],ComponentY1[counter]);
		LCDNokia_writeByte(LCD_DATA,ComponentY2[counter]);
	}
	/**Return the up direction**/
	return (DIRECTION_UP);
}
Direction_Type moveDown(void){

	/**General counter**/
	uint32 counter;
	/**Last value of X**/
	uint32 lastValueX;

	/**Save the captured value in Y1-axis**/
	lastValueX = ComponentX[LenghtSnake - 1];

	/**Loop to clear the previous snake to print the new snake**/
	for(counter = 0; counter < LenghtSnake; counter++){
		LCDNokia_gotoXY(ComponentX[counter],ComponentY1[counter]);
		LCDNokia_writeByte(LCD_DATA, 0x00);
	}
	/**Displace the snake to next position removing the first position**/
	for(counter = 0; counter < LenghtSnake; counter++){
		ComponentX[counter] = ComponentX[counter + 1];
		ComponentY1[counter] = ComponentY1[counter + 1];
		ComponentY2[counter] = ComponentY2[counter + 1];
	}
	/**Increment the value of X-axis**/
	CounterBitY++;
	/**Reset CounterBit if the snake reaches the limit in Y2**/
	if(CounterBitY > LIMIT_DOWNCOUNT){
		/**Counter bit is reseted**/
		CounterBitY = LIMIT_UPCOUNT;
		/**Value of Y2-axis is set at 1**/
		ValueY2 = BIT_LSB_Y2;
		/**Increment the Y1-axis if Y2 is full**/
		ValueY1++;
		/**Reset Y1 is set at 0 if the limit is reached**/
		if(ValueY1 > MAX_LINES_Y1){ValueY1 = MIN_LINES_Y1;}
	}
	/**Increment the position to Y2**/
	ValueY2 |= 1<<CounterBitY;

	/**Redefined the last position with the new values**/
	ComponentX[LenghtSnake - 1] = lastValueX;
	ComponentY1[LenghtSnake - 1] = ValueY1;
	ComponentY2[LenghtSnake - 1] = ValueY2;

	/**Print the new snake in the current position**/
	for(counter = 0; counter < LenghtSnake; counter++){
		LCDNokia_gotoXY(ComponentX[counter],ComponentY1[counter]);
		LCDNokia_writeByte(LCD_DATA,ComponentY2[counter]);
	}
	/**Return the down direction**/
	return (DIRECTION_DOWN);
}
Direction_Type moveLeft(void){

	/**General counter**/
	uint32 counter;
	/**Last value of Y1**/
	uint32 lastValueY1;
	/**Last value of Y2**/
	uint32 lastValueY2;

	/**Save the captured value in Y1-axis**/
	lastValueY1 = ComponentY1[LenghtSnake - 1];
	/**Save the captured value in Y2-axis**/
	lastValueY2 = ComponentY2[LenghtSnake - 1];

	/**Loop to clear the previous snake to print the new snake**/
	for(counter = 0; counter < LenghtSnake; counter++){
		LCDNokia_gotoXY(ComponentX[counter],ComponentY1[counter]);
		LCDNokia_writeByte(LCD_DATA, 0x00);
	}
	/**Displace the snake to next position removing the first position**/
	for(counter = 0; counter < LenghtSnake; counter++){
		ComponentX[counter] = ComponentX[counter + 1];
		ComponentY1[counter] = ComponentY1[counter + 1];
		ComponentY2[counter] = ComponentY2[counter + 1];
	}
	/**Decrement the value of X-axis**/
	ValueX--;
	/**Reset if the snake reaches the limit**/
	if(ValueX < LIMIT_LEFT){ValueX = LIMIT_RIGHT;}

	/**Redefined the last position with the new values**/
	ComponentX[LenghtSnake - 1] = ValueX;
	ComponentY1[LenghtSnake - 1] = lastValueY1;
	ComponentY2[LenghtSnake - 1] = lastValueY2;

	/**Print the new snake in the current position**/
	for(counter = 0; counter < LenghtSnake; counter++){
		LCDNokia_gotoXY(ComponentX[counter],ComponentY1[counter]);
		LCDNokia_writeByte(LCD_DATA,ComponentY2[counter]);
	}
	/**Return the left direction**/
	return (DIRECTION_LEFT);
}
Direction_Type moveRight(void){

	/**General counter**/
	uint32 counter;
	/**Last value of Y1**/
	uint32 lastValueY1;
	/**Last value of Y2**/
	uint32 lastValueY2;

	/**Save the captured value in Y1-axis**/
	lastValueY1 = ComponentY1[LenghtSnake - 1];
	/**Save the captured value in Y2-axis**/
	lastValueY2 = ComponentY2[LenghtSnake - 1];

	/**Loop to clear the previous snake to print the new snake**/
	for(counter = 0; counter < LenghtSnake; counter++){
		LCDNokia_gotoXY(ComponentX[counter],ComponentY1[counter]);
		LCDNokia_writeByte(LCD_DATA, 0x00);
	}
	/**Displace the snake to next position removing the first position**/
	for(counter = 0; counter < LenghtSnake; counter++){
		ComponentX[counter] = ComponentX[counter + 1];
		ComponentY1[counter] = ComponentY1[counter + 1];
		ComponentY2[counter] = ComponentY2[counter + 1];
	}
	/**Increment the value of X-axis**/
	ValueX++;
	/**Reset if the snake reaches the limit**/
	if(ValueX > LIMIT_RIGHT){ValueX = LIMIT_LEFT;}

	/**Redefined the last position with the new values**/
	ComponentX[LenghtSnake - 1] = ValueX;
	ComponentY1[LenghtSnake - 1] = lastValueY1;
	ComponentY2[LenghtSnake - 1] = lastValueY2;

	/**Print the new snake in the current position**/
	for(counter = 0; counter < LenghtSnake; counter++){
		LCDNokia_gotoXY(ComponentX[counter],ComponentY1[counter]);
		LCDNokia_writeByte(LCD_DATA,ComponentY2[counter]);
	}
	/**Return the right direction**/
	return (DIRECTION_RIGHT);
}

uint8 directMove(Direction_Type direction){

	delay(TIME);
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
		delay(INITIAL_TIME);
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
