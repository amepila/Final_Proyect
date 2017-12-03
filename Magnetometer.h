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
/*
 * \brief Le da las configuraciones iniciales al magnetometro para que funcione de manera deseada.
 */
void setSysConfig();
/*
 * \brief Configura el GPIO relacionado con el magnetometro para conectar principalmente las interrupciones con la K64.
 */
void GPIOForMagnetometerInit();
/*
 * \brief Hace una lectura en cadena sobre los registros del magnetometro y los guarda en una estructura para que puedan ser
 * utilizados
 */
void burstReadMag(rawdatamag *mag);
/*
 * \brief Setter que se dispara en la interrupcion de data ready en el GPIO.C
 */
void setDataReady();
/*
 * \brief Getter del data ready usado principalmente para metodos de debugging
 */
uint8 getDataReady();
/*
 * \brief delay propio del magnetometro solo por fines de claridad
 */
void MAGdelay(uint32);
/*
 * \brief funcion que recibe un angulo y en base a ello dibujara una flecha que indicara hacia donde te tienes que girar para que ese angulo quede
 * frente a ti. usado principalmente para mostrar el norte
 * \param[dir] angulo del cual se basara las imagenes.
 */
void drawDirection(uint16 dir);
/*
 * \brief funcion que calibra el magnetometro por medio de sumas de valores mas altos y bajos posibles dados por el dispositivo y de ahi sacando un
 * promedio con el cual sea maas facil sacar los valores deseados del magnetometro
 */
void magCalibration();
/*
 * \brief Inicia el magnetometro, obtiene los valores de lectura del magnetometro y los trata para obtener valores legibles direccion y angulo
 */
void startCompass();
#endif /* MAGNETOMETER_H_ */
