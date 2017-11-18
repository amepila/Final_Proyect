/*
 * I2CE2PROM.h
 *
 *  Created on: 08/11/2017
 *      Author: eric_
 */

#ifndef I2CE2PROM_H_
#define I2CE2PROM_H_

/*\brief delay para E2PROM hecho por medio de un while
 * \param[delay] numero de ciclos que durara dentro del while
 * */
void E2PROMdelay(uint32 delay);
/*\brief funcion para escribir en memoria recibe una direccion de 16 bits y el dato que se quiere escribir, por medio del protocolo estandar de I2C
 * \param[add] direccion a la que se quiere escribir
 *\param[data] datos a escribir
 * */
void writeMemory(uint16 add,uint8 data);
/*\brief funcion para leer la memoria recibe una direccion de 16 bits para obtener el dato por medio del protocolo estandar de I2C
 * \param[add] direccion a la que se quiere leer
 * */
uint8 readMemory(uint16 add);

#endif /* I2CE2PROM_H_ */
