/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         motor driver
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          15.04.2020
 *
 *--------------------------------------------------------------------
 */

#ifndef SOURCES_DRIVE_MOTOR_H_
#define SOURCES_DRIVE_MOTOR_H_

#define MOTOR_MAX_VALUE               127

void motorIncrementPwmRight(int8_t value);
void motorIncrementPwmLeft(int8_t value);

void motorSetPwmRight(int8_t value);
void motorSetPwmLeft(int8_t value);
void motorInit(void);

#endif /* SOURCES_DRIVE_MOTOR_H_ */
