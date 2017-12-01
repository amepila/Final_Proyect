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

/*
 * Escrituras estandar de I2C Start->WriteControl->Address->Data.
 *
 */
void setRTC_sec(uint8 sec){
	if(sec>60){
		sec=0; //Defaults the seconds counter to 0 when above 60 to avoid irregularities
	}
	uint8 BCDsec=(sec%10);

	BCDsec|=(sec/10)<<BIT4;
	writeI2CDevice(WRITECONTROL,0x02,BCDsec);


}

void setRTC_min(uint8 min){
	if(min>60){
		min=0; //Defaults the seconds counter to 0 when above 60 to avoid irregularities
	}
	uint8 BCDmin=(min%10);

	BCDmin|=(min/10)<<BIT4;
	writeI2CDevice(WRITECONTROL,0x03,BCDmin);

}

void setRTC_hour(uint8 hour){
	if(hour>23){
		hour=0; //Defaults the seconds counter to 0 when above 23 to avoid irregularities
	}
	uint8 BCDhour=(hour%10);

	BCDhour|=(hour/10)<<BIT4;

	writeI2CDevice(WRITECONTROL,0x04,BCDhour);
}





/*
 *
 *\//////////////////////////////////INICIAN LECTURAS DE RTC//////////////////////////////////
 *Para las lecturas del RTC se uso el protocolo estandar del I2C el se reduce a
 *Start->CodigoDeControl->ConfirmacrAcknowledge->Direccion a leer->ConfirmarAcknowledge->Parar escritura e iniciar lectura, cambiar a receptor, lectura de prueba->parar y leer el dato verdadero, retornandolo
 */
uint8 readRTC_sec(){
	readI2CDevice(WRITECONTROL,READCONTROL,0x02);


}
uint8 readRTC_min(){
	readI2CDevice(WRITECONTROL,READCONTROL,0x03);


}
uint8 readRTC_hour(){
	readI2CDevice(WRITECONTROL,READCONTROL,0x04);

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
