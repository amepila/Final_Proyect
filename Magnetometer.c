/*
 * Magnetometer.c
 *
 *  Created on: 14/11/2017
 *      Author: eric_
 */
#include "MK64F12.h"
#include "DataTypeDefinitions.h"
#include <math.h>
#include "GPIO.h"
#include "I2C.h"
#include "LCDNokia5110.h"
#include "NVIC.h"
#include "SPI.h"
#include "stdlib.h"
#include "Magnetometer.h"
#include "DirectionPicture.h"
#define WRITECONTROL 0x3A
#define READCONTROL 0x3B
#define STATUS 0x00
#define WHOAMI 0x0D
#define XYZDATACONF 0x0E
#define ACC_CTRLREG 0x2A
#define MAGCTRLREG 0x5B
#define MAGCTRLREG2 0x5C
#define SYSCTRL1 0x2A
#define SYSCTRL2 0x2B
#define SYSCTRL3 0x2C
#define SYSCTRL4 0x2D
#define SYSCTRL5 0x2E
#define WHOIAMVAL 0xC7
uint8 dataReady;
double arc,inarc;
float AXmt, AYmt, AZmt;
float AXg,AYg,AZg;
uint8 fullDegrees=180;
rawdatamag mag;
//Esta funcion inicializa los puertos PTC13 Y PTC6 que estan conectados al FXOS8700CQ (accelerometro y magnetometro) de la K64.
//Los inicializa para que funcionen por medio de la pull-up resistance que tienen en medio.

void GPIOForMagnetometerInit(){
	GPIO_clockGating(GPIO_C);
	GPIO_clockGating(GPIO_A);
	static GPIO_pinControlRegisterType interrupt1= GPIO_MUX1|INTR_FALLING_EDGE|GPIO_PS|GPIO_PE;

	GPIO_pinControlRegister(GPIO_C,BIT13,&interrupt1);
	GPIO_pinControlRegister(GPIO_C,BIT6,&interrupt1);
	GPIO_pinControlRegister(GPIO_A,BIT4,&interrupt1);
	GPIO_dataDirectionPIN(GPIO_C,GPIO_INPUT,BIT13);
	GPIO_dataDirectionPIN(GPIO_C,GPIO_INPUT,BIT6);
	GPIO_dataDirectionPIN(GPIO_A,GPIO_INPUT,BIT4);
}
//SetSysConfig configura el FXOS8700CQ para que funcione de la manera especifica deseada.

void setSysConfig(){
	//Se reinician el FX0S8700CQ
	writeI2CDevice(WRITECONTROL,SYSCTRL2, 0x40);
	MAGdelay(3000); //Delay necesario para que escriba el siguiente dato ya que inicia de stand by
	//FXOS8700CQ en modo hibrido, accelerometro y magnetometro activado
	writeI2CDevice(WRITECONTROL,MAGCTRLREG,0x1F);
	MAGdelay(3000);
	//Escritura de confirmacion en 0s
	writeI2CDevice(WRITECONTROL,MAGCTRLREG2,0x00);
	MAGdelay(2000);

	//Captura en alta resolucion
	writeI2CDevice(WRITECONTROL,SYSCTRL2,0x02);
	MAGdelay(2000);

	//Interrupciones en push-pull, por la resistencia pull-up interna
	writeI2CDevice(WRITECONTROL,SYSCTRL3,0x00);
	MAGdelay(2000);

	//Activa la interrupcion de data ready
	writeI2CDevice(WRITECONTROL,SYSCTRL4,0x01);
	MAGdelay(2000);

	//Se coloca el data ready en el PTC6 (INT1 del magnetometro)
	writeI2CDevice(WRITECONTROL,SYSCTRL5,0x01);
	MAGdelay(2000);

	//Se generan datos cada 3.15Hz para reducir ruido.
	writeI2CDevice(WRITECONTROL,SYSCTRL1,0x35);
	MAGdelay(2000);

}
//BurstReadMag esta funcion hace una lectura en cadena de los registros donde se guardan los valores del magnetometro cada vez que
//los datos estan listos estos son almacenados en una estructura que se pasa por referencia para su utilizacion

