/**
	\file
	\brief
		This is the source file for the GPIO device driver for Kinetis K64.
		It contains all the implementation for configuration functions and runtime functions.
		i.e., this is the application programming interface (API) for the GPIO peripheral.
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	7/09/2014
	\todo
	    Interrupts are not implemented in this API implementation.
 */
#include "MK64F12.h"
#include "GPIO.h"
#include "DataTypeDefinitions.h"
#include "Buttons.h"


static GPIO_interruptFlags_t GPIO_intrStatusFlag;


/**This function returns the real value of selecting PIN**/
uint32 valuePIN(uint8 bit){
	/**Value of 1 to  **/
	uint8 temp = 1;
	uint32 value;
	if(bit>32){
		return FALSE;
	}else{
		value = (uint32)(temp<<bit);
		return value;
	}
}

void PORTC_IRQHandler()
{
	GPIO_readInterrupt(GPIO_C);

	if((1<<BIT2) == GPIO_readInterrupt(GPIO_C)){
		GPIO_intrStatusFlag.flagPortC  = TRUE;
		Button_statusFlag(GPIO_C,BIT2);
	}
	if((1<<BIT5) == GPIO_readInterrupt(GPIO_C)){
		GPIO_intrStatusFlag.flagPortC  = TRUE;
		Button_statusFlag(GPIO_C,BIT5);
	}
	if((1<<BIT7) == GPIO_readInterrupt(GPIO_C)){
		GPIO_intrStatusFlag.flagPortC  = TRUE;
		Button_statusFlag(GPIO_C,BIT7);
	}
	if((1<<BIT0) == GPIO_readInterrupt(GPIO_C)){
		GPIO_intrStatusFlag.flagPortC  = TRUE;
		Button_statusFlag(GPIO_C,BIT0);
	}

	//GPIO_intrStatusFlag.flagPortC  = TRUE;
	GPIO_clearInterrupt(GPIO_C);
}

uint8 GPIO_getIRQStatus(GPIO_portNameType gpio)
{
	switch (gpio) {
		case GPIO_A:
			return(GPIO_intrStatusFlag.flagPortA);
			break;
		case GPIO_B:
			return(GPIO_intrStatusFlag.flagPortB);
			break;
		case GPIO_C:
			return(GPIO_intrStatusFlag.flagPortC);
			break;
		case GPIO_D:
			return(GPIO_intrStatusFlag.flagPortD);
			break;
		case GPIO_E:
			return(GPIO_intrStatusFlag.flagPortE);
			break;
		default:
			return(ERROR);
			break;
	}
}

uint8 GPIO_clearIRQStatus(GPIO_portNameType gpio)
{

	switch (gpio) {
		case GPIO_A:
			GPIO_intrStatusFlag.flagPortA = FALSE;
			break;
		case GPIO_B:
			GPIO_intrStatusFlag.flagPortB = FALSE;
			break;
		case GPIO_C:
			GPIO_intrStatusFlag.flagPortC = FALSE;
			break;
		case GPIO_D:
			GPIO_intrStatusFlag.flagPortD = FALSE;
			break;
		case GPIO_E:
			GPIO_intrStatusFlag.flagPortE = FALSE;
			break;
		default:
			return(ERROR);
			break;
	}
	return(TRUE);
}

uint32 GPIO_readInterrupt(GPIO_portNameType portName){
	uint32 intrValue = 0;

	switch(portName)/** Selecting the GPIO for clock enabling*/
	{
		case GPIO_A: /** GPIO A is selected*/
			intrValue |= PORTA->ISFR;
			break;
		case GPIO_B: /** GPIO B is selected*/
			intrValue |= PORTB->ISFR;
			break;
		case GPIO_C: /** GPIO C is selected*/
			intrValue |= PORTC->ISFR;
			break;
		case GPIO_D: /** GPIO D is selected*/
			intrValue |= PORTD->ISFR;
			break;
		default: /** GPIO E is selected*/
			intrValue |= PORTE->ISFR;
			break;

	}// end switch

	return intrValue;

}

