/*
 * UART.c
 *
 *  Created on: Oct 10, 2017
 *      Author: Andres Hernandez
 */
#include "UART.h"
#include "GPIO.h"
#define S 65000

/*Global variable that saves the info*/
UART_MailBoxType UART0_MailBox;
UART_MailBoxType UART1_MailBox;

FIFO_Type FIFO_UART0;
FIFO_Type FIFO_UART1;

/*Table of BRFD. Range 0:31 */
const float BRFD_Type[32] = {
		(0),	(0.03125),	(0.0625),	(0.09375),	(0.125),	(0.15625),	(0.1875),	(0.21875),
		(0.25), (0.28125),	(0.3125),	(0.34375),	(0.375),	(0.40625),	(0.4375),	(0.46875),
		(0.5),	(0.53125),	(0.5625),	(0.59375),	(0.625),	(0.65625),	(0.6875),	(0.71875),
		(0.75),	(0.78125),	(0.8125),	(0.84375),	(0.875),	(0.90625),	(0.9375),	(0.96875)
};

void UART0_RX_TX_IRQHandler (void){
	/*First is verified if the serial port finished to transmit*/
	while(!(UART0->S1 & UART_S1_RDRF_MASK));
	/*The info is saved in Data Register*/
	UART0_MailBox.mailBox = UART0->D;
	/*There are new data*/
	UART0_MailBox.flag = 1;

}

void UART1_RX_TX_IRQHandler (void){
	/*First is verified if the serial port finished to transmit*/
	while(!(UART1->S1 & UART_S1_RDRF_MASK));
	/*The info is saved in Data Register*/
	UART1_MailBox.mailBox = UART1->D;
	/*There are new data*/
	UART1_MailBox.flag = 1;

}


uint8 getUART0_mailBox(){
	/*Return the value of mailbox*/
	return (UART0_MailBox.mailBox);
}

uint8 getUART1_mailBox(){
	/*Return the value of mailbox*/
	return (UART1_MailBox.mailBox);
}

uint8 getUART0_flag(){
	/*Return the value of the flag of mailbox*/
	return (UART0_MailBox.flag);
}

uint8 getUART1_flag(){
	/*Return the value of the flag of mailbox*/
	return (UART1_MailBox.flag);
}

void setUART0_mailBox(uint8 character){
	/*Assigns the character into the mailbox*/
	UART0_MailBox.mailBox = character;
}

void setUART1_mailBox(uint8 character){
	/*Assigns the character into the mailbox*/
	UART1_MailBox.mailBox = character;
}

void setUART0_flag(uint8 status){
	/*Changes the value of the flag of mailbox*/
	UART0_MailBox.flag = status;
}

void setUART1_flag(uint8 status){
	/*Changes the value of the flag of mailbox*/
	UART1_MailBox.flag = status;
}


void delay(uint32 delay){
	volatile uint32 counter;

	for(counter=delay;counter>0;counter--){
	}
}

