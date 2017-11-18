/*
 * Buttons.h
 *
 *  Created on: Oct 14, 2017
 *      Author: Andres Hernandez
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "MK64F12.h"
#include "DataTypeDefinitions.h"
#include "GPIO.h"

/*Macro to define the value of system Clock*/
#define SYSTEM_CLOCK	(21000000U)

/*Data type to assign the buttons*/
typedef enum{
	BUTTON_0,
	BUTTON_1,
	BUTTON_2,
	BUTTON_3,
	BUTTON_4,
	BUTTON_5
}Button_Type;

/*Data type to assign the flag of the buttons*/
typedef struct{
	uint8 flagButton0 : 1;
	uint8 flagButton1 : 1;
	uint8 flagButton2 : 1;
	uint8 flagButton3 : 1;
	uint8 flagButton4 : 1;
	uint8 flagButton5 : 1;
} Button_PressedFlag;

/*Data type to assign the port of buttons to use*/
typedef enum{PORT_A,PORT_B,PORT_C,PORT_D,PORT_E}Button_PortType;

/*Data type to set up the buttons*/
typedef struct{
	Button_PortType Button_Port;
	BitsType Bit_Type;
}Button_ConfigType;


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief This activate the clock of the button port
 	\param[in] port Selected port
 	\return void
 */
//static void Button_clock(Button_PortType port);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief This enable the button
 	\param[in] port Selected port
 	\param[in] bit	Selected bit
 	\return void
 */
//static void Button_enable(Button_PortType port,BitsType bit);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief This set the flag of Button
 	\param[in] port Selected port
 	\param[in] bit Selected bit
 	\return void
 */
void Button_statusFlag(GPIO_portNameType gpio, BitsType bit);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief This send the value of the flag of a button
 	\param[in] button Selected button
 	\return status of the button flag
 */
uint8 Button_getFlag(Button_Type button);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief This clear the flag of a button
 	\param[in] button Selected button
 	\return void
 */
void Button_clearFlag(Button_Type button);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief This set up of the buttons
 	\param[in] Button_Config Configurations of buttons
 	\return void
 */
void Buttons_init(const Button_ConfigType*);


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief This verifies the status of the buzzer
 	\param[in] currentValue	Value to compare
 	\param[in] limitConf	Limit to change the state
 	\return void
 */
void Buzzer_setAlarm(uint32 currentValue, uint32 limitConf);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief This initializes the configurations of Buzzer
 	\return void
 */
void Buzzer_init();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief This calculate the conversion of celsius to fahrenheit
 	\param[in] celsius_Temp
 	\return Value converted to fahrenheit
 */
float Conversion_Fahrenheit(uint32 celsius_Temp);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief This controls the motor velocity respect to temperature
 	\param[in] temperature	The ADC result
 	\param[in] increment	Set increment of motor
 	\param[in] modeManual	Mode to control the motor
 	\param[in] motor		Current value of motor
 	\return The velocity of motor
 */
uint32 Control_Velocity(uint32 temperature, uint8 increment, uint8 modeManual, uint32 motor);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief This controls manually the motor
 	\param[in] velocity	Current value of motor
 	\param[in] increment Set increment of motor
 	\param[in] modeIncrement Increment or decrement
 	\return The velocity of motor
 */
uint32 Control_ManualVelocity(uint32 velocity, uint8 increment, uint8 modeIncrement);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief This calculate the input frequency
 	\param[in] value Counter value
 	\return The frequency calculated
 */
uint32 Calculate_Frequency(uint16 value);


#endif /* BUTTONS_H_ */