void GPIO_clearInterrupt(GPIO_portNameType portName)
{
	switch(portName)/** Selecting the GPIO for clock enabling*/
	{
		case GPIO_A: /** GPIO A is selected*/
			PORTA->ISFR=0xFFFFFFFF;
			break;
		case GPIO_B: /** GPIO B is selected*/
			PORTB->ISFR=0xFFFFFFFF;
			break;
		case GPIO_C: /** GPIO C is selected*/
			PORTC->ISFR = 0xFFFFFFFF;
			break;
		case GPIO_D: /** GPIO D is selected*/
			PORTD->ISFR=0xFFFFFFFF;
			break;
		default: /** GPIO E is selected*/
			PORTE->ISFR=0xFFFFFFFF;
			break;

	}// end switch
}
uint8 GPIO_clockGating(GPIO_portNameType portName)
{
	switch(portName)/** Selecting the GPIO for clock enabling*/
			{
				case GPIO_A: /** GPIO A is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTA; /** Bit 9 of SIM_SCGC5 is  set*/
					break;
				case GPIO_B: /** GPIO B is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTB; /** Bit 10 of SIM_SCGC5 is set*/
					break;
				case GPIO_C: /** GPIO C is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTC; /** Bit 11 of SIM_SCGC5 is set*/
					break;
				case GPIO_D: /** GPIO D is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTD; /** Bit 12 of SIM_SCGC5 is set*/
					break;
				case GPIO_E: /** GPIO E is selected*/
					SIM->SCGC5 |= GPIO_CLOCK_GATING_PORTE; /** Bit 13 of SIM_SCGC5 is set*/
					break;
				default: /**If doesn't exist the option*/
					return(FALSE);
			}// end switch
	/**Successful configuration*/
	return(TRUE);
}// end function

uint8 GPIO_pinControlRegister(GPIO_portNameType portName,uint8 pin,const GPIO_pinControlRegisterType*  pinControlRegister)
{

	switch(portName)
		{
		case GPIO_A:/** GPIO A is selected*/
			PORTA->PCR[pin] = *pinControlRegister;
			break;
		case GPIO_B:/** GPIO B is selected*/
			PORTB->PCR[pin] = *pinControlRegister;
			break;
		case GPIO_C:/** GPIO C is selected*/
			PORTC->PCR[pin] = *pinControlRegister;
			break;
		case GPIO_D:/** GPIO D is selected*/
			PORTD->PCR[pin] = *pinControlRegister;
			break;
		case GPIO_E: /** GPIO E is selected*/
			PORTE->PCR[pin]= *pinControlRegister;
		default:/**If doesn't exist the option*/
			return(FALSE);
		break;
		}
	/**Successful configuration*/
	return(TRUE);
}

void GPIO_writePORT(GPIO_portNameType portName, uint8 Data){
	uint32 realData;

	realData = valuePIN(Data);
	switch(portName){
	case GPIO_A:
		GPIOA->PDOR|=realData;
		break;
	case GPIO_B:
		GPIOB->PDOR|=realData;
		break;
	case GPIO_C:
		GPIOC->PDOR|=realData;
		break;
	case GPIO_D:
		GPIOD->PDOR|=realData;
		break;
	case GPIO_E:
		GPIOE->PDOR|=realData;
		break;
	default:
		break;
	}
}

uint32 GPIO_readPORT(GPIO_portNameType portName){
	uint32 readPort = 0;

	switch(portName){
	case GPIO_A:
		readPort = GPIOA->PDIR;
		break;
	case GPIO_B:
		readPort = GPIOB->PDIR;
		break;
	case GPIO_C:
		readPort = GPIOC->PDIR;
		break;
	case GPIO_D:
		readPort = GPIOD->PDIR;
		break;
	case GPIO_E:
		readPort = GPIOE->PDIR;
		break;
	default:
		break;
	}
	return readPort;
}

