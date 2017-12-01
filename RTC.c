#include "MK64F12.h"
#include "DataTypeDefinitions.h"
#include "GPIO.h"
#include "I2C.h"
#include "LCDNokia5110.h"
#include "NVIC.h"
#include "SPI.h"
#include "stdlib.h"
#include "RTC.h"
#define WRITECONTROL 0xA2
#define READCONTROL 0xA3
AMPM AMPMFlag;

RTCErrorCode Code;

void setRTCError(RTCErrorCode code){
	Code=code;
}
RTCErrorCode getRTCError(){
	return Code;
}

void setRTC_sec(uint8 sec){
	if(sec>60){
		sec=0; //Defaults the seconds counter to 0 when above 60 to avoid irregularities
	}
	uint8 BCDsec=(sec%10);

	BCDsec|=(sec/10)<<BIT4;

	I2C_start(); //Generate Start Signal
	I2C_write_Byte(WRITECONTROL); //"11011110" Write control code, CSS and choose write
	I2C_wait();
	I2C_get_ACK();
	RTCdelay(1000);

	I2C_write_Byte(0x02); //Write address to access	I2C_get_ACK();
	I2C_wait();
	I2C_get_ACK();
	RTCdelay(1000);

	I2C_write_Byte(BCDsec); //Write BCD code
	I2C_wait();
	I2C_get_ACK();
	RTCdelay(1000);
	I2C_stop(); //Stop transfer
	RTCdelay(1000);

}

void setRTC_min(uint8 min){
	if(min>60){
		min=0; //Defaults the seconds counter to 0 when above 60 to avoid irregularities
	}
	uint8 BCDmin=(min%10);

	BCDmin|=(min/10)<<BIT4;

	I2C_start(); //Generate Start Signal
	I2C_write_Byte(WRITECONTROL); //"11011110" Write control code, CSS and choose write
	I2C_wait();
	I2C_get_ACK();
	RTCdelay(1000);

	I2C_write_Byte(0x03); //Write address to access	I2C_get_ACK();
	I2C_wait();
	I2C_get_ACK();
	RTCdelay(1000);

	I2C_write_Byte(BCDmin); //Write BCD code
	I2C_wait();
	I2C_get_ACK();
	RTCdelay(1000);
	I2C_stop(); //Stop transfer
	RTCdelay(1000);

}

void setRTC_hour(uint8 hour){
	if(hour>23){
		hour=0; //Defaults the seconds counter to 0 when above 23 to avoid irregularities
	}
	uint8 BCDhour=(hour%10);

	BCDhour|=(hour/10)<<BIT4;

	I2C_start(); //Generate Start Signal
	I2C_write_Byte(WRITECONTROL); //"11011110" Write control code, CSS and choose write
	I2C_wait();
	I2C_get_ACK();
	RTCdelay(1000);

	I2C_write_Byte(0x04); //Write address to access	I2C_get_ACK();
	I2C_wait();
	I2C_get_ACK();
	RTCdelay(1000);

	I2C_write_Byte(BCDhour); //Write BCD code
	I2C_wait();
	I2C_get_ACK();
	RTCdelay(6500);
	I2C_stop(); //Stop transfer
	RTCdelay(1000);

}




void setRTC_day(uint8 day){
	if(day>31){
		day=0; //Defaults the seconds counter to 0 when above 23 to avoid irregularities
	}
	uint8 BCDday=(day%10);

	BCDday|=(day/10)<<BIT4;

	I2C_start(); //Generate Start Signal
	I2C_write_Byte(WRITECONTROL); //"11011110" Write control code, CSS and choose write
	I2C_wait();
	I2C_get_ACK();
	RTCdelay(1000);

	I2C_write_Byte(0x05); //Write address to access	I2C_get_ACK();
	I2C_wait();
	I2C_get_ACK();
	RTCdelay(1000);

	I2C_write_Byte(BCDday); //Write BCD code
	I2C_wait();
	I2C_get_ACK();
	RTCdelay(1000);
	I2C_stop(); //Stop transfer
	RTCdelay(1000);
}
void setRTC_month(uint8 month){
	if(month>12){
		month=12; //Defaults the seconds counter to 0 when above 23 to avoid irregularities
	}
	uint8 BCDmonth=(month%10);

	BCDmonth|=(month/10)<<BIT4;

	I2C_start(); //Generate Start Signal
	I2C_write_Byte(WRITECONTROL); //"11011110" Write control code, CSS and choose write
	I2C_wait();
	I2C_get_ACK();
	RTCdelay(1000);

	I2C_write_Byte(0x07); //Write address to access	I2C_get_ACK();
	I2C_wait();
	I2C_get_ACK();
	RTCdelay(1000);

	I2C_write_Byte(BCDmonth); //Write BCD code
	I2C_wait();
	I2C_get_ACK();
	RTCdelay(1000);
	I2C_stop(); //Stop transfer
	RTCdelay(1000);

}
void setRTC_year(uint16 year){
	//Reset to current year on excesss
	if(year>99){
		year=17;
	}

	uint8 BCDyeardec=(year%10);
	BCDyeardec|=(year/10)<<BIT4;
	RTCdelay(1000);

	I2C_start(); //Generate Start Signal
	I2C_write_Byte(WRITECONTROL); //"11011110" Write control code, CSS and choose write
	I2C_wait();
	I2C_get_ACK();
	RTCdelay(1000);

	I2C_write_Byte(0x08); //Write address to access	I2C_get_ACK();
	I2C_wait();
	I2C_get_ACK();
	RTCdelay(1000);

	I2C_write_Byte(BCDyeardec); //Write BCD code
	I2C_wait();
	I2C_get_ACK();
	RTCdelay(1000);
	I2C_stop(); //Stop transfer
	RTCdelay(1000);

}


