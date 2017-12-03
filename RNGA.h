/*
 * RNGA.h
 *
 *  Created on: Nov 23, 2017
 *      Author: Andres Hernandez
 */

#ifndef RNGA_H_
#define RNGA_H_

#include "DataTypeDefinitions.h"

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Enable the RNGA module to generate random numbers
 	 \param[in]  void
 	 \return TRUE
 */
void enableRNGA(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 Getter of the random data
 	 \param[in]  void
 	 \return random data
 */
uint32 getRandomData(void);

#endif /* RNGA_H_ */