void burstReadMag(rawdatamag *mag){
	if(dataReady==1){
	dataReady=0;
	int dummy=0;
	sint8 buffer[6];
	I2C_start(); //Generate Start Signal
	I2C_write_Byte(WRITECONTROL); //"10100000" Write control code, CSS and choose write
	I2C_wait();
	//I2C_get_ACK();
	I2C_write_Byte(0x33); //Write first most significant 8 bits
	I2C_wait();
	//I2C_get_ACK();
	I2C_repeated_Start();

	I2C_write_Byte(READCONTROL); //"10100000" Write control code, CSS and choose write
	I2C_wait();
	//I2C_get_ACK();
	I2C_TX_RX_Mode(I2C_RX_MODE);// Changing I2C module to receiver mode
	I2C0->C1 &= ~I2C_C1_TXAK_MASK;

	dummy=I2C_read_Byte();
	I2C_wait();
	for(int i=0;i<6-2;i++){
		buffer[i]=I2C_read_Byte();
		I2C_wait();
	}
	I2C_NACK();
	buffer[4]=I2C_read_Byte();
	I2C_wait();
	I2C_stop();// Generating stop signal
	buffer[5]=I2C_read_Byte();
	MAGdelay(500);

	mag->xm=((uint16)(buffer[0]<<8)|(buffer[1]));
	mag->ym=((uint16)(buffer[2]<<8)|(buffer[3]));
	mag->zm=((uint16)(buffer[4]<<8)|(buffer[5]));
	}
}
void burstReadAcc(rawdataacc *acc){
	if(dataReady==1){
	dataReady=0;
	int dummy=0;
	sint8 buffer[6];
	I2C_start(); //Generate Start Signal
	I2C_write_Byte(WRITECONTROL); //"10100000" Write control code, CSS and choose write
	I2C_wait();
	//I2C_get_ACK();
	I2C_write_Byte(0x01); //Write first most significant 8 bits
	I2C_wait();
	//I2C_get_ACK();
	I2C_repeated_Start();

	I2C_write_Byte(READCONTROL); //"10100000" Write control code, CSS and choose write
	I2C_wait();
	//I2C_get_ACK();
	I2C_TX_RX_Mode(I2C_RX_MODE);// Changing I2C module to receiver mode
	I2C0->C1 &= ~I2C_C1_TXAK_MASK;

	for(int i=0;i<6-2;i++){
		buffer[i]=I2C_read_Byte();
		I2C_wait();
	}
	I2C_NACK();
	buffer[4]=I2C_read_Byte();
	I2C_wait();
	I2C_stop();// Generating stop signal
	buffer[5]=I2C_read_Byte();
	MAGdelay(500);

	acc->x=((uint16)(buffer[0]<<8)|(buffer[1])>>2);
	acc->y=((uint16)(buffer[2]<<8)|(buffer[3])>>2);
	acc->z=((uint16)(buffer[4]<<8)|(buffer[5])>>2);
	}
}

