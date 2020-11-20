/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         joystick driver of the MC-Car
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          01.05.2020
 *
 *--------------------------------------------------------------------
 */

#include "platform.h"
#include "joystick.h"

tJoystick joystick(void)
{
  return (0b0000001000001111 & ~GPIOB->PDIR);
}


/**
 * Up:     PTB2
 * Dn:     PTB1
 * Right:  PTB0
 * Left:   PTB9
 * Center: PTB3
 */
void joystickInit(void)
{
  PORTB->PCR[0] = PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(1);
  PORTB->PCR[1] = PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(1);
  PORTB->PCR[2] = PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(1);
  PORTB->PCR[3] = PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(1);
  PORTB->PCR[9] = PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(1);
}