uint32 GPIO_readPIN(GPIO_portNameType portName, uint8 pin){
	uint32 value = 0;
	uint32 realPin;

	realPin = valuePIN(pin);

	switch(portName){
	case GPIO_A:
		value = GPIOA->PDIR;
		value |= realPin;
		break;
	case GPIO_B:
		value = GPIOB->PDIR;
		value |= realPin;
		break;
	case GPIO_C:
		value = GPIOC->PDIR;
		value |= realPin;
		break;
	case GPIO_D:
		value = GPIOD->PDIR;
		value |= realPin;
		break;
	case GPIO_E:
		value = GPIOE->PDIR;
		value |= realPin;
		break;
	default:
		break;
	}
	return value;

}
void GPIO_setPIN(GPIO_portNameType portName, uint8 pin){
	uint32 realPin;
	realPin = valuePIN(pin);

	switch(portName){
	case GPIO_A:
		GPIOA->PSOR |= realPin;
		break;
	case GPIO_B:
		GPIOB->PSOR |= realPin;
		break;
	case GPIO_C:
		GPIOC->PSOR |= realPin;
		break;
	case GPIO_D:
		GPIOD->PSOR |= realPin;
		break;
	case GPIO_E:
		GPIOE->PSOR |= realPin;
		break;
	default:
		break;
	}
}
void GPIO_clearPIN(GPIO_portNameType portName, uint8 pin){
	uint32 realPin;
	realPin = valuePIN(pin);

	switch(portName){
	case GPIO_A:
		GPIOA->PCOR |= realPin;
		break;
	case GPIO_B:
		GPIOB->PCOR |= realPin;
		break;
	case GPIO_C:
		GPIOC->PCOR |= realPin;
		break;
	case GPIO_D:
		GPIOD->PCOR |= realPin;
		break;
	case GPIO_E:
		GPIOE->PCOR |= realPin;
		break;
	default:
		break;
	}
}
void GPIO_tooglePIN(GPIO_portNameType portName, uint8 pin){

	uint32 realPin;

	realPin = valuePIN(pin);

	switch(portName){
	case GPIO_A:
		GPIOA->PTOR |= realPin;
		break;
	case GPIO_B:
		GPIOB->PTOR |= realPin;
		break;
	case GPIO_C:
		GPIOC->PTOR |= realPin;
		break;
	case GPIO_D:
		GPIOD->PTOR |= realPin;
		break;
	case GPIO_E:
		GPIOE->PTOR |= realPin;
		break;
	default:
		break;
	}

}
void GPIO_dataDirectionPORT(GPIO_portNameType portName ,uint32 direction);
void GPIO_dataDirectionPIN(GPIO_portNameType portName, uint8 State, uint8 pin){
	uint32 realPin;
	realPin = valuePIN(pin);

	switch(State){
	case GPIO_INPUT:
		switch(portName){
		case GPIO_A:
			GPIOA->PDDR &= ~(realPin);
			break;
		case GPIO_B:
			GPIOB->PDDR &= ~(realPin);
			break;
		case GPIO_C:
			GPIOC->PDDR &= ~(realPin);
			break;
		case GPIO_D:
			GPIOD->PDDR &= ~(realPin);
			break;
		case GPIO_E:
			GPIOE->PDDR &= ~(realPin);
			break;
		default:
			break;
		}
		break;
	case GPIO_OUTPUT:
		switch(portName){
		case GPIO_A:
			GPIOA->PDDR|=realPin;
			break;
		case GPIO_B:
			GPIOB->PDDR|=realPin;
			break;
		case GPIO_C:
			GPIOC->PDDR|=realPin;
			break;
		case GPIO_D:
			GPIOD->PDDR|=realPin;
			break;
		case GPIO_E:
			GPIOE->PDDR|=realPin;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

