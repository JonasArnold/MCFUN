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

#include "platform.h"
#include "eeprom.h"
#include "i2c.h"
#include "util.h"

tConfig config;

#define EEPROM_ADR   0x50


void eepromLoad(void)
{
  i2cReadCmdData(EEPROM_ADR, 0x00, config.data, sizeof(tConfig));
}


void eepromSave(void)
{
  uint8_t p;
  uint8_t index = 0;
  uint8_t pages = sizeof(config) / 16;
  for (p=0; p<pages; p++)
  {
    i2cWriteCmdData(EEPROM_ADR, index, &config.data[index], 16);
    index = 16<<p;
    utilWaitUs(5000);
  }

  uint8_t bytes = sizeof(config) % 16;
  if (bytes > 0)
  {
    i2cWriteCmdData(EEPROM_ADR, index, &config.data[index], bytes);
    utilWaitUs(5000);
  }
}



void eepromInit(void)
{
  eepromLoad();
}
