/*
 * Buttons.c
 *
 *  Created on: Oct 14, 2017
 *      Author: Andres Hernandez
 */

#include "Buttons.h"

/*Flag of buttons*/
static Button_PressedFlag Button_Pressed;

static void Button_clock(Button_PortType port){

	/*Selector of Port*/
	switch(port){
	case PORT_A:
		/**Enable the clock of port A**/
		GPIO_clockGating(GPIO_A);
		break;
	case PORT_B:
		/**Enable the clock of port B**/
		GPIO_clockGating(GPIO_B);
		break;
	case PORT_C:
		/**Enable the clock of port C**/
		GPIO_clockGating(GPIO_C);
		break;
	case PORT_D:
		/**Enable the clock of port D**/
		GPIO_clockGating(GPIO_D);
		break;
	case PORT_E:
		/**Enable the clock of port E**/
		GPIO_clockGating(GPIO_E);
		break;
	default:
		break;
	}
}

static void Button_enable(Button_PortType port, BitsType bit){

	/*The configuration of buttons is Pull Up and Rising edge interrupter*/
	static GPIO_pinControlRegisterType pinControlRegisterPort = GPIO_MUX1|GPIO_PE|GPIO_PS|INTR_RISING_EDGE;

	/*Selector of port*/
	switch(port){
	case PORT_A:
		/**Configuration the button like output**/
		GPIO_pinControlRegister(GPIO_A,bit,&pinControlRegisterPort);
		GPIO_dataDirectionPIN(GPIO_A,GPIO_INPUT,bit);
		break;
	case PORT_B:
		/**Configuration the button like output**/
		GPIO_pinControlRegister(GPIO_B,bit,&pinControlRegisterPort);
		GPIO_dataDirectionPIN(GPIO_B,GPIO_INPUT,bit);
		break;
	case PORT_C:
		/**Configuration the button like output**/
		GPIO_pinControlRegister(GPIO_C,bit,&pinControlRegisterPort);
		GPIO_dataDirectionPIN(GPIO_C,GPIO_INPUT,bit);
		break;
	case PORT_D:
		/**Configuration the button like output**/
		GPIO_pinControlRegister(GPIO_D,bit,&pinControlRegisterPort);
		GPIO_dataDirectionPIN(GPIO_D,GPIO_INPUT,bit);
		break;
	case PORT_E:
		/**Configuration the button like output**/
		GPIO_pinControlRegister(GPIO_E,bit,&pinControlRegisterPort);
		GPIO_dataDirectionPIN(GPIO_E,GPIO_INPUT,bit);
		break;
	default:
		break;
	}
}

void Button_statusFlag(GPIO_portNameType gpio, BitsType bit){

	/*Selector of Port*/
	switch(gpio){
	case GPIO_A:
		if(BIT1 == bit){
			/**Flag Button4 is set**/
			Button_Pressed.flagButton4 = TRUE;
		}
		if(BIT2 == bit){
			/**Flag Button2 is set**/
			Button_Pressed.flagButton2 = TRUE;
		}
		break;
	case GPIO_B:
		if(BIT9 == bit){
			/**Flag Button5 is set**/
			Button_Pressed.flagButton5 = TRUE;
		}
		if(BIT23 == bit){
			/**Flag Button3 is set**/
			Button_Pressed.flagButton3 = TRUE;
		}
		break;
	case GPIO_C:
		if(BIT2 == bit){
			/**Flag Button1 is set**/
			Button_Pressed.flagButton1 = TRUE;
		}
		if(BIT3 == bit){
			/**Flag Button0 is set**/
			Button_Pressed.flagButton0 = TRUE;
		}
		break;
	case GPIO_D:
		break;
	case GPIO_E:
		break;
	default:
		break;
	}
}

uint8 Button_getFlag(Button_Type button){

	/**Selector of buttons**/
	switch(button){
	case BUTTON_0:
		/**Return the status flag Button 0**/
		return (Button_Pressed.flagButton0);
		break;
	case BUTTON_1:
		/**Return the status flag Button 1**/
		return (Button_Pressed.flagButton1);
		break;
	case BUTTON_2:
		/**Return the status flag Button 2**/
		return (Button_Pressed.flagButton2);
		break;
	case BUTTON_3:
		/**Return the status flag Button 3**/
		return (Button_Pressed.flagButton3);
		break;
	case BUTTON_4:
		/**Return the status flag Button 4**/
		return (Button_Pressed.flagButton4);
		break;
	case BUTTON_5:
		/**Return the status flag Button 5**/
		return (Button_Pressed.flagButton5);
		break;
	default:
		return FALSE;
		break;
	}
}