void accCalibration(){
	sint8 Xcal;
	sint8 Ycal;
	sint8 Zcal;
	rawdataacc accCalib;
	dataReady=0;
	while(0==dataReady){};
	if(1==dataReady){
	burstReadAcc(&accCalib);
	Xcal=accCalib.x/8*(-1);
	Ycal=accCalib.y/8*(-1);
	Zcal=(accCalib.z-4096)/8*(-1);
	//Accelerometer Offset Registers X, Y , Z
	writeI2CDevice(WRITECONTROL,SYSCTRL1,0x00);
	writeI2CDevice(WRITECONTROL,0x2F,Xcal);
	writeI2CDevice(WRITECONTROL,0x30,Ycal);
	writeI2CDevice(WRITECONTROL,0x31,Zcal);
	writeI2CDevice(WRITECONTROL,SYSCTRL1,0x35);
	}

}
void burstReadAccMag(rawdataacc *acc,rawdatamag *mag){
	dataReady=0;
	uint8 dummy=0;
	sint8 buffer[12];

	if(1==dataReady){
	dataReady=0;

	I2C_start(); //Generate Start Signal
	I2C_write_Byte(WRITECONTROL); //"10100000" Write control code, CSS and choose write
	I2C_wait();
	I2C_write_Byte(0x01); //Write first most significant 8 bits
	I2C_wait();

	I2C_repeated_Start();

	I2C_write_Byte(READCONTROL); //"10100000" Write control code, CSS and choose write
	I2C_wait();

	I2C_TX_RX_Mode(I2C_RX_MODE);// Changing I2C module to receiver mode
	I2C0->C1 &= ~I2C_C1_TXAK_MASK;

	dummy=I2C_read_Byte();
	for(int i=0;i<12-2;i++){
		buffer[i]=I2C_read_Byte();
		I2C_wait();
	}
	I2C_NACK();
	buffer[10]=I2C_read_Byte();
	I2C_wait();
	I2C_stop();// Generating stop signal
	buffer[11]=I2C_read_Byte();
	MAGdelay(500);

	acc->x=((uint16)(buffer[0]<<BIT8)|(buffer[1]))>>BIT2;
	acc->y=((uint16)(buffer[2]<<BIT8)|(buffer[3]))>>BIT2;
	acc->z=((uint16)(buffer[4]<<BIT8)|(buffer[5]))>>BIT2;
	mag->xm=((uint16)(buffer[6]<<8)|(buffer[7]));
	mag->ym=((uint16)(buffer[8]<<8)|(buffer[9]));
	mag->zm=((uint16)(buffer[10]<<8)|(buffer[11]));
	}
}
//Se aprovecha el ciclo de calibracion para mostrar una imagen animada en lo que se completa la calibracion
//Cada 15 ciclos de i avanzara j con ello avanzara el frame y permitira que la animacion se  vea de manera mas limpia
//es decir reduce la velocidad.
void magCalibration(){
	sint16 Xmcalmax;
	sint16 Ymcalmax;
	sint16 Zmcalmax;
	sint16 Xmcalmin;
	sint16 Ymcalmin;
	sint16 Zmcalmin;
	sint16 XmAv;
	sint16 YmAv;
	sint16 ZmAv;
	rawdatamag magCalib;
	uint8 i=0;
	uint8 j=0;
	uint8 k=0;
	dataReady=0;

	while(i<120){
		j++;
		if(j==45){
			k++;
			j=0;
		}
		if(k>7){
			k=0;
		}
		printLoading(k);
	if(1==dataReady){
		burstReadMag(&magCalib);
		if(i==0){
		Xmcalmax=magCalib.xm;
		Xmcalmin=magCalib.xm;

		Ymcalmax=magCalib.ym;
		Ymcalmin=magCalib.ym;


		Zmcalmax=magCalib.zm;
		Zmcalmin=magCalib.zm;

		}
		//Confirming max and mins for X magnometer values.
		if(magCalib.xm>Xmcalmax){
			Xmcalmax=magCalib.xm;
		}
		if(magCalib.xm<Xmcalmin){
			Xmcalmin=magCalib.xm;
		}
		//Confirming max and mins for Y magnometer values.
		if(magCalib.ym>Ymcalmax){
			Ymcalmax=magCalib.ym;
		}
		if(magCalib.ym<Ymcalmin){
			Ymcalmin=magCalib.ym;
		}
		//Confirming max and mins for Z magnometer values.
		if(magCalib.zm>Zmcalmax){
			Zmcalmax=magCalib.zm;
		}
		if(magCalib.zm<Zmcalmin){
			Zmcalmin=magCalib.zm;
		}
		dataReady=0;
		i++;

		}
	}
	//End data verification. Averaging obtained Data.
	XmAv=(Xmcalmax+Xmcalmin)/2;
	YmAv=(Ymcalmax+Ymcalmin)/2;
	ZmAv=(Zmcalmax+Zmcalmin)/2;
	//Justifing to 15 bits
	XmAv<<=1;
	YmAv<<=1;
	ZmAv<<=1;
	writeI2CDevice(WRITECONTROL,SYSCTRL1,0x00);
	writeI2CDevice(WRITECONTROL,0x3F,(XmAv>>BIT8)&0xFF);
	writeI2CDevice(WRITECONTROL,0x40,XmAv&0xFF);
	writeI2CDevice(WRITECONTROL,0x41,(YmAv>>BIT8)&0xFF);
	writeI2CDevice(WRITECONTROL,0x42,YmAv&0xFF);
	writeI2CDevice(WRITECONTROL,0x43,(ZmAv>>BIT8)&0xFF);
	writeI2CDevice(WRITECONTROL,0x44,YmAv&0xFF);
	writeI2CDevice(WRITECONTROL,SYSCTRL1,0x35);


}

