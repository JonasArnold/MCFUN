/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         quadrature decoder
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          15.04.2020
 *
 *--------------------------------------------------------------------
 */
#ifndef SOURCES_DRIVE_QUAD_H_
#define SOURCES_DRIVE_QUAD_H_


int16_t quadGetSpeedLeft(void);
int16_t quadGetSpeedRight(void);

void quadInit(void);

#endif /* SOURCES_DRIVE_QUAD_H_ */