void Button_clearFlag(Button_Type button){

	/*Selector of buttons*/
	switch(button){
	case BUTTON_0:
		/**Clear the flag of button 0**/
		Button_Pressed.flagButton0 = FALSE;
		break;
	case BUTTON_1:
		/**Clear the flag of button 1**/
		Button_Pressed.flagButton1 = FALSE;
		break;
	case BUTTON_2:
		/**Clear the flag of button 2**/
		Button_Pressed.flagButton2 = FALSE;
		break;
	case BUTTON_3:
		/**Clear the flag of button 3**/
		Button_Pressed.flagButton3 = FALSE;
		break;
	case BUTTON_4:
		/**Clear the flag of button 4**/
		Button_Pressed.flagButton4 = FALSE;
		break;
	case BUTTON_5:
		/**Clear the flag of button 5**/
		Button_Pressed.flagButton5 = FALSE;
		break;
	default:
		break;
	}
}

void Buttons_init(const Button_ConfigType* Button_Config){

	/**Number of buttons**/
	uint8 counter;

	/**Loop that set up each button**/
	for(counter = 0; counter < 6; counter++){
		/**Set the clock of button**/
		Button_clock(Button_Config[counter].Button_Port);
		/**Set the button like output**/
		Button_enable(Button_Config[counter].Button_Port, Button_Config[counter].Bit_Type);
	}
}

void Buzzer_setAlarm(uint32 current_Value, uint32 limit_Conf){

	/**Conditional that verifies if the current value of ADC reached the limit**/
	if(current_Value >= limit_Conf){
		/**Set the buzzer**/
		GPIO_setPIN(GPIO_C,BIT4);
	}else{/**In case if the current value of ADC doesn't have to be reached the limit**/
		/**Silence the buzzer**/
		GPIO_clearPIN(GPIO_C,BIT4);
	}
}

void Buzzer_init(){
	/**Set the configuration of buzzer like GPIO**/
	static GPIO_pinControlRegisterType pinControlRegisterPortA = GPIO_MUX1;
	/**Enable the clock of PIN**/
	GPIO_clockGating(GPIO_C);
	/**Set up the PIN C4**/
	GPIO_pinControlRegister(GPIO_C,BIT4,&pinControlRegisterPortA);
	/**Set the pin C4 like output**/
	GPIO_dataDirectionPIN(GPIO_C,GPIO_OUTPUT,BIT4);
	/**Write the PIN**/
	GPIO_writePORT(GPIO_C,BIT4);
}

float Conversion_Fahrenheit(uint32 celsius_Temp){
	/**This variable saves the converted value in fahrenheit**/
	float fahrenheit_Temp;

	/**Calculate the fahrenheit like **/
	fahrenheit_Temp = ((float)(celsius_Temp*9)/5) + 32;
	return (fahrenheit_Temp);
}

