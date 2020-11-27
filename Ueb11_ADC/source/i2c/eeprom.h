/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         i2c eeprom driver
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          01.05.2020
 *
 *--------------------------------------------------------------------
 */

#ifndef SOURCES_I2C_EEPROM_H_
#define SOURCES_I2C_EEPROM_H_


typedef union
{
  uint8_t data[32];
  struct
  {
    uint16_t lsCalibDataWhite[8];
    uint16_t lsCalibDataBlack[8];
  };
} tConfig;


extern tConfig config;

void eepromLoad(void);
void eepromSave(void);
void eepromInit(void);

#endif /* SOURCES_I2C_EEPROM_H_ */
