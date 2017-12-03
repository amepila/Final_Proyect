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
//Funcion para escribir en la I2CEEPROM por medio de una funcion general de I2C.
void writeMemory(uint16 add,uint8 data){
	uint8 addl=(add&0xFF);
	writeI2CDevice2(WRITECONTROL,addl,add>>8,data);
	E2PROMdelay(300000);
}

//Funcion para lectura de  la I2CEEPROM por medio de una funcion general de I2C
uint8 readMemory(uint16 add){
	uint8 addl=(add&0xFF);
	return	readI2CDevice(WRITECONTROL,READCONTROL,add>>8,addl);
}

//Delay especifico de la E2PROM
void E2PROMdelay(uint32 delay){
	uint32 i=delay;
	while(i!=0){
		i--;
	}
}
