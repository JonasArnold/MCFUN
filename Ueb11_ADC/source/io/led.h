/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         led driver of the MC-Car
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          01.05.2020
 *
 *--------------------------------------------------------------------
 */
#ifndef SOURCES_UTILS_LED_H_
#define SOURCES_UTILS_LED_H_

typedef enum
{
  ledFrontLeftRed = 1,
  ledFrontLeftGreen = 2,
  ledFrontLeftBlue = 4,
  ledFrontLeftCyan = ledFrontLeftGreen | ledFrontLeftBlue,
  ledFrontLeftMagenta = ledFrontLeftRed | ledFrontLeftBlue,
  ledFrontLeftYellow = ledFrontLeftRed | ledFrontLeftGreen,
  ledFrontLeftWhite = ledFrontLeftRed | ledFrontLeftGreen | ledFrontLeftBlue,
  ledFrontLeft = (ledFrontLeftRed | ledFrontLeftGreen | ledFrontLeftBlue),

  ledFrontRightRed = 8,
  ledFrontRightGreen = 16,
  ledFrontRightBlue = 32,
  ledFrontRightCyan = ledFrontRightGreen | ledFrontRightBlue,
  ledFrontRightMagenta = ledFrontRightRed | ledFrontRightBlue,
  ledFrontRightYellow = ledFrontRightRed | ledFrontRightGreen,
  ledFrontRightWhite = ledFrontRightRed | ledFrontRightGreen | ledFrontRightBlue,
  ledFrontRight = (ledFrontRightRed | ledFrontRightGreen | ledFrontRightBlue),

  ledFrontRed = ledFrontLeftRed | ledFrontRightRed,
  ledFrontGreen = ledFrontLeftGreen | ledFrontRightGreen,
  ledFrontBlue = ledFrontLeftBlue | ledFrontRightBlue,
  ledFrontCyan = ledFrontGreen | ledFrontBlue,
  ledFrontMagenta = ledFrontRed | ledFrontBlue,
  ledFrontYellow = ledFrontRed | ledFrontGreen,
  ledFrontWhite = ledFrontLeft | ledFrontRight,
  ledFront = ledFrontLeft | ledFrontRight,

  ledAll = 0xff,
  ledNone = 0
} Leds;

void ledSet(Leds led);
void ledClear(Leds led);
void ledSetColorLeft(uint8_t red, uint8_t green, uint8_t blue);
void ledSetColorRight(uint8_t red, uint8_t green, uint8_t blue);
void ledInit(void);

#endif /* SOURCES_UTILS_LED_H_ */
