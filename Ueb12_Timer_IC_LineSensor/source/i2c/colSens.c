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
#include "platform.h"
#include "color.h"
#include "colSens.h"
#include "i2c.h"
#include "util.h"

#define I2C_COLSENS_ADR                 0x29



#define REG_COMMAND                     (0b10100000)      // Command with "Auto-increment protocol transaction"

// TCS34725 register definitions
#define REG_ENABLE                      0x00
#define REG_ATIME                       0x01
#define REG_WTIME                       0x03
#define REG_AILTL                       0x04
#define REG_AILTH                       0x05
#define REG_AIHTL                       0x06
#define REG_AIHTH                       0x07
#define REG_PERS                        0x0C
#define REG_CONFIG                      0x0D
#define REG_CONTROL                     0x0F
#define REG_ID                          0x12
#define REG_STATUS                      0x13
#define REG_CDATAL                      0x14
#define REG_CDATAH                      0x15
#define REG_RDATAL                      0x16
#define REG_RDATAH                      0x17
#define REG_GDATAL                      0x18
#define REG_GDATAH                      0x19
#define REG_BDATAL                      0x1A
#define REG_BDATAH                      0x1B

#define REG_ENABLE_PON_MASK             0x01
#define REG_ENABLE_PON_SHIFT            0x00
#define REG_ENABLE_PON(x)               (((uint8_t)(((uint8_t)(x))<<REG_ENABLE_PON_SHIFT))&REG_ENABLE_PON_MASK)

#define REG_ENABLE_AEN_MASK             0x02
#define REG_ENABLE_AEN_SHIFT            0x01
#define REG_ENABLE_AEN(x)               (((uint8_t)(((uint8_t)(x))<<REG_ENABLE_AEN_SHIFT))&REG_ENABLE_AEN_MASK)

#define REG_CONTROL_AGAIN_MASK          0x03
#define REG_CONTROL_AGAIN_SHIFT         0x00
#define REG_CONTROL_AGAIN(x)            (((uint8_t)(((uint8_t)(x))<<REG_CONTROL_AGAIN_SHIFT))&REG_CONTROL_AGAIN_MASK)

static tRawColorRGBC colWhite;  // contains the sensor values of the white color
static tRawColorRGBC colBlack;  // contains the sensor values of the black color



/**
 * Reads the raw color data from the color sensor.
 *
 * @param[out] col
 *   a pointer to a tRegColor struct which will contains the color read out of the sensor
 * @return
 *   the error code or EC_SUCCESS if no error has occurred
 */
tError colSensReadRawColor(tRawColorRGBC *col)
{
  tError result;
  uint8_t cmd = REG_COMMAND | REG_CDATAL;
  result = i2cReadCmdData(I2C_COLSENS_ADR, cmd, col->data, sizeof(tRawColorRGBC));
  return result;
}


/**
 * Reads the RGB color from the color sensor
 *
 * @param[out] col
 *   a pointer to a tColorRGB struct which will contains the color read out of the sensor
 *   taking account of the white and black level
 * @return
 *   the error code or EC_SUCCESS if no error has occurred
 */
tError colSensReadColor(tColorRGB *col)
{
  tError result;
  tRawColorRGBC c;
  int32_t value;
  result = colSensReadRawColor(&c);
  if (result) return result;

  value = (int32_t)c.red - colBlack.red;
  if (value < 0) value = 0;
  else {
    value = (value * 255) / (colWhite.red - colBlack.red);
    if (value > 255) value = 255;
  }
  col->red = value;

  value = (int32_t)c.green - colBlack.green;
  if (value < 0) value = 0;
  else {
    value = (value * 255) / (colWhite.green - colBlack.green);
    if (value > 255) value = 255;
  }
  col->green = value;

  value = (int32_t)c.blue - colBlack.blue;
  if (value < 0) value = 0;
  else {
    value = (value * 255) / (colWhite.blue - colBlack.blue);
    if (value > 255) value = 255;
  }
  col->blue = value;

  return EC_SUCCESS;
}


/**
 * Sets the current color as white color.
 *
 * @return
 *   the error code or EC_SUCCESS if no error has occurred
 */
tError colSensSetWhite(void)
{
  return colSensReadRawColor(&colWhite);
}


/**
 * Sets the current color value as black color.
 *
 * @return
 *   the error code or EC_SUCCESS if no error has occurred
 */
tError colSensSetBlack(void)
{
  return colSensReadRawColor(&colBlack);
}


tError colSensInit(void)
{
  tError result;

  // enable sensor step1: set PON bit
  uint8_t cmd = REG_COMMAND | REG_ENABLE;
  uint8_t data = REG_ENABLE_PON(1);
  result = i2cWriteCmdData(I2C_COLSENS_ADR, cmd, &data, sizeof(data));
  if (result) return result;

  utilWaitUs(3000); // Wait >2.4ms after PON=1 (Datasheet p22)

  // enable sensor step2: set PON & AEN bit to '1'
  cmd = REG_COMMAND | REG_ENABLE;
  data = REG_ENABLE_PON(1) | REG_ENABLE_AEN(1);
  result = i2cWriteCmdData(I2C_COLSENS_ADR, cmd, &data, sizeof(data));
  if (result) return result;

  // configure timing: ATIME = 0xC0
  // TIME = (256 - ATIME) x 2.4ms = (256 - 0xC0) x 2.4ms = 153.6ms
  cmd = REG_COMMAND | REG_ATIME;
  data = 0xC0;
  result = i2cWriteCmdData(I2C_COLSENS_ADR, cmd, &data, sizeof(data));
  if (result) return result;

  // configure gain to 60x
  cmd = REG_COMMAND | REG_CONTROL;
  data = REG_CONTROL_AGAIN(3); // 60x
  result = i2cWriteCmdData(I2C_COLSENS_ADR, cmd, &data, sizeof(data));
  if (result) return result;

  // enable the white led under the MC-Car
  PORTC->PCR[12] = PORT_PCR_MUX(1);
  GPIOC->PDDR = (1<<12);
  return EC_SUCCESS;
}
