/*
 * I2C.c
 *
 *  Created on: Oct 31, 2017
 *      Author: Andres Hernandez
 */

#include "I2C.h"
#include "GPIO.h"
uint8 PIT_control=0;
/*\brief funcion inicializar un modulo I2C de la k64 y tambien calcula el SCL respectivo
 * \param[channel] modulo I2C que se quiere utilizar
 *\param[systemClock] valor del reloj del sistema
 *\param[baudRate] Unidades de señal por segundo
 * */
void I2C_init(I2C_ChannelType channel, uint32 systemClock, uint32 baudRate){

	uint32 valueSCL;
	uint32 valueMULT = 4;
	static GPIO_pinControlRegisterType pinControlRegisterPortTx = GPIO_MUX5;
	static GPIO_pinControlRegisterType pinControlRegisterPortRx = GPIO_MUX5;

	GPIO_clockGating(GPIO_B);
	GPIO_clockGating(GPIO_E);
	SIM->SCGC1|=BIT_ON<<BIT6;
	SIM->SCGC4|=BIT_ON<<BIT6;
	SIM->SCGC4|=BIT_ON<<BIT7;

	GPIO_pinControlRegister(GPIO_E,BIT24,&pinControlRegisterPortTx);
	GPIO_dataDirectionPIN(GPIO_E,GPIO_OUTPUT,BIT24);

	GPIO_pinControlRegister(GPIO_E,BIT25,&pinControlRegisterPortRx);
	GPIO_dataDirectionPIN(GPIO_E,GPIO_INPUT,BIT25);

	valueSCL = systemClock/(baudRate*valueMULT);

	switch(channel){
	case I2C_0:

		I2C0->F |= I2C_F_ICR(valueSCL);// ICR = 75
		I2C0->F |= I2C_F_MULT(2);//MULT = 4
		I2C0->C1 |= I2C_C1_IICEN_MASK; //Enable I2C module

		break;
	case I2C_1:

		I2C1->F |= I2C_F_ICR(valueSCL);// ICR = 75
		I2C1->F |= I2C_F_MULT(2);//MULT = 4
		I2C1->C1 |= I2C_C1_IICEN_MASK; //Enable I2C module
		break;
	case I2C_2:

		I2C2->F |= I2C_F_ICR(valueSCL);// ICR = 75
		I2C2->F |= I2C_F_MULT(2);//MULT = 4
		I2C2->C1 |= I2C_C1_IICEN_MASK; //Enable I2C module
		break;
	default:
		break;
	}
}
/*\brief funcion que verifica que el modulo actual no este ocupado y pueda continuar
 *
 * */
uint8 I2C_busy(){
	if(FALSE == (I2C0->S & I2C_S_BUSY_MASK)){
		return TRUE;//I2C is idle
	}else{
		return FALSE;//I2C is busy
	}
}
/*\brief funcion que cambia el modulo de I2C entre maestro o esclavo
 *
 * */
void I2C_MST_OrSLV_Mode(uint8 mode){
	if(FALSE == mode){
		I2C0->C1 &= ~(I2C_C1_MST_MASK);//Slave mode
	}else{
		I2C0->C1 |= I2C_C1_MST_MASK; //Master mode
	}
}
/*\brief funcion que cambia el modulo de I2C entre transmisor y receptor
 *
 * */
void I2C_TX_RX_Mode(uint8 mode){

	if(FALSE == mode){
		I2C0->C1 &= ~(I2C_C1_TX_MASK); //Receive mode
		I2C0->C1 &= ~(I2C_C1_TXAK_MASK); //Receive mode

	}else{
		I2C0->C1 |= I2C_C1_TX_MASK; //Master mode
	}
}
/*\brief funcion que lanza una señal de not acknowledge para cortar la recepcion al estar leyendo datos
 *
 * */
void I2C_NACK(void){

	I2C0->C1 |= I2C_C1_TXAK_MASK;
}

void I2C_repeated_Start(void){

	I2C0->C1 |= I2C_C1_RSTA_MASK;

}

void I2C_write_Byte(uint8 data){

	I2C0->D = data;

}

