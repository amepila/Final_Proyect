/*
 * Foona800.C
 *
 *  Created on: Nov 30, 2017
 *      Author: Andres Hernandez
 */

#include "Foona800.h"
#include "UART.h"

/**Commands to FOONA 800**/

const uint8 ReadMsg[] = "AT+CMGR";
const uint8 SendMsg[] = "AT+CMGS";

void sendMessage(FIFO_Type fifo){

	FIFO_Type tempFIFO;

	tempFIFO = popFIFO_1();

	UART_putString(UART_1,&SendMsg);
}

uint8 readMessage(){

	return TRUE;
}
