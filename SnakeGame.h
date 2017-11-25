/*
 * SnakeGame.h
 *
 *  Created on: Nov 17, 2017
 *      Author: Andres Hernandez
 */

#ifndef SNAKEGAME_H_
#define SNAKEGAME_H_

#include "DataTypeDefinitions.h"
#include "MK64F12.h"
#include "Phases.h"

typedef struct{
	uint8 score;
	uint8 lives;
}SnakeInfo_Type;

typedef struct{
	sint8 foodX;
	sint8 foodY1;
	sint8 foodY2;
}Food_Type;

typedef enum{DIRECTION_UP,
			DIRECTION_DOWN,
			DIRECTION_LEFT,
			DIRECTION_RIGHT
}Direction_Type;

typedef Direction_Type(*fptrStateMove)(void);

typedef const struct StateMove{
	Direction_Type(*stateMove)(void);
}StateMove_Type;

void edgeGame(void);
uint8 initialConditions(void);
uint8 foodGenerator(void);
Direction_Type moveUp(void);
Direction_Type moveDown(void);
Direction_Type moveLeft(void);
Direction_Type moveRight(void);
uint8 directMove(Direction_Type direction);
uint8 moveSnake(void);
uint8 runSnake(void);

#endif /* SNAKEGAME_H_ */