/*
 *
 *\//////////////////////////////////INICIAN LECTURAS DE RTC//////////////////////////////////
 *Para las lecturas del RTC se uso el protocolo estandar del I2C el se reduce a
 *Start->CodigoDeControl->ConfirmacrAcknowledge->Direccion a leer->ConfirmarAcknowledge->Parar escritura e iniciar lectura, cambiar a receptor, lectura de prueba->parar y leer el dato verdadero, retornandolo
 */
uint8 readRTC_sec(){
	uint8 dummy=0;
	I2C_start();
	I2C_write_Byte(WRITECONTROL);
	I2C_wait();
	I2C_get_ACK(); //Waiting for acknowledge, this function is able to detect
	RTCdelay(1000);


	I2C_write_Byte(0x02);
	I2C_wait();
	I2C_get_ACK(); //Waiting for acknowledge, this function is able to detect
	RTCdelay(1000);


	I2C_repeated_Start();

	I2C_write_Byte(READCONTROL);
	I2C_wait();
	I2C_get_ACK(); //Waiting for acknowledge, this function is able to detect
	RTCdelay(1000);

	I2C_TX_RX_Mode(I2C_RX_MODE);// Changing I2C module to receiver mode

	I2C_NACK();
	dummy=I2C_read_Byte();
	I2C_wait();// Checking if the I2C module is busy
	RTCdelay(1000);

	I2C_stop();// Generating stop signal
	dummy=I2C_read_Byte();
	if(dummy>0x70){
		setRTCError(MEMORYERROR);
	}

	return dummy;


}
uint8 readRTC_min(){
	uint8 dummy=0;
	I2C_start();

	I2C_write_Byte(WRITECONTROL);
	I2C_wait();
	I2C_get_ACK(); //Waiting for acknowledge, this function is able to detect
	RTCdelay(1000);


	I2C_write_Byte(0x03);

	I2C_wait();
	I2C_get_ACK(); //Waiting for acknowledge, this function is able to detect
	RTCdelay(1000);


	I2C_repeated_Start();

	I2C_write_Byte(READCONTROL);
	I2C_wait();
	I2C_get_ACK(); //Waiting for acknowledge, this function is able to detect
	RTCdelay(1000);

	I2C_TX_RX_Mode(I2C_RX_MODE);// Changing I2C module to receiver mode

	I2C_NACK();
	dummy=I2C_read_Byte();
	I2C_wait();// Checking if the I2C module is busy
	RTCdelay(1000);

	I2C_stop();// Generating stop signal
	dummy=I2C_read_Byte();
	return dummy;


}
uint8 readRTC_hour(){
	uint8 dummy=0;
	I2C_start();

	I2C_write_Byte(WRITECONTROL);
	I2C_wait();
	I2C_get_ACK(); //Waiting for acknowledge, this function is able to detect
	RTCdelay(1000);


	I2C_write_Byte(0x04);
	I2C_wait();
	I2C_get_ACK(); //Waiting for acknowledge, this function is able to detect
	RTCdelay(1000);


	I2C_repeated_Start();

	I2C_write_Byte(READCONTROL);
	I2C_wait();
	I2C_get_ACK(); //Waiting for acknowledge, this function is able to detect
	RTCdelay(1000);

	I2C_TX_RX_Mode(I2C_RX_MODE);// Changing I2C module to receiver mode

	I2C_NACK();
	dummy=I2C_read_Byte();
	I2C_wait();// Checking if the I2C module is busy
	RTCdelay(1000);

	I2C_stop();// Generating stop signal
	dummy=I2C_read_Byte();
	return dummy;


}
uint8 readRTC_12hourformat(){
	uint8 hour=readRTC_hour();
	if(hour>0x12){
		setAMPMFlag(PMFLAG);
		return hour/2;
	}else{
		setAMPMFlag(AMFLAG);
		return hour;
	}

}
uint8 readRTC_day(){
	uint8 dummy=0;
	I2C_start();

	I2C_write_Byte(WRITECONTROL);
	I2C_wait();
	I2C_get_ACK(); //Waiting for acknowledge, this function is able to detect
	RTCdelay(1000);


	I2C_write_Byte(0x05);

	I2C_wait();
	I2C_get_ACK(); //Waiting for acknowledge, this function is able to detect
	RTCdelay(1000);


	I2C_repeated_Start();

	I2C_write_Byte(READCONTROL);
	I2C_wait();
	I2C_get_ACK(); //Waiting for acknowledge, this function is able to detect
	RTCdelay(1000);

	I2C_TX_RX_Mode(I2C_RX_MODE);// Changing I2C module to receiver mode

	I2C_NACK();
	dummy=I2C_read_Byte();
	I2C_wait();// Checking if the I2C module is busy
	RTCdelay(1000);

	I2C_stop();// Generating stop signal
	dummy=I2C_read_Byte();
	return dummy;
}
uint8 readRTC_month(){
		uint8 dummy=0;
		I2C_start();

		I2C_write_Byte(WRITECONTROL);
		I2C_wait();
		I2C_get_ACK(); //Waiting for acknowledge, this function is able to detect
		RTCdelay(1000);


		I2C_write_Byte(0x07);

		I2C_wait();
		I2C_get_ACK(); //Waiting for acknowledge, this function is able to detect
		RTCdelay(1000);


		I2C_repeated_Start();

		I2C_write_Byte(READCONTROL);
		I2C_wait();
		I2C_get_ACK(); //Waiting for acknowledge, this function is able to detect
		RTCdelay(1000);

		I2C_TX_RX_Mode(I2C_RX_MODE);// Changing I2C module to receiver mode

		I2C_NACK();
		dummy=I2C_read_Byte();
		I2C_wait();// Checking if the I2C module is busy
		RTCdelay(1000);

		I2C_stop();// Generating stop signal
		dummy=I2C_read_Byte();
		return dummy;


}
uint8 readRTC_year(){
		uint8 dummy=0;
		I2C_start();

		I2C_write_Byte(WRITECONTROL);
		I2C_wait();
		I2C_get_ACK(); //Waiting for acknowledge, this function is able to detect
		RTCdelay(1000);


		I2C_write_Byte(0x08);

		I2C_wait();
		I2C_get_ACK(); //Waiting for acknowledge, this function is able to detect
		RTCdelay(1000);


		I2C_repeated_Start();

		I2C_write_Byte(READCONTROL);
		I2C_wait();
		I2C_get_ACK(); //Waiting for acknowledge, this function is able to detect
		RTCdelay(1000);

		I2C_TX_RX_Mode(I2C_RX_MODE);// Changing I2C module to receiver mode

		I2C_NACK();
		dummy=I2C_read_Byte();
		I2C_wait();// Checking if the I2C module is busy
		RTCdelay(1000);

		I2C_stop();// Generating stop signal
		dummy=I2C_read_Byte();
		return dummy;


}

uint8 BCDDec(uint8 BCD){
	BCD=BCD>>BIT4;
	BCD&=~(0xF8);
	return BCD;
}
uint8 BCDUni(uint8 BCD){
	BCD&=~(0xF0);
	return BCD;
}
uint8 BCDHDec(uint8 BCD){
	BCD=BCD>>BIT4;
	BCD&=~(0xFC);
	return BCD;
}
uint8 BCDDayDec(uint8 BCD){
	BCD=BCD>>BIT4;
	BCD&=~(0xFC);
	return BCD;
}
uint8 BCDMonthDec(uint8 BCD){
	BCD=BCD>>BIT4;
	BCD&=~(0xFE);
	return BCD;
}
uint8 BCDYearDec(uint8 BCD){
	BCD=BCD>>BIT4;
	BCD&=~(0xF0);
	return BCD;
}
void RTCdelay(uint32 delay){
	uint32 i=delay;
	while(i!=0){
		i--;
	}
}
void setAMPMFlag(AMPM flag){
	AMPMFlag=flag;
}
AMPM getAMPMFlag(){
	return AMPMFlag;
}
