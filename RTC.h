/*
 * RTC.h
 *
 *  Created on: 01/11/2017
 *      Author: eric_
 */

#ifndef RTC_H_
#define RTC_H_
#include <stdio.h>
#include "MK64F12.h"
#include "DataTypeDefinitions.h"
#include "GPIO.h"
#include "I2C.h"
#include "LCDNokia5110.h"
#include "NVIC.h"
#include "SPI.h"

typedef enum {SUCCESS,MEMORYERROR} RTCErrorCode;
typedef enum {AMFLAG,PMFLAG} AMPM; //\enum Enum for the Am Pm flag control
/*\brief Esta funcion regresa el valor actual de la bandera AMPM
 * \return bandera actual en modo de 12 hrs
 * */
AMPM getAMPMFlag();
/*\brief Esta funcion recibe un valor para la bandera de am pm usada en el modo de 12 hrs
 * \param[AMPM] Recibe ya sea la bandera de AM o PM para ponerla como valor.
 *
 * */
void setAMPMFlah(AMPM);
void RTCdelay();
/*\brief Esta funcion recibe un valor entero que se convierte en BCD para ser guardado en la parte de segundos del RTC.
 * \param[sec] numero entero para segundos.
 *
 * */
void setRTC_sec(uint8 sec);
/*\brief Esta funcion recibe un valor entero que se convierte en BCD para ser guardado en la parte de minutos del RTC.
 * \param[min] numero entero para segundos.
 *
 * */
void setRTC_min(uint8 min);
/*\brief Esta funcion recibe un valor entero que se convierte en BCD para ser guardado en la parte de segundos del RTC.
 * \param[hour] numero entero para segundos.
 *
 * */
void setRTC_hour(uint8 hour);
/*\brief Esta funcion recibe un valor entero que se convierte en BCD para ser guardado en la parte de dias del RTC.
 * \param[day] numero entero para segundos.
 *
 * */
void setRTC_day(uint8 day);
/*\brief Esta funcion recibe un valor entero que se convierte en BCD para ser guardado en la parte de mes del RTC.
 * \param[month] numero entero para segundos.
 *
 * */
void setRTC_month(uint8 month);
/*\brief Esta funcion recibe un valor entero que se convierte en BCD para ser guardado en la parte de año del RTC.
 * \param[year] numero entero para segundos.
 *
 * */
void setRTC_year(uint16 year);


/*\brief Esta funcion accesa a la direccion de memoria de horas del RTC para regresar el valor
 * \return entero en formato BCD que representa horas
 *
 * */
uint8 readRTC_hour();
/*\brief Esta funcion accesa a la direccion de memoria de horas del RTC y cambia la interpretacion del formato para que sea legible en formato de 12 horas
 * cambiando las banderas que representan AM y PM
 * \return entero en formato BCD que representa horas
 *
 * */
uint8 readRTC_12hourformat();
/*\brief Esta funcion accesa a la direccion de memoria de minutos y la regresa el dato
 * \return entero en formato BCD que representa minutos
 * */
uint8 readRTC_min();
/*\brief Esta funcion accesa a la direccion de memoria de segundos y regresa el dato
 * \return entero en formato BCD que representa segundos
 * */
uint8 readRTC_sec();
/*\brief Esta funcion accesa a la direccion de memoria de dia y regresa el datoa
 * \return entero en formato BCD que representa dia
 * */
uint8 readRTC_day();
/*\brief Esta funcion accesa a la direccion de memoria de meses y regresa el dato
 * \return entero en formato BCD que representa meses
 * */
uint8 readRTC_month();
/*\brief Esta funcion accesa a la direccion de memoria de años y regresa el dato
 * \return entero en formato BCD que representa años
 * */
uint8 readRTC_year();
/*\brief Esta funcion accesa a la direccion de memoria de dias y
 * \return entero en formato BCD que representa dias
 * */
uint8 readRTC_day();

/*\brief Convierte la parte alta de la representacion BCD de un byte a entero
 * \return parte alta de la representacion BCD a entero
 * */
uint8 BCDDec(uint8 BCD);
/*\brief Convierte la parte baja de la representacion BCD de un byte a entero
 * \return parte baja de la representacion BCD a entero
 * */
uint8 BCDUni(uint8 BCD);
/*\brief Convierte la parte alta de la representacion BCD de un byte a entero adaptado para la direccion de memoria de horas del RTC
 * \return parte alta de la representacion BCD a entero
 * */
uint8 BCDHDec(uint8 BCD);
/*\brief Convierte la parte baja de la representacion BCD de un byte a entero adaptada para los dias
 * \return parte baja de la representacion BCD a entero
 * */
uint8 BCDDayDec(uint8 BCD);
/*\brief Convierte la parte baja de la representacion BCD de un byte a entero adaptada para los meses
 * \return parte baja de la representacion BCD a entero
 * */
uint8 BCDMonthDec(uint8 BCD);
/*\brief Convierte la parte baja de la representacion BCD de un byte a entero adaptada para los años
 * \return parte baja de la representacion BCD a entero
 * */
uint8 BCDYearDec(uint8 BCD);
/*\brief delay para RTC hecho por medio de un while
 * \param[delay] numero de ciclos que durara dentro del while
 * */
void RTCdelay(uint32 delay);
/*\brief set para la bandera que indica que no se encuentra la memoria
 * \param[RTCErrorCode] tipo enum que 0 cuando no hay problema, 1 cuando existe error.
 * */
void setRTCError(RTCErrorCode);
/*\brief get para la bandera de error 0 cuando no hay problema, 1 cuando existe error.
 *
 * */
RTCErrorCode getRTCError(void);

#endif /* RTC_H_ */