uint32 Control_Velocity(uint32 temperature, uint8 increment, uint8 modeManual, uint32 motor){

	/**This constant is the limit of motor velocity**/
	const uint32 limit_Vel = 85;
	/**Initial temperature**/
	static uint32 default_Temp = 20;
	/**Initial motor velocity**/
	static uint32 default_Vel = 80;
	/**Variable that saves the difference of temperature**/
	uint32 diff_Temp;
	/**The current velocity respect to temperature**/
	sint32 velocity;
	/**Factor of increment of velocity**/
	uint32 constant_Increment;
	/**Modulo of temperature if the temperature is odd**/
	uint32 mod_Temp;

	/**Calculate the module of current temperature**/
	mod_Temp =  temperature % 2;
	/**Set like default the current motor velocity**/
	default_Vel = motor;

	/**If the temperature oscillate above temperature + 1 and the configuration isn't manual**/
	if((temperature > (default_Temp + 1)) && (FALSE == modeManual)){
		/**If the temperature is odd, the temperature is rested 1 to practicality of measure**/
		if(mod_Temp == 1){temperature -= 1;}
		/**If the temperature is less than default temperature then the operators change**/
		/**In order to avoid the negative numbers**/
		if(temperature < default_Temp){
			diff_Temp = default_Temp - temperature;
		}else{/**The operators change of priority**/
			diff_Temp = temperature - default_Temp;
		}
		/**The factor of increment of velocity each 2 grades Celsius**/
		constant_Increment = diff_Temp / 2;
		/**The resultant velocity is the difference between the default velocity minus the increment**/
		velocity = (default_Vel)-(constant_Increment*increment);

		/**Safe section to avoid the overflow**/
		if(velocity <= 0){
			/**In case of velocity is minus that zero then assign zero like velocity**/
			velocity = 0;
		}
	}
	/**If the temperature oscillate below temperature - 1 and the configuration isn't manual**/
	if((temperature < (default_Temp - 1)) && (FALSE == modeManual)){
		/**If the temperature is odd, the temperature is reset 1 to practicality of measure**/
		if(mod_Temp == 1){temperature -= 1;}
		/**If the temperature is less than default temperature then the operators change**/
		/**In order to avoid the negative numbers**/
		if(temperature < default_Temp){
			diff_Temp = default_Temp - temperature;
		}else{/**The operators change of priority**/
			diff_Temp = temperature - default_Temp;
		}
		/**The factor of increment of velocity each 2 grades Celsius**/
		constant_Increment = diff_Temp / 2;
		/**The resultant velocity is the difference between the default velocity plus the increment**/
		velocity = (default_Vel)+(constant_Increment*increment);

		/**Safe section to avoid the overflow**/
		if(velocity >= limit_Vel){
			/**In case of overflow then the velocity is set like limi_Vel constant**/
			velocity = limit_Vel;
		}
	}
	/**If the temperature oscillate between the values +1 or -1, the velocity doesn't change**/
	if((temperature >= (default_Temp - 1)) && (temperature <= (default_Temp + 1))){
		/**Verifies if there isn't mode manual activated**/
		if(FALSE == modeManual){
			/**The velocity is maintained**/
			velocity = default_Vel;
			/**The temperature is maintained**/
			temperature = default_Temp;
		}
	}

	/**Update the values of temperature previously calculated**/
	default_Temp = temperature;
	/**Update the values of velocity previously calculated**/
	default_Vel = velocity;

	/**Return the resultant velocity**/
	return (velocity);
}

uint32 Control_ManualVelocity(uint32 velocity, uint8 increment, uint8 modeIncrement){

	/**Constant with maxim limit of velocity**/
	const sint32 limit_Max = 85;
	/**Constant with minim limit of velocity**/
	const sint32 limit_Min = 0;
	/**Default velocity that saves the current velocity**/
	static sint32 default_Velocity;
	/**Save the velocity input**/
	default_Velocity = velocity;

	/**If the decrement is activated then decrements respect to the increment previously calculated**/
	if(FALSE == modeIncrement){
		default_Velocity -= increment;
	}
	/**If the increment is activated then increments respect to the increment previously calculated**/
	if(TRUE == modeIncrement){
		default_Velocity += increment;
	}

	/**Safe conditionals that assigns the limits in case of overflow**/
	if(default_Velocity < limit_Min){default_Velocity = limit_Min;}
	if(default_Velocity > limit_Max){default_Velocity = limit_Max;}

	return (default_Velocity);
}