void UART0_init(UART_ChannelType uartChannel, uint32 systemClk, UART_BaudRateType baudRate){

	/*Variable saves the original value of Baud Rate*/
	float sBR;
	/*Variables saves the Baud Rate by parts*/
	uint32 sbr_HIGH;
	uint32 sbr_LOW;
	/*Variable saves the complete Baud Rate*/
	uint32 sbr_TEMP;
	/*Variables saves the Baud Rate Fine Adjust*/
	uint8 BRFA;
	/*Variables saves the difference of BaudRate expected and the original*/
	float diff;
	/*Counter that advances between BRFD values*/
	uint8 counter;
	/*Variable saves the BRFD value
	 *its default value is 1 for practicality
	 */
	float BRFD = 1;


	/**Enables the clock of PortB in order to configures TX and RX of UART peripheral*/
	SIM->SCGC5 = SIM_SCGC5_PORTB_MASK;
	/**Configures the pin control register of pin16 in PortB as UART RX*/
	PORTB->PCR[16] = PORT_PCR_MUX(3);
	/**Configures the pin control register of pin17 in PortB as UART TX*/
	PORTB->PCR[17] = PORT_PCR_MUX(3);

	/*Isolate the baudRate variable
	 * 115200 = 21000000/((SBR+BRFD)*16)
	 * sBR+BRFD = 11.3932
	 * Assume BRFD = 0
	 * sBR = 11.3932 - BRFD
	 * sBR = 11.3932 - 0.375
	 * BRFD = 0.375 = 12/32 = 01100
	 * 115384 BR
	 */

	/*Isolate the SBR value assuming that BRFD = 0*/
	sBR = ((float)systemClk/(baudRate*16));
	/*Obtains the value of the float difference of SBR*/
	diff = (sBR - (uint32)sBR);

	/*Loop that verifies with the BRFD values*/
	for(counter = 0; counter < 32; counter++){

		/*Verifies that the difference is between some value of BRFA*/
		if((diff > BRFD_Type[counter-1]) && (diff < BRFD_Type[counter+1])){
			/*Verifies that the BRFD is the value with smaller difference*/
			if(BRFD > BRFD_Type[counter]){
				/*Saves the values of BRFD and BRFA*/
				BRFD = BRFD_Type[counter];
				BRFA = counter;
			}
		}
		/*Special case with the initial value of BRFA*/
		if(counter == 0){
			/*Verifies that the difference is between some value of BRFA*/
			if((diff >= BRFD_Type[counter]) && (diff < BRFD_Type[counter+1])){
				/*Verifies that the BRFD is the value with smaller difference*/
				if(BRFD > BRFD_Type[counter]){
					/*Saves the values of BRFD and BRFA*/
					BRFD = BRFD_Type[counter];
					BRFA = counter;
				}
			}
		}
		/*Special case with the final value of BRFA*/
		if(counter == 31){
			/*Verifies that the difference is between some value of BRFA*/
			if((diff <= BRFD_Type[counter]) && (diff > BRFD_Type[counter-1])){
				/*Verifies that the BRFD is the value with smaller difference*/
				if(BRFD > BRFD_Type[counter]){
					/*Saves the values of BRFD and BRFA*/
					BRFD = BRFD_Type[counter];
					BRFA = counter;
				}
			}
		}
	}

	/*Considers only the integer part*/
	sbr_TEMP = (uint32)sBR;
	/*Saves the High part of Baud Rate*/
	sbr_HIGH = sbr_TEMP & UART_SBR_MASK_HIGH;
	/*Saves the Low part of Baud Rate*/
	sbr_LOW = sbr_TEMP & UART_SBR_MASK_LOW;

	/*Enable the clock of UART 0 */
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
	/*Disable the Rx of UART*/
	UART0->C2 &= ~(UART_C2_RE_MASK);
	/*Disable the Tx of UART*/
	UART0->C2 &= ~(UART_C2_TE_MASK);
	/*Default settings*/
	UART0->C1 = 0;


	/*Clear the High part of SBR*/
	UART0->BDH &= ~(UART_CLEAR_BDH);
	/*Send the High part of SBR*/
	UART0->BDH |= sbr_HIGH;
	/*Clear the Low part of SBR*/
	UART0->BDL &= ~(UART_CLEAR_BDL);
	/*Send the Low part of SBR*/
	UART0->BDL |= sbr_LOW;


	/*Send the Baud Rate Fine Adjust to register*/
	UART0->C4 |= BRFA;
	/*Enable Rx of UART*/
	UART0->C2 |= (UART_C2_RE_MASK);
	/*Enable Tx of UART*/
	UART0->C2 |= (UART_C2_TE_MASK);
}

