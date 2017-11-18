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

/*
 * Escrituras estandar de I2C Start->WriteControl->Address->Data.
 *
 */
void writeMemory(uint16 add,uint8 data){
	uint8 Hadd=add>>BIT8;
	uint8 Ladd=add;
	I2C_start(); //Generate Start Signal
	I2C_write_Byte(WRITECONTROL); //"10100001" Write control code, CSS and choose write
	I2C_wait();
	I2C_get_ACK();
	E2PROMdelay(500);

	I2C_write_Byte(Hadd); //Write address to access	I2C_get_ACK();
	I2C_wait();
	I2C_get_ACK();
	E2PROMdelay(500);


	I2C_write_Byte(Ladd); //Write address to access	I2C_get_ACK();
	I2C_wait();
	I2C_get_ACK();
	E2PROMdelay(500);

	I2C_write_Byte(data);
	I2C_wait();
	I2C_get_ACK();
	I2C_wait();
	E2PROMdelay(500);

	I2C_stop(); //Stop transfer

	E2PROMdelay(500);
}

void ACKpollingW(){
	I2C_start();
	I2C_write_Byte(WRITECONTROL);
	I2C_get_ACK();
}
void ACKpollingR(){
	I2C_start();
	I2C_write_Byte(READCONTROL);
	I2C_get_ACK();
}
//Lectura estandar I2C
//Writecontrol->Direccion Alta->Direccion baja->Stop->Start(Repeated Start)->ReadControl->RX mode->DummyRead->Obtain Data

uint8 readMemory(uint16 add){
	uint8 dummy=0;

	uint8 Hadd=add>>BIT8;
	uint8 Ladd=add;
	I2C_start(); //Generate Start Signal
	I2C_write_Byte(WRITECONTROL); //"10100000" Write control code, CSS and choose write
	I2C_wait();
	I2C_get_ACK();
	E2PROMdelay(500);

	I2C_write_Byte(Hadd); //Write first most significant 8 bits
	I2C_wait();
	I2C_get_ACK();
	E2PROMdelay(500);

	I2C_write_Byte(Ladd); //Write least significant 8 bits
	I2C_wait();
	I2C_get_ACK();
	E2PROMdelay(500);

	I2C_repeated_Start();

	I2C_write_Byte(READCONTROL); //"10100000" Write control code, CSS and choose write
	I2C_wait();
	I2C_get_ACK();
	E2PROMdelay(500);

	I2C_TX_RX_Mode(I2C_RX_MODE);// Changing I2C module to receiver mode
	I2C_NACK();

	dummy=I2C_read_Byte();
	I2C_wait();
	E2PROMdelay(500);

	I2C_stop();// Generating stop signal
	dummy=I2C_read_Byte();
	return dummy;
}

void E2PROMdelay(uint32 delay){
	uint32 i=delay;
	while(i!=0){
		i--;
	}
}
