/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Driver for the color sensor TCS34725
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          01.05.2020
 *
 *--------------------------------------------------------------------
 */
#ifndef SOURCES_I2C_COLSENS_H_
#define SOURCES_I2C_COLSENS_H_

typedef union
{
  uint8_t data[8];
  struct {
    uint16_t clear;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
  };
} tRawColorRGBC;


tError colSensReadColor(tColorRGB *col);
tError colSensReadRawColor(tRawColorRGBC *color);

tError colSensSetWhite(void);
tError colSensSetBlack(void);

tError colSensInit(void);

#endif /* SOURCES_I2C_COLSENS_H_ */
