/*
 *
 *
 */

/***Libraries to the project***/
#include <stdio.h>
#include "MK64F12.h"
#include "GPIO.h"
#include "DatatypeDefinitions.h"
#include "LCDNokia5110.h"
#include "SPI.h"
#include "NVIC.h"
#include "MCG.h"
#include "UART.h"
#include "States.h"
#include "Frames.h"
#include "RNGA.h"
#include "Magnetometer.h"

/**Macros to Clock Frequency**/

/* CLKIN0 frequency */
#define CLK_FREQ_HZ 50000000
/*This is the approximate value for the slow irc*/
#define SLOW_IRC_FREQ 32768
/*This is the approximate value for the fast irc*/
#define FAST_IRC_FREQ 4000000
/*It defines an external clock*/
#define EXTERNAL_CLOCK 0
/**PLL is enabled*/
#define PLL_ENABLE 1
/**PLL is disabled*/
#define PLL_DISABLE 0
/*It defines an crystal oscillator*/
#define CRYSTAL_OSC 1
/* Set the oscillator for low power mode */
#define LOW_POWER 0
/* Set the slow IRC */
#define SLOW_IRC 0
/* Crystal or canned oscillator clock input */
#define CLK0_TYPE 0
/* PLL pre-divider value */
#define PLL0_PRDIV 25
/* PLL multiplier value*/
#define PLL0_VDIV 30

/** Macros for debugging*/
#define DEBUG
#define PLL_DIRECT_INIT

/**Settings of SPI**/
const SPI_ConfigType SPI_Config={
							SPI_DISABLE_FIFO,	/**Disable fifos of SPI**/
							SPI_LOW_POLARITY,	/**Low Polarity to SPI**/
							SPI_LOW_PHASE,		/**Low Phase to SPI**/
							SPI_MSB,			/**The MSB bit is the first to be transmitted**/
							SPI_0,				/**Channel SPI0**/
							SPI_MASTER,			/**Set like master the SPI**/
							GPIO_MUX2,			/**Set like MUX2 in GPIO**/
							SPI_BAUD_RATE_6,	/**Baudrate to SPI**/
							SPI_FSIZE_8,		/**Size of frame**/
							{GPIO_D,BIT1,BIT2}};/**GPIO for SPI**/

/**Simple machine state only change the tag**/
const StateType StateProgram[6] =
{
		{stateMainMenu},
		{stateContacts},
		{stateMessages},
		{stateSnakeGame},
		{stateCompass},
		{stateWallpaper}
};

int main(void){

	/**Configuration to enable the clock in 60 MHz**/
	int mcg_clk_hz;
	unsigned char modeMCG = 0;
	mcg_clk_hz = pll_init(CLK_FREQ_HZ, LOW_POWER, EXTERNAL_CLOCK, PLL0_PRDIV, PLL0_VDIV, PLL_ENABLE);

	/**First state in the program**/
  	States_MenuType currentState = MAIN_MENU;
	States_MenuType(*mainFunctions)(void);

	/**Configurations of devices**/
	SPI_init(&SPI_Config);
	LCDNokia_init();
	LCDNokia_clear();
	enableRNGA();
	I2C_init(I2C_0, 30000000, 100000);
	GPIOForMagnetometerInit();

	/***Interruptions Configurations***/
	/**Set the reference priority **/
	NVIC_setBASEPRI_threshold(PRIORITY_10);

	/**Set the priority**/
	NVIC_enableInterruptAndPriotity(PORTA_IRQ,PRIORITY_7);
	NVIC_enableInterruptAndPriotity(PORTC_IRQ, PRIORITY_7);
	NVIC_enableInterruptAndPriotity(PORTE_IRQ,PRIORITY_7);
	NVIC_enableInterruptAndPriotity(UART0_IRQ, PRIORITY_9);
	NVIC_enableInterruptAndPriotity(UART1_RX_TX_IRQn, PRIORITY_9);
	NVIC_enableInterruptAndPriotity(I2C0_IRQ,PRIORITY_9);
	NVIC_enableInterruptAndPriotity(PIT_CH0_IRQ,PRIORITY_9);

	/**Configures UART 0 to transmit/receive at 115200 bauds with a 60 MHz of clock core*/
	UART0_init(UART_0, 60000000, BD_115200);
	/**Configures UART 1 to transmit/receive at 115200 bauds in GSM module with a 60 MHz of clock core*/
	UART1_init(UART_1, 60000000, BD_115200);
	/**Enables the UART interrupts*/
	UART0_interruptEnable(UART_0);
	UART1_interruptEnable(UART_1);

	/**Enable all the interrupts **/
	EnableInterrupts;
	setSysConfig();

    while(1){
    	/**Machine states based on tags**/
    	mainFunctions = StateProgram[currentState].stateFunction;
    	currentState = mainFunctions();
    }
    return 0;
}