void UART1_init(UART_ChannelType uartChannel, uint32 systemClk, UART_BaudRateType baudRate){

	/*Variable saves the original value of Baud Rate*/
	float sBR;
	/*Variables saves the Baud Rate by parts*/
	uint32 sbr_HIGH;
	uint32 sbr_LOW;
	/*Variable saves the complete Baud Rate*/
	uint32 sbr_TEMP;
	/*Variables saves the Baud Rate Fine Adjust*/
	uint8 BRFA;
	/*Variables saves the difference of BaudRate expected and the original*/
	float diff;
	/*Counter that advances between BRFD values*/
	uint8 counter;
	/*Variable saves the BRFD value
	 *its default value is 1 for practicality
	 */
	float BRFD = 1;


	/**Enables the clock of PortC in order to configures TX and RX of UART peripheral*/
	SIM->SCGC5 = SIM_SCGC5_PORTC_MASK;

	/**Configures the pin control register of pin3 in PortC as UART RX*/
	PORTC->PCR[3] = PORT_PCR_MUX(3);
	/**Configures the pin control register of pin4 in PortC as UART TX*/
	PORTC->PCR[4] = PORT_PCR_MUX(3);

	/*Isolate the baudRate variable
	 * 115200 = 21000000/((SBR+BRFD)*16)
	 * sBR+BRFD = 11.3932
	 * Assume BRFD = 0
	 * sBR = 11.3932 - BRFD
	 * sBR = 11.3932 - 0.375
	 * BRFD = 0.375 = 12/32 = 01100
	 * 115384 BR
	 */

	/*Isolate the SBR value assuming that BRFD = 0*/
	sBR = ((float)systemClk/(baudRate*16));
	/*Obtains the value of the float difference of SBR*/
	diff = (sBR - (uint32)sBR);

	/*Loop that verifies with the BRFD values*/
	for(counter = 0; counter < 32; counter++){

		/*Verifies that the difference is between some value of BRFA*/
		if((diff > BRFD_Type[counter-1]) && (diff < BRFD_Type[counter+1])){
			/*Verifies that the BRFD is the value with smaller difference*/
			if(BRFD > BRFD_Type[counter]){
				/*Saves the values of BRFD and BRFA*/
				BRFD = BRFD_Type[counter];
				BRFA = counter;
			}
		}
		/*Special case with the initial value of BRFA*/
		if(counter == 0){
			/*Verifies that the difference is between some value of BRFA*/
			if((diff >= BRFD_Type[counter]) && (diff < BRFD_Type[counter+1])){
				/*Verifies that the BRFD is the value with smaller difference*/
				if(BRFD > BRFD_Type[counter]){
					/*Saves the values of BRFD and BRFA*/
					BRFD = BRFD_Type[counter];
					BRFA = counter;
				}
			}
		}
		/*Special case with the final value of BRFA*/
		if(counter == 31){
			/*Verifies that the difference is between some value of BRFA*/
			if((diff <= BRFD_Type[counter]) && (diff > BRFD_Type[counter-1])){
				/*Verifies that the BRFD is the value with smaller difference*/
				if(BRFD > BRFD_Type[counter]){
					/*Saves the values of BRFD and BRFA*/
					BRFD = BRFD_Type[counter];
					BRFA = counter;
				}
			}
		}
	}

	/*Considers only the integer part*/
	sbr_TEMP = (uint32)sBR;
	/*Saves the High part of Baud Rate*/
	sbr_HIGH = sbr_TEMP & UART_SBR_MASK_HIGH;
	/*Saves the Low part of Baud Rate*/
	sbr_LOW = sbr_TEMP & UART_SBR_MASK_LOW;

	/*Enable the clock of UART 1 */
	SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;
	/*Disable the Rx of UART*/
	UART1->C2 &= ~(UART_C2_RE_MASK);
	/*Disable the Tx of UART*/
	UART1->C2 &= ~(UART_C2_TE_MASK);
	/*Default settings*/
	UART1->C1 = 0;


	/*Clear the High part of SBR*/
	UART1->BDH &= ~(UART_CLEAR_BDH);
	/*Send the High part of SBR*/
	UART1->BDH |= sbr_HIGH;
	/*Clear the Low part of SBR*/
	UART1->BDL &= ~(UART_CLEAR_BDL);
	/*Send the Low part of SBR*/
	UART1->BDL |= sbr_LOW;


	/*Send the Baud Rate Fine Adjust to register*/
	UART1->C4 |= BRFA;
	/*Enable Rx of UART*/
	UART1->C2 |= (UART_C2_RE_MASK);
	/*Enable Tx of UART*/
	UART1->C2 |= (UART_C2_TE_MASK);
}

