/*
 * RNGA.c
 *
 *  Created on: Nov 23, 2017
 *      Author: Andres Hernandez
 */

#include "RNGA.h"
#include "MK64F12.h"
#include "GPIO.h"

/**Data with random numbers**/
uint32 RandomData;


void enableRNGA(void){
	/**Enable the clocks*/
	SIM->SCGC3 |= SIM_SCGC3_RNGA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_RNGA_MASK;
	/**Set the configurations **/
	RNG->CR |= RNG_CR_INTM_MASK;
	RNG->CR |= RNG_CR_HA_MASK;
	RNG->CR |= RNG_CR_GO_MASK;
}

uint32 getRandomData(void){
	/**Wait for the data*/
	while((RNG->SR & RNG_SR_OREG_LVL_MASK) == 0);
	/**Saves the data in variable*/
	RandomData = RNG->OR;
	/**Launch the data*/
	return (RandomData);
}
