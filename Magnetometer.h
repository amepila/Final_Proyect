/*
 * Magnetometer.h
 *
 *  Created on: 14/11/2017
 *      Author: eric_
 */

#ifndef MAGNETOMETER_H_
#define MAGNETOMETER_H_
#include "MK64F12.h"
#include "DataTypeDefinitions.h"
#include "GPIO.h"
#include "I2C.h"
#include "LCDNokia5110.h"
#include "NVIC.h"
#include "SPI.h"
#include "stdlib.h"
#include "DirectionPicture.h"

typedef struct {
	sint16 x;
	sint16 y;
	sint16 z;
}rawdataacc;
typedef struct {
	sint16 xm;
	sint16 ym;
	sint16 zm;
}rawdatamag;
typedef struct{
	sint8 calX;
	sint8 calY;
	sint8 calZ;
}accCalData;
void setSysConfig();
void readSysConfig();
void GPIOForMagnetometerInit();
void burstWriteAccCalibration(sint8 Xcal, sint8 Ycal, sint8 Zcal);
void setMagnetometerConfig();
void setMagConfigReg1Def();
void setMagConfigReg1();
void setMagConfigReg2();
void setXYZ_config();
void setAccelControlReg1();
void setAccelControlReg2();
void readFXOS8700CQ(rawdatamag*);
uint8 readTest();
void burstReadMag(rawdatamag *mag);
void burstReadAcc(rawdataacc *acc);
void accCalibration();
void setDataReady();
uint8 getDataReady();
void MAGdelay(uint32);
void drawDirection(uint16 dir);
void magCalibration();
void startCompass();
#endif /* MAGNETOMETER_H_ */