void UART0_interruptEnable(UART_ChannelType uartChannel){
	/*Verifies if the data is complete*/
	if(!(UART0->S1 & UART_S1_RDRF_MASK)){
		/*Enable the interrupter of reception*/
		UART0->C2 |= UART_C2_RIE_MASK;
	}
}

void UART1_interruptEnable(UART_ChannelType uartChannel){
	/*Verifies if the data is complete*/
	if(!(UART1->S1 & UART_S1_RDRF_MASK)){
		/*Enable the interrupter of reception*/
		UART1->C2 |= UART_C2_RIE_MASK;
	}
}

void UART_putChar (UART_ChannelType uartChannel, uint8 character){

	switch(uartChannel){
	case UART_0:
		/*Check if there isn't data transmission*/
		while(!(UART0->S1 & UART_S1_TDRE_MASK));
		/*Send character to Data Register*/
		UART0->D = character;
		delay(S);
		break;
	case UART_1:
		/*Check if there isn't data transmission*/
		while(!(UART1->S1 & UART_S1_TDRE_MASK));
		/*Send character to Data Register*/
		UART1->D = character;
		delay(S);
		break;
	default:
		break;
	}
}

void UART_putString(UART_ChannelType uartChannel, sint8* string){
	/*Counter that verifies each position of the array*/
	uint8 counter = 0;
	switch(uartChannel){
	case UART_0:
		/*Check if there isn't data transmission*/
		while(!(UART0->S1 & UART_S1_TDRE_MASK));
		/*Transmit the data until find the NULL value*/
		while(string[counter] != '\0'){
			/*Each character of string is send to Data Register*/
			UART0->D = string[counter];
			/*Move to next position in the array*/
			counter++;
			delay(S);
		}
		break;
	case UART_1:
		/*Check if there isn't data transmission*/
		while(!(UART1->S1 & UART_S1_TDRE_MASK));
		/*Transmit the data until find the NULL value*/
		while(string[counter] != '\0'){
			/*Each character of string is send to Data Register*/
			UART1->D = string[counter];
			/*Move to next position in the array*/
			counter++;
			delay(S);
		}
		break;
	default:
		break;
	}
}

uint8 clearUART0_mailbox(){
	/*Clean the MailBox of UART0*/
	UART0_MailBox.mailBox = 0;
	return TRUE;
}

uint8 clearUART1_mailbox(){
	/*Clean the mailbox of UART1*/
	UART1_MailBox.mailBox = 0;
	return TRUE;
}

uint32 expBASE10(uint8 limit){
	/*Use the exponent base 10 to different digits*/
	static uint32 value;

	if(limit == 0){	value = 1;}
	if(limit == 1){	value = 10;}
	if(limit > 1){	value *= 10;}

	return value;
}

uint32 Convert_numberASCIItoDATA(uint8 *string){

	const uint32 adjustASCII = 48;
	const uint32 CR = 13;
	uint32 counter1 = 0;
	uint32 counter2 = 0;
	uint32 counter3 = 0;
	uint32 data;
	uint32 tmpData1 = 0;
	uint32 tmpData2 = 0;
	uint32 expValue;

	/*First loop to manage find the size*/
	while(string[counter1] != CR){
		counter1++;
	}

	/*Second loop to begin in last position of array*/
	for(counter2 = counter1; counter2 != 0; counter2--){
		/*Elevate the number by 10 depending of its position*/
		expValue = expBASE10(counter3);

		/*Converts the ASCII to INT*/
		if(counter3 == 0){
			tmpData2 = string[counter2 - 1];
			tmpData2 -= adjustASCII;
			tmpData1 += tmpData2;
		}
		/*Save the value and sums*/
		if(counter3 > 0){
			tmpData2 = string[counter2 - 1];
			tmpData2 -= adjustASCII;
			tmpData2 *= expValue;
			tmpData1 += tmpData2;
		}
		counter3++;
	}

	data = tmpData1;
	return (data);
}

