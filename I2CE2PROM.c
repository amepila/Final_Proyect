 /*
 * I2CE2PROM.c
 *
 *  Created on: 08/11/2017
 *      Author: eric_
 */

#include "I2C.h"
#include "I2CE2PROM.h"

#define WRITECONTROL 0xA0
#define READCONTROL 0xA1

void writeMemory(uint16 add,uint8 data){
	writeI2CDevice2(WRITECONTROL,add,data);
}


uint8 readMemory(uint16 add){
	return readI2CDevice2(WRITECONTROL,READCONTROL,add);
}

void E2PROMdelay(uint32 delay){
	uint32 i=delay;
	while(i!=0){
		i--;
	}
}
