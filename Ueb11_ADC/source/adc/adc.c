/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         ADC-Driver for the MC-Car
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          09.05.2020
 *
 *--------------------------------------------------------------------
 */

#include "platform.h"
#include "adc.h"
#include "util.h"
#if !SOLUTION

// todo #11.01 determine the ADC1 channels
// see KRM99ff
#define ADC_CH_TEMPERATURE        26 // x
#define ADC_CH_CURRENT            19 // y
#define ADC_CH_BAT_VOLTAGE        0 // z

#define REFERENCE_VOLTAGE    1195000  // 1'195'000uV = 1.195V
#define VTEM25 				 716000   // 716'000uV = 716mV
#define TEMP_SENSOR_SLOPE    162      // 1620 uV/°C = 1.62mV/°C (10th of degrees needed) => div by 10


/**
 * Performs one A/D conversion for the specified channel with 16 bit resolution.
 * The function blocks until the conversion has been finished.
 *
 * @param[in]
 *   the channel number to convert
 * @return
 *   a 16bit value
 */
uint16_t adcGet16BitValue(uint8_t channel)
{
  // todo #11.06 implement the function code as follows:
  // 1. start a conversion
  ADC1->SC1[0] &= ADC_SC1_ADCH(channel);  // = ADC1_SC1A  &= because default of ADCH is 1111

  // 2. wait until conversion has been finished
  while((ADC1->SC1[0] & ADC_SC1_COCO_MASK) > 0);

  // 3. return the result.
  return ADC1->R[0];
}


/**
 * Performs a A/D conversation and returns the measured voltage of this channel.
 * The maximum voltage is the REFERENCE_VOLTAGE defined above.
 *
 * @param[in]
 *   the channel number to convert
 * @return
 *   the measured voltage in uV (0..REFERENCE_VOLTAGE)
 */
uint32_t adcGetVoltage(uint8_t channel)
{
  // todo #11.07 calculate the measured voltage
  // 1. perform a conversion of the desired channel
  uint16_t voltageAdc = adcGet16BitValue(channel);

  // 2. calculate and return the voltage in uV.
  uint64_t uV = ((uint64_t)REFERENCE_VOLTAGE * voltageAdc)/ 65535;
  return (uint32_t)uV;
}


/**
 * Returns the power consumption of the MC-Car
 *
 *           Ushunt [mV]        Ushunt
 * I [mA] = -------------- = ------------ x Yshunt
 *           Rshunt [Ohm]     100 * 1000
 *
 * @return
 *   the power consumption in mA
 */
uint16_t adcGetCurrent(void)
{
  uint32_t uShunt = adcGetVoltage(ADC_CH_CURRENT);
  uint16_t iShunt = uShunt * 68 / (1000 * 100) - 15;
  return iShunt;
}


/**
 * Reads and returns the temperature of the microcontroller
 *
 * @returns
 *   the temperature in 0.1°C => 237 = 23.7°C
 */
int16_t adcGetTemperature(void)
{
  // todo #11.08 determine the temperature of the microcontroller
  // 1. perform a adc conversion
  uint32_t temp = adcGetVoltage(ADC_CH_TEMPERATURE);

  // 2. calculate the temperature. KRM804 & MK22 Datasheet p42 (250 because of 10th of degree needed)
  return 250 - ((int16_t)(temp - VTEM25) / TEMP_SENSOR_SLOPE);
}


/**
 * Returns the battery voltage
 *
 * @return
 *   the battery voltage in mV
 */
uint16_t adcGetBatVoltage(void)
{
  // todo #11.09 determine the battery voltage
  // 1. perform a adc conversion
  uint32_t batVoltage = adcGetVoltage(ADC_CH_BAT_VOLTAGE);
  // 2. calculate the voltage (see also the MC-Car schematic)
  return (2*batVoltage)/1000;
}

/**
 * Initializes the ADC1.
 */
void adcInit(void)
{
  uint16_t calib;

  // init voltage reference module
  SIM->SCGC4 |= SIM_SCGC4_VREF_MASK;          // clock gating
  VREF->TRM |= VREF_TRM_CHOPEN(1);            // Chop oscillator enable
  VREF->SC = VREF_SC_VREFEN(1)                // internal voltage reference enable
      | VREF_SC_ICOMPEN(1)                    // second order curvature compensation enable
      | VREF_SC_MODE_LV(1);                   // buffer mode: high power
  utilWaitUs(1);                              // Wait >300ns
  VREF->SC |= VREF_SC_REGEN(1);               // 1.75V regulator enable
  utilWaitUs(35000);                          // Wait 35ms

  // init adc
  // todo #11.02 configure clock gating
  SIM->SCGC6 |= SIM_SCGC6_ADC1_MASK;

  // todo #11.03 configure adc as follows:
  // set the ADCK to 7.5 MHz using the busClock (60MHz)
  // and configure 16 bit conversion with long sample time
  ADC1->CFG1 = ADC_CFG1_ADICLK(0)   // clock = busclick
		     | ADC_CFG1_ADIV(3)     // Div 8 => ADCK = 60MHz/8 = 7.5MHz
			 | ADC_CFG1_ADLSMP(1)   // Long sample time
			 | ADC_CFG1_MODE(3);    // 16bit conversion
  ADC1->CFG2 = 0;

  // todo #11.04 select the alternate reference source (VREF)
  ADC1->SC2 |= ADC_SC2_REFSEL(0x01);   // alternate reference source (VREF)

  // todo #11.05 configure hardware average of 32 samples
  ADC1->SC3 |= (ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(0x03));   // hardware average enable, 32 samples averaged

  // adc calibration
  ADC1->SC3 |= ADC_SC3_CAL(1);                 // Start calibration
  while (!(ADC1->SC1[0] & ADC_SC1_COCO_MASK)){}  // wait until calibration has been finished

  // set calib data
  calib = ADC1->CLP0 + ADC1->CLP1 + ADC1->CLP2 + ADC1->CLP3 + ADC1->CLP4 + ADC1->CLPS;
  calib = calib / 2;
  calib = calib | 0x8000;
  ADC1->PG = calib;

  calib = ADC1->CLM0 + ADC1->CLM1 + ADC1->CLM2 + ADC1->CLM3 + ADC1->CLM4 + ADC1->CLMS;
  calib = calib / 2;
  calib = calib | 0x8000;
  ADC1->MG = calib;
}
#endif
