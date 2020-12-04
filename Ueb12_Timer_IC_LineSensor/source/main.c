/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Exercise 12 - Line Sensor
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          16.05.2020
 *
 *--------------------------------------------------------------------
 */
#include "platform.h"
#include "ftm0.h"
#include "ftm3.h"
#include "motor.h"
#include "quad.h"
#include "ir.h"
#include "term.h"
#include "sound.h"
#include "soundPlayer.h"
#include "drive.h"
#include "pwrSwitch.h"
#include "i2c.h"
#include "led.h"
#include "color.h"
#include "colSens.h"
#include "joystick.h"
#include "adc.h"
#include "SSD1306.h"
#include "util.h"
#include "linesens.h"
#include "eeprom.h"


// calulate nr of TOF count for a given number of milliseconds
#define TOFS_MS(x)   ((uint16_t)(((FTM3_CLOCK / 1000) * x) / (FTM3_MODULO + 1)))

int16_t speedL = 0;
int16_t speedR = 0;
int16_t speed;


void ProcessInfraredCommand(char key)
{
  switch (key)
  {
    case KeyUp:               // drive forward
      speedL += 100;
      speedR += 100;
      speed += 50;
      break;

    case KeyDown:             // drive backward
      speedL -= 100;
      speedR -= 100;
      speed -= 50;
      break;

    case KeyRight:            // drive right
      speedR -= 10;
      speedL += 10;
      break;

    case KeyLeft:             // drive left
      speedR += 10;
      speedL -= 10;
      break;

    case KeyOK:               // Stop
      speedR = speedL = 0;
      speed = 0;
      break;

    case KeyStar:             // predefined velocity
      speed = speedR = speedL = 500;
      break;
  }

  if (pwrSwitchEnabled())
  {
    if (speed && speed < 100) speed = 100;
    if (speed > 200) speed = 200;
  }
  else
  {
    driveSetSpeed(speedL, speedR);
  }
}

void ProcessColorSensor(void)
{
  static uint16_t k;

  if (k++ == TOFS_MS(50))
  {
    k=0;
    tColorRGB col;
    tColorHSV hsv;
    colSensReadColor(&col);
    colorRGBtoHSV(&col, &hsv);
    colorHueToRGB(hsv.hue, &col);
    ledSetColorLeft(col.red, col.green, col.blue);
    ledSetColorRight(col.red, col.green, col.blue);
  }
}

void CheckAdc(void)
{
  static uint16_t j;

  if (j++ == TOFS_MS(250)) {            // toogle rear leds every 250ms
    j=0;
    if (adcGet16BitValue(18) > 65000)
    {
      GPIOA->PTOR = (1<<17);
      if (adcGetVoltage(18) > 1190000) GPIOA->PTOR = (1<<15);
    }
  }
}


void ProcessDrive(void)
{
  static uint16_t i;
  if (i++ == TOFS_MS(25)) {             // run pid worker every 25ms
    i=0;
    if (pwrSwitchEnabled()) {

      speedR = speedL = 0;
      if (speed != 0)
      {
        int16_t dir = lsGetDir() * 11;
        uint16_t offset;
        if (dir > 0) offset = dir + (speed / 25);
        if (dir < 0) offset = dir - (speed / 25);
        speedL = speed - offset;
        speedR = speed + offset;
        if (dir == 127) speed = speedL = speedR = 0;
      }

      driveSetSpeed(speedL, speedR);
      driveToWork();
    }
    else {
      driveToWork();
    }
  }
}

void ProcessJoystick(void)
{
  if (joystick() & jsLeft) // Joystick Left Key
  {
    colSensSetBlack();
    lsCalibBlack();
  }

  if (joystick() & jsRight) // Joystick Right Key
  {
    colSensSetWhite();
    lsCalibWhite();
  }
}

void UpdateDisplay(void)
{
  static uint16_t current;
  char buf[32];
  static uint16_t i;
  if (i++ == TOFS_MS(250))
  {
    i = 0;
    int16_t voltage = adcGetBatVoltage();
    current = ((1 * current) + adcGetCurrent()) / 2;
    int16_t temp = adcGetTemperature();
    SSD1306_SetPageStartAddr(0);
    SSD1306_SetColStartAddr(0);

    if (voltage > 0)
    {
      SSD1306_PrintString("Bat Voltage: ");
      buf[0]='\0';
      utilStrcatNum16u(buf, sizeof(buf), voltage);
      SSD1306_PrintString(buf);
      SSD1306_PrintString(" mV \n");
    }

    if (current < 5000)
    {
      SSD1306_PrintString("Current: ");
      buf[0]='\0';
      utilStrcatNum16u(buf, sizeof(buf), current);
      SSD1306_PrintString(buf);
      SSD1306_PrintString(" mA \n");
    }

    if (temp < 1000)
    {
      SSD1306_PrintString("Temperature: ");
      buf[0]='\0';
      utilStrcatNum16s(buf, sizeof(buf), temp/10);
      utilStrcat(buf, sizeof(buf), ".");
      if (temp<0) temp = -temp;
      utilStrcatNum16s(buf, sizeof(buf), temp%10);
      SSD1306_PrintString(buf);
      SSD1306_PrintString(" C \n");
    }

    uint8_t s = lsGetSensData();
    uint8_t i;
    SSD1306_PrintString("LS: ");
    for (i=0; i<8; i++)
    {
      if (s & (1<<i))
      {
        SSD1306_PrintChar('X');
      }
      else
      {
        SSD1306_PrintChar('_');
      }
    }

    SSD1306_PrintChar(' ');
    int8_t dir = lsGetDir();
    buf[0]='\0';
    utilStrcatNum16s(buf, sizeof(buf), (int16_t)dir);
    SSD1306_PrintString(buf);
    SSD1306_PrintString("   ");

  }
}


/**
 * The main function of the MC-Car app.
 */
void main(void)
{
  ftm0Init();
  ftm3Init();
  termInit(57600);
  soundInit();
  soundPlayerInit();
  irInit();
  motorInit();
  quadInit();
  driveInit();
  pwrSwitchInit();
  EnableDebugLeds();
  ledInit();
  i2cInit();
  eepromInit();
  colSensInit();
  joystickInit();
  adcInit();
  lsInit();
  SSD1306_Init();

  soundBeep(4000, 100);

  SSD1306_Clear();

  // configure read red leds on PTA15 and PTA17
  PORTA->PCR[15] = PORTA->PCR[17] = PORT_PCR_MUX(1);
  GPIOA->PDDR |= (1<<15) | (1<<17);

  while(TRUE)
  {
    // check for commands from terminal
    termDoWork();

    // check for infrared remote control
    char key = irGetKey();
    if (key) ProcessInfraredCommand(key);

    // check for timer overflow
    if (FTM3->SC & FTM_SC_TOF_MASK)
    {
      FTM3->SC &= ~FTM_SC_TOF_MASK;    // clear TOF flag
      ProcessDrive();
      CheckAdc();
      ProcessColorSensor();
      ProcessJoystick();
      UpdateDisplay();
    }
  }
}
