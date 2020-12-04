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
#ifndef SOURCES_IO_JOYSTICK_H_
#define SOURCES_IO_JOYSTICK_H_


typedef enum
{
  jsRight   = (1<<0), // PTB0
  jsDown    = (1<<1), // PTB1
  jsUp      = (1<<2), // PTB2
  jsCenter  = (1<<3), // PTB3
  jsLeft    = (1<<9)  // PTB9
} tJoystick;


tJoystick joystick(void);
void joystickInit(void);


#endif /* SOURCES_IO_JOYSTICK_H_ */
