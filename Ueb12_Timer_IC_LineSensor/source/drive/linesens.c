/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Linesensor
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          16.05.2020
 *
 *--------------------------------------------------------------------
 */

#include "platform.h"
#include "linesens.h"
#include "eeprom.h"
#if !SOLUTION

// todo #12.01 complete the macros to switch the IR diodes on/off
#define IR_L_ON()               (GPIOC->PCOR = 1<<6)  // clear output enables the IR diode (led is low active)
#define IR_L_OFF()              (GPIOC->PSOR = 1<<6)  // set output disables the IR diode.
#define IR_ML_ON()              (GPIOC->PCOR = 1<<7)
#define IR_ML_OFF()				(GPIOC->PSOR = 1<<7)
#define IR_M_ON()				(GPIOC->PCOR = 1<<15)
#define IR_M_OFF()				(GPIOC->PSOR = 1<<15)
#define IR_MR_ON()				(GPIOC->PCOR = 1<<17)
#define IR_MR_OFF()				(GPIOC->PSOR = 1<<17)
#define IR_R_ON()				(GPIOC->PCOR = 1<<18)
#define IR_R_OFF()				(GPIOC->PSOR = 1<<18)
#define IR_OFF()                (GPIOC->PSOR = (1<<6) | (1<<7) | (1<<15) | (1<<17) | (1<<18))

// todo #12.02 complete the macros to switch a pin configuration from GPIO to FTM-Mode and vice versa
#define PT_L_OUT_HIGH()         (PORTC->PCR[4] = PORT_PCR_MUX(1))
#define PT_L_INPUT_CAPTURE()    (PORTC->PCR[4] = PORT_PCR_MUX(4))
#define PT_ML_OUT_HIGH()		(PORTC->PCR[5] = PORT_PCR_MUX(1))
#define PT_ML_INPUT_CAPTURE()	(PORTC->PCR[5] = PORT_PCR_MUX(7))
#define PT_MR_OUT_HIGH()		(PORTD->PCR[6] = PORT_PCR_MUX(1))
#define PT_MR_INPUT_CAPTURE()	(PORTD->PCR[6] = PORT_PCR_MUX(4))
#define PT_R_OUT_HIGH()			(PORTD->PCR[7] = PORT_PCR_MUX(1))
#define PT_R_INPUT_CAPTURE()	(PORTD->PCR[7] = PORT_PCR_MUX(4))

// todo #12.03 determine the channels for the phototransistors
#define PT_L_CH_VALUE           (FTM0->CONTROLS[3].CnV)  // PTC4
#define PT_ML_CH_VALUE			(FTM0->CONTROLS[2].CnV)  // PTC5
#define PT_MR_CH_VALUE			(FTM0->CONTROLS[6].CnV)  // PTD6
#define PT_R_CH_VALUE			(FTM0->CONTROLS[7].CnV)  // PTD7

static uint16_t ticksL;
static uint16_t ticksML;
static uint16_t ticksMR;
static uint16_t ticksR;
static uint16_t ticksStart;
static uint16_t sensData[8];

/**
 * This ISR is called on a falling edge (input capture) of Q4.
 * It measures the charge time of the capacitor, clears the
 * interrupt flag and start the discharge of the capacitor.
 */
void FTM0CH3_IRQHandler(void)
{
  ticksL = FTM0->CONTROLS[3].CnV - ticksStart;       // calc the charge time
  FTM0->CONTROLS[3].CnSC &= ~FTM_CnSC_CHF_MASK;      // clear the channel interrupt flag
  PT_L_OUT_HIGH();                      // discharge the capacitor
}

/**
 * This ISR is called on a falling edge (input capture) of Q5.
 * It measures the charge time of the capacitor, clears the
 * interrupt flag and start the discharge of the capacitor.
 */
void FTM0CH2_IRQHandler(void)
{
  ticksML = FTM0->CONTROLS[2].CnV - ticksStart;
  FTM0->CONTROLS[2].CnSC &= ~FTM_CnSC_CHF_MASK;
  PT_ML_OUT_HIGH();
}

/**
 * This ISR is called on a falling edge (input capture) of Q6.
 * It measures the charge time of the capacitor, clears the
 * interrupt flag and start the discharge of the capacitor.
 */
