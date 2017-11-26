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

/**ASCII W to move up**/
#define UP				(87)
/**ASCII S to move down**/
#define DOWN			(83)
/**ASCII A to move left**/
#define LEFT			(65)
/**ASCII D to move right**/
#define RIGHT			(68)
/**Time to launch the new snake**/
#define TIME			(500000)
/**Time to wait the initial game**/
#define INITIAL_TIME	(1000000)
/**Coordinate of edge up**/
#define EDGE_UP			(0)
/**Coordinate of edge down**/
#define EDGE_DOWN		(6)
/**Coordinate of edge left**/
#define EDGE_LEFT		(0)
/**Coordinate of edge right**/
#define EDGE_RIGHT		(84)
/**Bit of Y2 is empty**/
#define EMPTY_BIT_Y2	(0x00)
/**Bit of y2 is full**/
#define FULL_BYTE_Y2	(0xFF)
/**Minimum of lines in Y1**/
#define MIN_LINES_Y1	(0)
/**Maximum lines in Y1**/
#define MAX_LINES_Y1	(6)
/**Limit of count in BitY2**/
#define LIMIT_UPCOUNT	(0)
/**Limit of count in BitY2**/
#define LIMIT_DOWNCOUNT	(7)
/**Bit MSB in Y2**/
#define BIT_MSB_Y2		(128)
/**Bit LSB in Y2**/
#define BIT_LSB_Y2		(1)
/**Limit in the right border**/
#define LIMIT_RIGHT		(84)
/**Limit in the left border**/
#define LIMIT_LEFT		(0)
/**Limit of count in Y2**/
#define LIMIT_BIT8		(255)
/**Food eaten**/
#define SUCCESS_FOOD	(3)

/**Current length of Snake**/
static uint32 LenghtSnake;
/**Save the current info of the game**/
static SnakeInfo_Type InfoSnake;
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
/**Coordinates to the food**/
static Food_Type FoodSnake;
/**Flag to generate new food**/
static uint8 FlagFood = TRUE;

/**Array of pointers to functions that saves the snake moves**/
const StateMove_Type StateMove[4] =
{
		{moveUp},
		{moveDown},
		{moveLeft},
		{moveRight}
};

