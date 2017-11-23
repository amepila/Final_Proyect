/*
 * RNGA.c
 *
 *  Created on: Nov 23, 2017
 *      Author: Andres Hernandez
 */

#include "RNGA.h"
#include "MK64F12.h"
#include "GPIO.h"

uint32 RandomData;

void enableRNGA(void){
	SIM->SCGC3 |= SIM_SCGC3_RNGA_MASK;
	SIM->SCGC6 |= SIM_SCGC6_RNGA_MASK;
	RNG->CR |= RNG_CR_INTM_MASK;
	RNG->CR |= RNG_CR_HA_MASK;
	RNG->CR |= RNG_CR_GO_MASK;
}

void initRNGA(){
	enableRNGA();
	while((RNG->SR & RNG_SR_OREG_LVL_MASK) == 0);
	RandomData = RNG->OR;
}

uint32 getRandomData(void){
	return (RandomData);
}