void FTM0CH6_IRQHandler(void)
{
  // todo #12.07 implement the ISR as above
  ticksMR = FTM0->CONTROLS[6].CnV - ticksStart;
  FTM0->CONTROLS[6].CnSC &= ~FTM_CnSC_CHF_MASK;
  PT_MR_OUT_HIGH();
}

/**
 * This ISR is called on a falling edge (input capture) of Q7.
 * It measures the charge time of the capacitor, clears the
 * interrupt flag and start the discharge of the capacitor.
 */
void FTM0CH7_IRQHandler(void)
{
  // todo #12.08 implement the ISR as above
  ticksR = FTM0->CONTROLS[7].CnV - ticksStart;
  FTM0->CONTROLS[7].CnSC &= ~FTM_CnSC_CHF_MASK;
  PT_R_OUT_HIGH();
}


/**
 * This ISR is called periodically and scans the 8 spots
 *
 *
 *    *             *               *              *              *
 * [IR_L] [PT_L] [IR_ML] [PT_ML] [IR_M] [PT_MR] [IR_MR] [PT_R] [IR_R]
 *      \ /    \ /     \ /     \ /    \ /     \ /     \ /    \ /
 *       0      1       2       3      4       5       6      7
 *
 * state 3      0       0       1      1       2       2      3
 *
 */
void PIT0_IRQHandler(void)
{
  static uint8_t state = 0;
  PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;   // clear interrupt flag
  IR_OFF();                         // disable all IR-Diodes

  switch (state)
  {
  case 0:
    sensData[7] = ticksR;           // store the measured time of spot 8
    sensData[0] = ticksL;           // store the measured time of spot 1
    PT_L_INPUT_CAPTURE();           // prepare measure spot 2
    PT_ML_INPUT_CAPTURE();          // prepare measure spot 3
    IR_ML_ON();                     // enable IR diode => start the measure
    state++;
    break;

  case 1:
    sensData[1] = ticksL;           // store the measured time of spot 2
    sensData[2] = ticksML;          // store the measured time of spot 3
    PT_ML_INPUT_CAPTURE();          // prepare measure spot 4
    PT_MR_INPUT_CAPTURE();          // prepare measure spot 5
    IR_M_ON();                      // enable IR diode => start the measure
    state++;
    break;

  case 2:
    sensData[3] = ticksML;          // store the measured time of spot 4
    sensData[4] = ticksMR;          // store the measured time of spot 5
    PT_MR_INPUT_CAPTURE();          // prepare measure spot 6
    PT_R_INPUT_CAPTURE();           // prepare measure spot 7
    IR_MR_ON();                     // enable IR diode => start the measure
    state++;
    break;

  case 3:
    sensData[5] = ticksMR;          // store the measured time of spot 6
    sensData[6] = ticksR;           // store the measured time of spot 7
    PT_R_INPUT_CAPTURE();           // prepare measure spot 8
    PT_L_INPUT_CAPTURE();           // prepare measure spot 1
    IR_R_ON();                      // enable IR diode left and right
    IR_L_ON();                      // => start the measure
    state = 0;
    break;

  default: state = 0; break;
  }

  ticksStart = FTM0->CNT;            // determine the start time of the measure
}


/**
 * White calibration
 * Place the MC-Car on a white area before this function is called
 */
void lsCalibWhite(void)
{
  uint8_t i;
  for (i=0; i<8; i++)
  {
    config.lsCalibDataWhite[i] = sensData[i];
  }
  eepromSave();
}


/**
 * Black calibration
 * Place the MC-Car on a black area before this function is called
 */
void lsCalibBlack(void)
{
  uint8_t i;
  for (i=0; i<8; i++)
  {
    config.lsCalibDataBlack[i] = sensData[i];
  }
  eepromSave();
}


/**
 * Returns the white/black value of the 8 spots
 * LSB: the left sport, MSB: the right spot:
 *
 * @return
 *   the bit coded value of the 8 spots. '1'=black, '0'=white
 */
