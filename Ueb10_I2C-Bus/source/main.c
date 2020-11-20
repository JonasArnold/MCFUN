/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Exercise 10 - I2C-Bus
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          01.05.2020
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



// calulate nr of TOF count for a given number of milliseconds
#define TOFS_MS(x)   ((uint16_t)(((FTM3_CLOCK / 1000) * x) / (FTM3_MODULO + 1)))

int16_t speedL = 0;
int16_t speedR = 0;

uint8_t ki =20, kp = 70;

void ProcessInfraredCommand(char key)
{
  uint8_t value = 10;

  switch (key)
  {
    // motor.c test for left motor
    case Key1 : motorIncrementPwmLeft(-value); break;
    case Key2 : motorSetPwmLeft(0); break;
    case Key3 : motorIncrementPwmLeft(+value); break;

    // motor.c test for right motor
    case Key4 : motorIncrementPwmRight(-value); break;
    case Key5 : motorSetPwmRight(0); break;
    case Key6 : motorIncrementPwmRight(+value); break;

    // drive.c test
    case KeyUp:               // drive forward
      speedL += 100;
      speedR += 100;
      break;

    case KeyDown:             // drive backward
      speedL -= 100;
      speedR -= 100;
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
      break;

    case KeyStar:             // predefined velocity
      speedR = speedL = 1300;
      break;
  }

  if (pwrSwitchEnabled())
  {
    driveSetSpeed(speedL, speedR);
    driveSetParameters(kp, kp, ki, ki);
  }
  else
  {

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

void CheckColorSensor(void)
{
  static uint16_t j;

  if (j++ == TOFS_MS(250)) {            // toogle rear leds every 250ms
    j=0;
    uint8_t id;
    if (i2cReadCmdData(0x29, 0x92, &id, sizeof(id)) == 0)
    {
      GPIOA->PTOR = (1<<17);
      if (id == 0x44) GPIOA->PTOR = (1<<15);
    }
  }
}

void ProcessDrive(void)
{
  static uint16_t i;
  if (i++ == TOFS_MS(25)) {             // run pid worker every 25ms
    i=0;
    if (pwrSwitchEnabled()) {
      driveToWork();
    }
    else {
      driveSetSpeed(0, 0);
    }
  }
}

void ProcessJoystick(void)
{
  if (joystick() & jsLeft) // Joystick Left Key
  {
    colSensSetBlack();
  }

  if (joystick() & jsRight) // Joystick Right Key
  {
    colSensSetWhite();
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
  colSensInit();
  joystickInit();

  soundBeep(4000, 100);

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
      CheckColorSensor();
      ProcessColorSensor();
      ProcessJoystick();
    }
  }
}