void edgeGame(void){
	/**General counter**/
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

uint8 initialConditions(void){

	/**General counter**/
	uint8 counter;

	/**Initial value to X**/
	ValueX = 14;
	/**Initial value to Y1**/
	ValueY1 = 4;
	/**Initial value to Y2**/
	ValueY2 = 1;
	/**Initial value to BIT**/
	CounterBitY = 0;
	/**Initial length of snake**/
	LenghtSnake = 15;

	/**Initial axis in Nokia**/
	LCDNokia_gotoXY(30,4);
	/**Initial direction**/
	CurrentDirection = DIRECTION_RIGHT;
	/**Initial info to game**/
	InfoSnake.lives = 3;
	InfoSnake.score = 0;

	/**Create the borders of the game**/
	//edgeGame();

	/**Save the initial coordinates**/
	for(counter = 0; counter < LenghtSnake; counter++){
		ComponentX[counter] = ValueX + counter;
		ComponentY1[counter] = ValueY1;
		ComponentY2[counter] = ValueY2;
	}
	/**Print the position initial to snake**/
	for(counter = 0; counter < LenghtSnake; counter++){
		LCDNokia_gotoXY(ComponentX[counter],ComponentY1[counter]);
		LCDNokia_writeByte(LCD_DATA,ComponentY2[counter]);
	}
	/**Success in this function**/
	return (TRUE);
}

uint8 foodGenerator(void){

	/**Get the food in X axis and adjust to new parameters**/
	FoodSnake.foodX = getRandomData();
	FoodSnake.foodX %= LIMIT_RIGHT;

	/**Get the food in Y1 axis and adjust to new parameters**/
	FoodSnake.foodY1 = getRandomData();
	FoodSnake.foodY1 %= MAX_LINES_Y1;

	/**Get the food in Y2 axis and adjust to new parameters**/
	FoodSnake.foodY2 = getRandomData();
	FoodSnake.foodY2 %= LIMIT_DOWNCOUNT;
	FoodSnake.bitY2 = FoodSnake.foodY2;
	FoodSnake.foodY2 = 1<<FoodSnake.foodY2;

	/**Print the food to snake**/
	LCDNokia_gotoXY(FoodSnake.foodX,FoodSnake.foodY1);
	LCDNokia_writeByte(LCD_DATA,FoodSnake.foodY2);

	/**Lock this function until the snake eats the food*/
	FlagFood = FALSE;
	/**Success in this function**/
	return TRUE;
}

void foodEatenX(uint32 axisX, uint32 axisY1, uint32 axisY2){

	/**Counter of success**/
	uint8 counter = 0;

	/**Comparison with axis X**/
	if(FoodSnake.foodX == axisX){counter++;}
	/**Comparison with axis Y1**/
	if(FoodSnake.foodY1 == axisY1){counter++;}
	/**Comparison with axis Y2**/
	if(FoodSnake.foodY2 == axisY2){counter++;}

	/**If the food is eaten then snake grows**/
	if(counter == SUCCESS_FOOD){
		LenghtSnake++;
		FlagFood = TRUE;
	}
}

void foodEatenY(uint32 axisX, uint32 axisY1){

	/**Counter of success**/
	uint8 counter = 0;

	/**Comparison with axis X**/
	if(FoodSnake.foodX == axisX){counter++;}
	/**Comparison with axis Y1**/
	if(FoodSnake.foodY1 == axisY1){counter++;}
	/**Comparison with bit of Y2**/
	if(FoodSnake.bitY2 == CounterBitY){counter++;}

	/**If the food is eaten then snake grows**/
	if(counter == SUCCESS_FOOD){
		LenghtSnake++;
		FlagFood = TRUE;
	}
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
	/**Increment the value of position in Y**/
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
	/**Verifies if the snake found the food**/
	foodEatenY(ComponentX[LenghtSnake - 1],
				ComponentY1[LenghtSnake - 1]);

	/**If the snake ate then is added a new position**/
	if(TRUE == FlagFood){
		/**Increment the value of position in Y**/
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

		/**Add the new position to grown snake**/
		ComponentX[LenghtSnake - 1] = lastValueX;
		ComponentY1[LenghtSnake - 1] = ValueY1;
		ComponentY2[LenghtSnake - 1] = ValueY2;
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
	/**Increment the value of position in Y**/
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
	/**Verifies if the snake found the food**/
	foodEatenY(ComponentX[LenghtSnake - 1],
				ComponentY1[LenghtSnake - 1]);

	/**If the snake ate then is added a new position**/
	if(TRUE == FlagFood){
		/**Increment the value of position in Y**/
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

		/**Add the new position to grown snake**/
		ComponentX[LenghtSnake - 1] = lastValueX;
		ComponentY1[LenghtSnake - 1] = ValueY1;
		ComponentY2[LenghtSnake - 1] = ValueY2;
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
	lastValueY2 = 1<<CounterBitY;
	/**Assign the new value to Y2**/
	ValueY2 = lastValueY2;

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
	/**Verifies if the snake found the food**/
	foodEatenX(ComponentX[LenghtSnake - 1],
				ComponentY1[LenghtSnake - 1],
				ComponentY2[LenghtSnake - 1]);

	/**If the snake ate then is added a new position**/
	if(TRUE == FlagFood){
		/**Decrement the value of X-axis**/
		ValueX--;
		/**Reset if the snake reaches the limit**/
		if(ValueX < LIMIT_LEFT){ValueX = LIMIT_RIGHT;}

		/**Add the new position to grown snake**/
		ComponentX[LenghtSnake - 1] = ValueX;
		ComponentY1[LenghtSnake - 1] = lastValueY1;
		ComponentY2[LenghtSnake - 1] = lastValueY2;
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
	lastValueY2 = 1<<CounterBitY;
	/**Assign the new value to Y2**/
	ValueY2 = lastValueY2;

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
	/**Verifies if the snake found the food**/
	foodEatenX(ComponentX[LenghtSnake - 1],
				ComponentY1[LenghtSnake - 1],
				ComponentY2[LenghtSnake - 1]);

	/**If the snake ate then is added a new position**/
	if(TRUE == FlagFood){
		/**Increment the value of X-axis**/
		ValueX++;
		/**Reset if the snake reaches the limit**/
		if(ValueX > LIMIT_RIGHT){ValueX = LIMIT_LEFT;}

		/**Add the new position to grown snake**/
		ComponentX[LenghtSnake - 1] = ValueX;
		ComponentY1[LenghtSnake - 1] = lastValueY1;
		ComponentY2[LenghtSnake - 1] = lastValueY2;
	}
	/**Return the right direction**/
	return (DIRECTION_RIGHT);
}

uint8 directMove(Direction_Type direction){

	/**States machine to snake moves with the current direction**/
	Direction_Type(*moveFunctions)(void);
	CurrentDirection = direction;
	/**Wait the next move of snake**/
	delay(TIME);

	/**Pointer to functions to elect the move**/
	moveFunctions = StateMove[CurrentDirection].stateMove;
	/**Execute the move**/
	CurrentDirection = moveFunctions();
	/**Success with the function**/
	return TRUE;
}

uint8 moveSnake(void){

	/**Detect the commands to move the snake**/
	if(getUART0_flag()){
		/**Character W is move up**/
		if(getUART0_mailBox() == UP){CurrentDirection = DIRECTION_UP;}
		/**Character S is move down**/
		if(getUART0_mailBox() == DOWN){CurrentDirection = DIRECTION_DOWN;}
		/**Character A is move left**/
		if(getUART0_mailBox() == LEFT){CurrentDirection = DIRECTION_LEFT;}
		/**Character D is move right**/
		if(getUART0_mailBox() == RIGHT){CurrentDirection = DIRECTION_RIGHT;}

		/**clear the reception flag*/
		setUART0_flag(FALSE);
	}
	/**Clear the mailbox**/
	clearUART0_mailbox();
	/**Execute the function to move the snake**/
	directMove(CurrentDirection);
	/**Success with the function**/
	return TRUE;
}

uint8 runSnake(void){

	/**Lock to execute once the initial position**/
	static uint8 flagDefault = FALSE;

	/**Draw the initial conditionals and wait 1 second**/
	if(FALSE == flagDefault){
		initialConditions();
		flagDefault = TRUE;
		delay(INITIAL_TIME);
	}
	/**Generate the food until is captured**/
	if(TRUE == FlagFood){foodGenerator();}

	/**Print the snake food**/
	LCDNokia_gotoXY(FoodSnake.foodX,FoodSnake.foodY1);
	LCDNokia_writeByte(LCD_DATA,FoodSnake.foodY2);

	/**Move the snake**/
	moveSnake();

	/**If the player looses then reset the initial conditions**/
	if(InfoSnake.lives == 0){flagDefault = FALSE;}
	/**Return the score of the game played**/
	return (InfoSnake.score);
}