uint8 Convert_wordASCIItoDATA(uint8 word){
	uint8 valueWord = (uint8)word;
	return (valueWord);
}

FIFO_Type popFIFO_0(void){

	uint32 counterSize = 0;
	uint32 position = 0;
	uint32 counterClear;
	static uint32 counterChar;
	FIFO_Type fifo;

	/*Loop to find the size*/
	while(FIFO_UART0.data[counterSize] != '\0'){
		counterSize++;
	}

	/*Second loop to clear the FIFO*/
	for(counterClear = 0; counterClear < 50; counterClear++){
		fifo.data[counterClear] = '\0';
	}
	/*Third loop to save the value into FIFO*/
	for(counterChar = counterSize; counterChar != 0; counterChar--){
		fifo.data[position] = FIFO_UART0.data[position];
		position++;
	}

	fifo.size = counterSize;
	fifo.stateFIFO = NORMAL;
	return (fifo);
}

FIFO_Type popFIFO_1(void){

	uint32 counterSize = 0;
	uint32 position = 0;
	uint32 counterClear;
	static uint32 counterChar;
	FIFO_Type fifo;

	/*Loop to find the size*/
	while(FIFO_UART1.data[counterSize] != '\0'){
		counterSize++;
	}
	/*Second loop to clear the FIFO*/
	for(counterClear = 0; counterClear < 50; counterClear++){
		fifo.data[counterClear] = '\0';
	}
	/*Third loop to save the data into FIFO*/
	for(counterChar = counterSize; counterChar != 0; counterChar--){
		fifo.data[position] = FIFO_UART1.data[position];
		position++;

	}

	fifo.size = counterSize;
	fifo.stateFIFO = NORMAL;
	return (fifo);
}

FIFO_FlagType pushFIFO_0(uint8 character){

	static uint32 counterChar = 0;
	const uint32 CR = 13;

	/*Get the info into FIFO until find the CR value*/
	if(character != CR){
		FIFO_UART0.data[counterChar] = character;
		counterChar++;
		FIFO_UART0.stateFIFO = NORMAL;
		FIFO_UART0.size = 0;
	}else{
		FIFO_UART0.data[counterChar] = character;
		FIFO_UART0.size = counterChar;
		counterChar = 0;
		FIFO_UART0.stateFIFO = NORMAL;
		if(FIFO_UART0.size >= 50){
			FIFO_UART0.stateFIFO = FULL;
		}
	}
	return (FIFO_UART0.stateFIFO);

}

FIFO_FlagType pushFIFO_1(uint8 character){

	static uint32 counterChar = 0;
	const uint32 CR = 13;

	/*Get the info into FIFO until find the CR value*/
	if(character != CR){
		FIFO_UART1.data[counterChar] = character;
		counterChar++;
		FIFO_UART1.stateFIFO = NORMAL;
	}else{
		FIFO_UART1.data[counterChar] = character;
		FIFO_UART1.size = counterChar;
		counterChar = 0;
		FIFO_UART1.stateFIFO = NORMAL;
		if(FIFO_UART1.size >= 50){
			FIFO_UART1.stateFIFO = FULL;
		}
	}
	return (FIFO_UART1.stateFIFO);
}

FIFO_FlagType clearFIFO_0(void){

	uint32 counter;

	/*Clear the FIFO*/
	for(counter = 0; counter < 50; counter++){
		FIFO_UART0.data[counter] = '\0';
	}
	FIFO_UART0.size = 0;
	FIFO_UART0.stateFIFO = EMPTY;

	return (FIFO_UART0.stateFIFO);
}

FIFO_FlagType clearFIFO_1(void){

	uint32 counter;

	/*Clear the FIFO*/
	for(counter = 0; counter < 50; counter++){
		FIFO_UART1.data[counter] = '\0';
	}
	FIFO_UART1.size = 0;
	FIFO_UART1.stateFIFO = EMPTY;

	return (FIFO_UART1.stateFIFO);
}