uint32 Calculate_Frequency(uint16 value){

	/**Constants of frequency in FTM counter**/
	const sint16 Range10K = 2100;
	const sint16 Range15K = 1400;
	const sint16 Range20K = 1024;
	const sint16 Range25K = 840;
	const sint16 Range30K = 700;
	const sint16 Range35K = 600;
	const sint16 Range40K = 525;
	const sint16 Range45K = 466;
	const sint16 Range50K = 420;
	const sint16 Range55K = 381;
	const sint16 Range60K = 350;
	const sint16 Range65K = 323;
	const sint16 Range70K = 300;
	const sint16 Range75K = 280;
	const sint16 Range80K = 262;
	const sint16 Range85K = 247;
	const sint16 Range90K = 233;
	const sint16 Range95K = 221;
	const sint16 Range100K = 210;

	/**The frequency initial is zero**/
	static uint32 frequency = 0;

	/**The initial conditional set the scale to 10Khz to 100Khz**/
	if((value > 200)&&(value < 2150)){
		/**In 10KHz verifies with a radio of 250 counters**/
		if((value > Range10K - 250) && (value < Range10K + 250)){
			/**Assign the frequency in 10KHz**/
			frequency = 10000;
		}
		/**In 15KHz verifies with a radio of 200 counters**/
		if((value > Range15K - 200) && (value < Range15K + 200)){
			/**Assign the frequency in 15KHz**/
			frequency = 15000;
		}
		/**In 20KHz verifies with a radio of 200 counters**/
		if((value > Range20K - 200) && (value < Range20K + 200)){
			/**Assign the frequency in 20KHz**/
			frequency = 20000;
		}
		/**In 25KHz verifies with a radio of 200 counters**/
		if((value > Range25K - 200) && (value < Range25K + 200)){
			/**Assign the frequency in 25KHz**/
			frequency = 25000;
		}
		/**In 30Hz verifies with a radio of 150 counters**/
		if((value > Range30K - 150) && (value < Range30K + 150)){
			/**Assign the frequency in 30KHz**/
			frequency = 30000;
		}
		/**In 35KHz verifies with a radio of 100 counters**/
		if((value > Range35K - 100) && (value < Range35K + 100)){
			/**Assign the frequency in 35KHz**/
			frequency = 35000;
		}
		/**In 40KHz verifies with a radio of 100 counters**/
		if((value > Range40K - 100) && (value < Range40K + 100)){
			/**Assign the frequency in 40KHz**/
			frequency = 40000;
		}
		/**In 45KHz verifies with a radio of 50 counters**/
		if((value > Range45K - 50) && (value < Range45K + 50)){
			/**Assign the frequency in 45KHz**/
			frequency = 45000;
		}
		/**In 50KHz verifies with a radio of 50 counters**/
		if((value > Range50K - 50) && (value < Range50K + 50)){
			/**Assign the frequency in 50KHz**/
			frequency = 50000;
		}
		/**In 55KHz verifies with a radio of 25 counters**/
		if((value > Range55K - 25) && (value < Range55K + 25)){
			/**Assign the frequency in 55KHz**/
			frequency = 55000;
		}
		/**In 60KHz verifies with a radio of 25 counters**/
		if((value > Range60K - 25) && (value < Range60K + 25)){
			/**Assign the frequency in 60KHz**/
			frequency = 60000;
		}
		/**In 65KHz verifies with a radio of 25 counters**/
		if((value > Range65K - 25) && (value < Range65K + 25)){
			/**Assign the frequency in 65KHz**/
			frequency = 65000;
		}
		/**In 70KHz verifies with a radio of 25 counters**/
		if((value > Range70K - 25) && (value < Range70K + 25)){
			/**Assign the frequency in 70KHz**/
			frequency = 70000;
		}
		/**In 75KHz verifies with a radio of 20 counters**/
		if((value > Range75K - 20) && (value < Range75K + 20)){
			/**Assign the frequency in 75KHz**/
			frequency = 75000;
		}
		/**In 80KHz verifies with a radio of 20 counters**/
		if((value > Range80K - 20) && (value < Range80K + 20)){
			/**Assign the frequency in 80KHz**/
			frequency = 80000;
		}
		/**In 85KHz verifies with a radio of 20 counters**/
		if((value > Range85K - 20) && (value < Range85K - 20)){
			/**Assign the frequency in 85KHz**/
			frequency = 85000;
		}
		/**In 90KHz verifies with a radio of 20 counters**/
		if((value > Range90K - 20) && (value < Range90K + 20)){
			/**Assign the frequency in 90KHz**/
			frequency = 90000;
		}
		/**In 95KHz verifies with a radio of 15 counters**/
		if((value > Range95K - 15) && (value < Range95K + 15)){
			/**Assign the frequency in 95KHz**/
			frequency = 95000;
		}
		/**In 100KHz verifies with a radio of 15 counters**/
		if((value > Range100K - 15) && (value < Range100K + 15)){
			/**Assign the frequency in 100KHz**/
			frequency = 100000;
		}
	}else{/**If not there is frequency detected then frequency is zero**/
		frequency = 0;
	}
	return frequency;
}