uint8_t lsGetSensData(void)
{
  uint8_t i;
  int16_t value;
  uint8_t result = 0;

  int16_t data[8];

  uint16_t min = 0xffff;
  uint16_t max = 0;
  uint32_t sum = 0;
  for (i=0; i<8; i++)
  {
    // remove offset
    value = sensData[i] - config.lsCalibDataWhite[i];
    if (value < 0) value = 0;

    // scale
    value = (uint16_t)((value * (uint32_t)1000) / config.lsCalibDataBlack[i]);
    data[i] = value;

    // determine min/max value
    if (value < min) min = value;
    if (value > max) max = value;
    sum += value;
  }
  uint16_t medium = sum / 8;
  int16_t level = medium + ((max - medium) / 4);

  for (i=0; i<8; i++)
  {
    if (data[i] > level) result |= (1<<i);
  }
  return result;
}


/**
 * Returns the deviation of the center: -7..+7
 * +7: the MC-Car is on the right side of the line
 * -7: the MC-Car is on the left side of the line
 *
 * @return
 *   the deviation from the center line
 */
int8_t lsGetDir(void)
{
  uint8_t value;
  int8_t i, v = 0;

  value = lsGetSensData();

  // only white or black spots found
  if (value == 0 || value == 255) return 127;

  // count white spots beginning from the left
  for (i=0; i<8; i++)
  {
    if (value & (1<<i)) break;
    v--;
  }

  // count white spots beginning from the right
  for (i=7; i>=0; i--)
  {
    if (value & (1<<i)) break;
    v++;
  }
  return v;
}


/**
 * Initializes the line sensor driver
 */
void lsInit(void)
{
  // todo #12.04 configure the 4 phototransistor pins as follows:
  // - pin muxing: GPIO
  // - gpio pin direction: output
  // - gpio pin level: high level (discharge capacitor)
  PT_L_OUT_HIGH(); PT_ML_OUT_HIGH(); PT_MR_OUT_HIGH(); PT_R_OUT_HIGH(); // pin muxing
  GPIOC->PDDR |= (1<<4) | (1<<5); GPIOD->PDDR |= (1<<6) | (1<<7); // as output
  GPIOC->PSOR |= (1<<4) | (1<<5); GPIOD->PSOR |= (1<<6) | (1<<7); // set pins to high


  // todo #12.05 configure the IR-Led pins as follows:
  // - pin muxing: GPIO
  // - gpio pin direction: output
  // - gpio pin level: high level (= LED off)
  PORTC->PCR[6] = PORT_PCR_MUX(1); PORTC->PCR[7] = PORT_PCR_MUX(1);  // pin muxing
  PORTC->PCR[15] = PORT_PCR_MUX(1); PORTC->PCR[17] = PORT_PCR_MUX(1);
  PORTC->PCR[18] = PORT_PCR_MUX(1);
  GPIOC->PDDR |= (1<<6) | (1<<7) | (1<<15) | (1<<17) | (1<<18);  // as output
  IR_L_OFF(); IR_ML_OFF(); IR_M_OFF(); IR_MR_OFF(); IR_R_OFF();  // disable leds

  // todo #12.06 configure each timer channel of the phototransistors as Input Capture on Falling Edge with Interrupt enable
  FTM0->CONTROLS[3].CnSC = FTM_CnSC_ELSx(2) | FTM_CnSC_MSx(0) | FTM_CnSC_CHIE(1);
  FTM0->CONTROLS[2].CnSC = FTM_CnSC_ELSx(2) | FTM_CnSC_MSx(0) | FTM_CnSC_CHIE(1);
  FTM0->CONTROLS[6].CnSC = FTM_CnSC_ELSx(2) | FTM_CnSC_MSx(0) | FTM_CnSC_CHIE(1);
  FTM0->CONTROLS[7].CnSC = FTM_CnSC_ELSx(2) | FTM_CnSC_MSx(0) | FTM_CnSC_CHIE(1);

  // configure Periodic Interrupt Timer (PIT)
  SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;               // configure clock gating
  PIT->MCR = PIT_MCR_MDIS(0);                     // enable PIT
  PIT->CHANNEL[0].LDVAL = 750000;                 // Timer Load Value (60e6MHz / 750000 => 12.5ms)
  PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TIE(1) | PIT_TCTRL_TEN(1);  // enable interrupt & PIT channel
  NVIC_SetPriority(PIT0_IRQn, PRIO_PIT0);         // set interrupt priority
  NVIC_EnableIRQ(PIT0_IRQn);                      // enable interrupt
}
#endif
