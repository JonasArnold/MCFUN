/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Exercise 09 - Motor Driver with PWM
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          14.04.2020
 *
 *--------------------------------------------------------------------
 */
#include <stdbool.h>
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
#include "led.h"
#include "color.h"

const char soundStarWars[] = "StarWars/Imp:d=4,o=5,b=112:8d.,16p,8d.,16p,8d.,16p,8a#4,16p,16f,8d.,16p,8a#4,16p,16f,d.,8p,8a.,16p,8a.,16p,8a.,16p,8a#,16p,16f,8c#.,16p,8a#4,16p,16f,d.,8p,8d.6,16p,8d,16p,16d,8d6,8p,8c#6,16p,16c6,16b,16a#,8b,8p,16d#,16p,8g#,8p,8g,16p,16f#,16f,16e,8f,8p,16a#4,16p,2c#";

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

    case Key0 : soundPlay(soundStarWars); break;

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
      soundStop();
      break;

    case KeyStar:             // predefined velocity
      speedR = speedL = 500;
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
  soundBeep(4000, 100);
  ledInit();
  ledSetColorLeft(50, 0, 0);
  ledSetColorLeft(0, 50, 0);
  ledSetColorLeft(50, 50, 0);



  // configure read red leds on PTA15 and PTA17
  PORTA->PCR[15] = PORTA->PCR[17] = PORT_PCR_MUX(1);
  GPIOA->PDDR |= (1<<15) | (1<<17);

  // configure front blue leds

  static uint16_t i, j, k;
  uint16_t hue;
  tColorRGB rgb;


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
      FTM3->SC &= ~FTM_SC_TOF_MASK;              // clear TOF flag

      if (j++ == FTM3_TOFS_MS(250)) {           // toogle rear leds every 250ms
        j=0;
        GPIOA->PTOR = (1<<15) | (1<<17);
      }


      if (i++ == FTM3_TOFS_MS(PID_PERIOD)) {    // run pid worker every 25ms
        i=0;
        if (pwrSwitchEnabled()) {
          driveToWork();
        }
        else {
          driveSetSpeed(0, 0);
        }
      }

      if (k++ == FTM3_TOFS_MS(25))
      {
        k=0;
        if (++hue >= 360) hue = 0;

        colorHueToRGB(hue, &rgb);
        ledSetColorLeft(rgb.red, rgb.green, rgb.blue);
        ledSetColorRight(rgb.red, rgb.green, rgb.blue);
      }
    }
  }

  // Never leave main
  for (;;){}
}