uint8  I2C_read_Byte(void){
	uint8 data;

	data = I2C0->D;
	return data;
}
void I2C_wait(void){
	while((FALSE==(I2C0->S & I2C_S_IICIF_MASK))){}
 	I2C0->S |= I2C_S_IICIF_MASK;
 }

void I2C_get_ACK(void){
	while(FALSE != (I2C0->S & I2C_S_RXAK_MASK)){}
}

void I2C_start(void){

	I2C0->C1|=I2C_C1_TX_MASK;
	I2C0->C1|=I2C_C1_MST_MASK;
}


void I2C_stop(void){

	I2C0->C1&= ~I2C_C1_TX_MASK;
	I2C0->C1&= ~I2C_C1_MST_MASK;
}
void writeI2CDevice2(uint8 slaveAddress,uint16 add,uint8 data){
	uint8 Hadd=add>>BIT8;
	uint8 Ladd=add;

	I2C_start(); //Generate Start Signal
	I2C_write_Byte(slaveAddress); //Device the I2C wants to write to.
	I2C_wait();
	//I2C_get_ACK();

	I2C_write_Byte(Hadd); //Write High address to access	I2C_get_ACK();
	I2C_wait();
	//I2C_get_ACK();

	I2C_write_Byte(Ladd); //Write Low address to access	I2C_get_ACK();
	I2C_wait();
	//I2C_get_ACK();

	I2C_write_Byte(data);
	I2C_wait();
	//I2C_get_ACK();

	I2Cdelay(6500);
	I2C_stop(); //Stop transfer
	I2Cdelay(6500);
}
void writeI2CDevice(uint8 slaveAddressW,uint8 add,uint8 data){
	I2C_start(); //Generate Start Signal
	I2C_write_Byte(slaveAddressW); //Device the I2C wants to write to.
	I2C_wait();

	I2C_write_Byte(add); //Write address to access	I2C_get_ACK();
	I2C_wait();

	I2C_write_Byte(data);
	I2C_wait();
	I2C_stop(); //Stop transfer

	I2Cdelay(500);
}
uint8 readI2CDevice2(uint8 slaveAddresW,uint8 slaveAddressR,uint16 add){
	uint8 dummy=0;

	uint8 Hadd=add>>BIT8;
	uint8 Ladd=add;
	I2C_start(); //Generate Start Signal
	I2C_write_Byte(WRITECONTROL); //"10100000" Write control code, CSS and choose write
	I2C_wait();
	I2Cdelay(1000);

	I2C_write_Byte(Hadd); //Write first most significant 8 bits
	I2C_wait();
	I2Cdelay(1000);

	I2C_write_Byte(Ladd); //Write first most significant 8 bits
	I2C_wait();
	I2Cdelay(1000);
	I2C_repeated_Start();

	I2C_write_Byte(READCONTROL); //"10100000" Write control code, CSS and choose write
	I2C_wait();
	I2Cdelay(1000);

	I2C_TX_RX_Mode(I2C_RX_MODE);// Changing I2C module to receiver mode
	I2C_NACK();

	dummy=I2C_read_Byte();
	I2C_wait();

	I2C_stop();// Generating stop signal
	dummy=I2C_read_Byte();

	return dummy;
}
uint8 readI2CDevice(uint8 slaveAddressW,uint8 slaveAddressR,uint8 add){
	uint8 dummy=0;

	I2C_start(); //Generate Start Signal
	I2C_write_Byte(slaveAddressW); //"10100000" Write control code, CSS and choose write
	I2C_wait();

	I2C_write_Byte(add); //Write first most significant 8 bits
	I2C_wait();

	I2C_repeated_Start();

	I2C_write_Byte(slaveAddressR); //"10100000" Write control code, CSS and choose write
	I2C_wait();

	I2C_TX_RX_Mode(I2C_RX_MODE);// Changing I2C module to receiver mode
	I2C0->C1 &= ~I2C_C1_TXAK_MASK;

	I2C_NACK();

	dummy=I2C_read_Byte();
	I2C_wait();

	I2C_stop();// Generating stop signal
	dummy=I2C_read_Byte();
	I2Cdelay(500);
	return dummy;
}

void I2C0_IRQHandler(){

}

void I2Cdelay(uint32 delay){
	uint32 i=delay;
	while(i!=0){
		i--;
	}
}
