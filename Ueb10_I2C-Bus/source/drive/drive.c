/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Drive with PID
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          15.04.2020
 *
 *--------------------------------------------------------------------
 */

#include "platform.h"
#include "drive.h"
#include "quad.h"
#include "motor.h"

static int16_t setValueLeft;
static int16_t setValueRight;
static uint8_t kpL, kiL, kdL;
static uint8_t kpR, kiR, kdR;
static int16_t integL, devL, devOldL;
static int16_t integR, devR, devOldR;
static int32_t valL, valR;



#define PID_LOOPS_PER_SECOND  (1000 / PID_PERIOD)
#define ACCELERATION_OFFSET   (ACCELERATION / PID_LOOPS_PER_SECOND)
#define DECELERATION_OFFSET   (DECELERATION / PID_LOOPS_PER_SECOND)

int16_t sl[256];
int16_t sr[256];
uint8_t sli, sri;




/**
 * Sets the speed
 * @param[in] speedL
 *   the speed of the left wheel in mm/sec
 * @param[in] speedR
 *   the speed of the right wheel in mm/sec
 */
void driveSetSpeed(int16_t speedL, int16_t speedR)
{
  if (speedL > MAX_SPEED) speedL = MAX_SPEED;
  else if (speedL < -MAX_SPEED) speedL = -MAX_SPEED;

  if (speedR > MAX_SPEED) speedR = MAX_SPEED;
  else if (speedR < -MAX_SPEED) speedR = -MAX_SPEED;

  setValueLeft = speedL;
  setValueRight = speedR;
}


/**
 * This function sets the control parameters
 * @param[in] pKpL Kp left 0..255
 * @param[in] pKpR Kp right 0..255
 * @param[in] pKiL Ki left 0..255
 * @param[in] pKiR Ki right 0..255
 */
void driveSetParameters(uint8_t pKpL, uint8_t pKpR, uint8_t pKiL, uint8_t pKiR)
{
  kpL = pKpL;
  kpR = pKpR;
  kiL = pKiL;
  kiR = pKiR;
}


/**
 * This function contains the PID closed loop controller
 */
void driveToWork(void)
{
  static int16_t speedLeft = 0;
  static int16_t speedRight = 0;
  static int16_t setValueL = 0;
  static int16_t setValueR = 0;

  //motorSetPwmLeft(0);
  //motorSetPwmRight(0);


  // pre control calculations
  //------------------------------------------------------------------
  // 2 tests with 100mm/s and 500mm/s
  sl[sli] = (uint8_t)valL;     // 100=18  500=45
  sr[sli++] = (uint8_t)valR;   // 100=16  500=42

  // y=m*x+n => preControl: val = setValue * m + n

  //              equation 1    equation 2
  // left motor:  100=18*m+n    500=45*m+n  =>  m=0.0675  n=11.25
  // right motor: 100=16*m+n    500=42*m+n  =>  m=0.065   n= 9.5
  //
  #define M_LEFT    68        // = m * 1000
  #define M_RIGHT   65        // = m * 1000
  #define N_LEFT    11
  #define N_RIGHT   10
  //------------------------------------------------------------------


  speedLeft = ((speedLeft * 1) + quadGetSpeedLeft())/2;
  speedRight = ((speedRight * 1) + quadGetSpeedRight())/2;

  if (setValueRight > setValueR) {                                        // accelerate right wheel
    setValueR += ACCELERATION_OFFSET;
    if (setValueRight < setValueR) setValueR = setValueRight;
  }
  if (setValueRight < setValueR) {                                        // decelerate right wheel
    setValueR -= DECELERATION_OFFSET;
    if (setValueRight > setValueR) setValueR = setValueRight;
  }

  if (setValueLeft > setValueL) {                                         // decelerate left wheel
    setValueL += ACCELERATION_OFFSET;
    if (setValueLeft < setValueL) setValueL = setValueLeft;
  }
  if (setValueLeft < setValueL) {                                         // decelerate left wheel
    setValueL -= DECELERATION_OFFSET;
    if (setValueLeft > setValueL) setValueL = setValueLeft;
  }

  if (setValueL)
  {
    devL = (setValueL - speedLeft);       // calc deviation: max devL = +2000 - -2000 = 4000
    valL = (kpL * devL);                  // P-Part: max (kpL * devL) = 1024000
    if (kiL) integL += devL;              // I-Part with anti-windup
    valL += (kiL * integL);
    valL += (kdL*(setValueL-devOldL));    // D-Part
    devOldL = setValueL;
    valL /= 1000;                         // scaling

    // pre control
    // y=m*x+n => preControl: setValue*m + n
    valL += (M_LEFT * setValueL) / 1000 + (setValueL > 0 ? N_LEFT : -N_LEFT);

    if (valL > MOTOR_MAX_VALUE) {
      valL = MOTOR_MAX_VALUE;
      integL -= devL;
    }
    else if (valL < -MOTOR_MAX_VALUE) {
      valL = -MOTOR_MAX_VALUE;
      integL += devL;
    }
  }
  else {
    valL = integL = devOldL = 0;
  }

  if (setValueR)
  {
    devR = (setValueR - speedRight);      // calc deviation
    valR = (kpR * devR);                  // P-Part: (max kpX = 2000 * 255 = 510'000)
    if (kiR) integR += devR;              // I-Part: with anti-windup below
    valR += (kiR * integR);
    valR += (kdR*(setValueR-devOldR));    // D-Part
    devOldR = setValueR;
    valR /= 1000;                         // scaling

    // pre control
    // y=m*x+n => preControl: setValue*m + n
    valR += (M_RIGHT * setValueR) / 1000 + (setValueR > 0 ? N_RIGHT : -N_RIGHT);

    if (valR > MOTOR_MAX_VALUE) {
      valR = MOTOR_MAX_VALUE;
      integR -= devR;                     // anti wind-up
    }
    else if (valR < -MOTOR_MAX_VALUE) {
      valR = -MOTOR_MAX_VALUE;
      integR += devR;                     // anti wind-up
    }
  }
  else {
    valR = integR = devOldR = 0;
  }

  motorSetPwmLeft((int8_t)valL);
  motorSetPwmRight((int8_t)valR);
}



void driveInit(void)
{
  kpL = kpR = 70;//80;
  kiL = kiR = 20;//30;
  kdL = kdR = 0;
  setValueLeft = setValueRight = 00;  //30... 7sec 30m = 4cm/sec
}
