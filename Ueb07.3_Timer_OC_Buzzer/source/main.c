/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Exercise 07.3 - Buzzer with timer output compare
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          04.04.2020
 *
 *--------------------------------------------------------------------
 */
#include "platform.h"
#include "ftm0.h"
#include "term.h"
#include "sound.h"
#include "soundPlayer.h"


/**
 * Buzzer test program with terminal
 */
void main(void)
{
  EnableDebugLeds();
  ftm0Init();
  termInit(57600);
  soundInit();
  soundPlayerInit();
  soundBeep(4000, 100);

  while(TRUE)
  {
    termDoWork();
  }

  // Never leave main
  for (;;){}
}


