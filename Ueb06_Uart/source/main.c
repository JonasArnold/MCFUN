/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Exercise 6 - serial communication
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          19.03.2020
 *
 *--------------------------------------------------------------------
 */
#include <stdio.h>
#include "platform.h"
#include "uart.h"
#include "term.h"
#include "led.h"


/**
 * Terminal-Demo
 */
void main(void)
{
  termInit(19200);  // 57600
  ledInit();

  while(TRUE)
  {
    termDoWork();
  }
}