//En base en la direccion a la que se encuentre el norte, se dibujara una flecha que indique hacia donde se tiene que girar para
//apuntar al norte.
void drawDirection(uint16 dir){
	if(dir>0&&dir<30){
		LCDNokia_bitmap((uint8*)getPunt0());
	}
	else if(dir>30&&dir<60){
		LCDNokia_bitmap((uint8*)getPunt330());
	}
	else if(dir>60&&dir<90){
		LCDNokia_bitmap((uint8*)getPunt300());
	}
	else if(dir>90&&dir<120){
		LCDNokia_bitmap((uint8*)getPunt270());
	}
	else if(dir>120&&dir<150){
		LCDNokia_bitmap((uint8*)getPunt240());
	}
	else if(dir>120&&dir<150){
		LCDNokia_bitmap((uint8*)getPunt210());
	}
	else if(dir>150&&dir<180){
		LCDNokia_bitmap((uint8*)getPunt180());
	}
	else if(dir>180&&dir<210){
		LCDNokia_bitmap((uint8*)getPunt150());
	}
	else if(dir>210&&dir<240){
		LCDNokia_bitmap((uint8*)getPunt120());
	}
	else if(dir>240&&dir<270){
		LCDNokia_bitmap((uint8*)getPunt90());
	}
	else if(dir>270&&dir<300){
		LCDNokia_bitmap((uint8*)getPunt60());
	}
	else if(dir>300&&dir<330){
		LCDNokia_bitmap((uint8*)getPunt30());
	}
	else if(dir>330&&dir<360){
		LCDNokia_bitmap((uint8*)getPunt0());
	}

}
//Funcion que ejecuta todo lo necesario para que funcione el compass

void startCompass(){
	//Lectura en cadena de los registros del magnetometro

	burstReadMag(&mag);
	//Calculos en base a la datasheet, relacionados con la sensibilidad del componente
	AXmt=(float)mag.xm/10;
	AYmt=(float)mag.ym/10;
	AZmt=(float)mag.zm/10;
	/*
	 * Ya que se quiere obtener el angulo que se encuentra entre dos vectores se usara el dividir el vector Y/X
	 * y obtener su arcotangente para obtener los grados del vector en radianes. se multiplica por 180 entre pi para cambiarlo a
	 * grados en circulo y se le suman 180 para evitar numeros negativos y evitar el manejo de numeros negativos en el LCDNokia5110
	 *
	 */
	inarc=AYmt/AXmt;
	arc=fullDegrees+(float)atan2(AYmt,AXmt)*180/3.1415;
	drawDirection(arc);
	//Para complementar tambien se muestra el la distancia actual al norte en grados.

	LCDNokia_gotoXY(0,0);
	LCDNokia_printValue(arc);
}
//Setter para el data ready usada en la interrupcion.

void setDataReady(){
	dataReady=1;
}
uint8 getDataReady(){
	return dataReady;
}
//Delay especifico del Magnetometro.
void MAGdelay(uint32 delay){
	uint32 i=delay;
	while(i!=0){
		i--;
	}
}
