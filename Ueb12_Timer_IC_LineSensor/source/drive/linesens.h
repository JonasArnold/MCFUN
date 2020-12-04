/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Linesensor
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          16.05.2020
 *
 *--------------------------------------------------------------------
 */

#ifndef SOURCES_DRIVE_LINESENS_H_
#define SOURCES_DRIVE_LINESENS_H_

void lsCalibWhite(void);
void lsCalibBlack(void);
uint8_t lsGetSensData(void);
int8_t lsGetDir(void);
bool lsDetectLine(void);
void lsInit(void);

#endif /* SOURCES_DRIVE_LINESENS_H_ */
