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
/* PLL predivider value */
#define PLL0_PRDIV 25
/* PLL multiplier value*/
#define PLL0_VDIV 30

/** Macros for debugging*/
#define DEBUG
#define PLL_DIRECT_INIT
/*Factor to convert the velocity in float*/
#define CENT			(100U)
/*The modulo of output FTM*/
#define MODULO			(0xFFU)
/*The modulo of input FTM*/
#define MODULO_CAPTURE	(0xFFFFU)
/*Init mode to output FTM*/
#define INIT_MOD		(0.80F)

#define	K	(500000)

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

	/***Interruptions Configurations***/
	/**Set the reference priority **/
	NVIC_setBASEPRI_threshold(PRIORITY_10);

	/**Set the priority**/
	NVIC_enableInterruptAndPriotity(PORTC_IRQ, PRIORITY_5);
	NVIC_enableInterruptAndPriotity(UART0_IRQ, PRIORITY_9);
	NVIC_enableInterruptAndPriotity(UART1_IRQ, PRIORITY_9);

	/**Configures UART 0 to transmit/receive at 115200 bauds with a 60 MHz of clock core*/
	UART0_init(UART_0, 60000000, BD_115200);
	/**Configures UART 1 to transmit/receive at 9600 bauds in Bluetooth with a 60 MHz of clock core*/
	UART1_init(UART_1, 60000000, BD_9600);
	/**Enables the UART interrupts*/
	UART0_interruptEnable(UART_0);
	UART1_interruptEnable(UART_1);

	/**Enable all the interrupts **/
	EnableInterrupts;

    while(1){
    	/**Machine states based on tags**/
    	//mainFunctions = StateProgram[currentState].stateFunction;
    	//currentState = mainFunctions();
    	//runSnake();


    	LCDNokia_bitmap(getFrame1());
    	delay(K);

    	LCDNokia_bitmap(getFrame2());
    	delay(K);

    	LCDNokia_bitmap(getFrame3());
    	delay(K);

    	LCDNokia_bitmap(getFrame4());
    	delay(K);

    	LCDNokia_bitmap(getFrame5());
    	delay(K);

    	LCDNokia_bitmap(getFrame6());
    	delay(K);

    	LCDNokia_bitmap(getFrame7());
    	delay(K);

    	LCDNokia_bitmap(getFrame8());
    	delay(K);

/*
    	LCDNokia_bitmap(getInfinite1());
    	delay(K);

    	LCDNokia_bitmap(getInfinite2());
    	delay(K);

    	LCDNokia_bitmap(getInfinite3());
    	delay(K);

    	LCDNokia_bitmap(getInfinite4());
    	delay(K);

    	LCDNokia_bitmap(getInfinite5());
    	delay(K);

    	LCDNokia_bitmap(getInfinite6());
    	delay(K);

    	LCDNokia_bitmap(getInfinite7());
    	delay(K);

    	LCDNokia_bitmap(getInfinite8());
    	delay(K);

    	LCDNokia_bitmap(getInfinite9());
    	delay(K);

    	LCDNokia_bitmap(getInfinite10());
    	delay(K);

    	LCDNokia_bitmap(getInfinite11());
    	delay(K);

    	LCDNokia_bitmap(getInfinite12());
    	delay(K);

    	LCDNokia_bitmap(getInfinite13());
    	delay(K);

    	LCDNokia_bitmap(getInfinite14());
    	delay(K);

    	LCDNokia_bitmap(getInfinite15());
    	delay(K);

    	LCDNokia_bitmap(getInfinite16());
    	delay(K);

    	LCDNokia_bitmap(getInfinite17());
    	delay(K);

    	LCDNokia_bitmap(getInfinite18());
    	delay(K);

    	LCDNokia_bitmap(getInfinite19());
    	delay(K);

    	LCDNokia_bitmap(getInfinite20());
    	delay(K);

    	LCDNokia_bitmap(getInfinite21());
    	delay(K);

    	LCDNokia_bitmap(getInfinite22());
    	delay(K);

    	LCDNokia_bitmap(getInfinite23());
    	delay(K);

    	LCDNokia_bitmap(getInfinite24());
    	delay(K);

    	LCDNokia_bitmap(getInfinite25());
    	delay(K);

    	LCDNokia_bitmap(getInfinite26());
    	delay(K);

    	LCDNokia_bitmap(getInfinite27());
    	delay(K);

    	LCDNokia_bitmap(getInfinite28());
    	delay(K);

    	LCDNokia_bitmap(getInfinite29());
    	delay(K);

    	LCDNokia_bitmap(getInfinite30());
    	delay(K);

    	LCDNokia_bitmap(getInfinite31());
    	delay(K);

    	LCDNokia_bitmap(getInfinite32());
    	delay(K);

    	LCDNokia_bitmap(getInfinite33());
    	delay(K);

    	LCDNokia_bitmap(getInfinite34());
    	delay(K);

    	LCDNokia_bitmap(getInfinite35());
    	delay(K);

    	LCDNokia_bitmap(getInfinite36());
    	delay(K);

    	LCDNokia_bitmap(getInfinite37());
    	delay(K);

    	LCDNokia_bitmap(getInfinite38());
    	delay(K);

    	LCDNokia_bitmap(getInfinite39());
    	delay(K);

    	LCDNokia_bitmap(getInfinite40());
    	delay(K);

    	LCDNokia_bitmap(getInfinite41());
    	delay(K);

    	LCDNokia_bitmap(getInfinite42());
    	delay(K);

    	LCDNokia_bitmap(getInfinite43());
    	delay(K);

    	LCDNokia_bitmap(getInfinite44());
    	delay(K);

    	LCDNokia_bitmap(getInfinite45());
    	delay(K);

    	LCDNokia_bitmap(getInfinite46());
    	delay(K);

    	LCDNokia_bitmap(getInfinite47());
    	delay(K);

    	LCDNokia_bitmap(getInfinite48());
    	delay(K);

    	LCDNokia_bitmap(getInfinite49());
    	delay(K);
*/
    }
    return 0;
}
