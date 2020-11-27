/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Power-Switch helper functions
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          01.05.2020
 *
 *--------------------------------------------------------------------
 */

#ifndef SOURCES_UTILS_PWRSWITCH_H_
#define SOURCES_UTILS_PWRSWITCH_H_



/**
 * Initializes the Power Switch
 * Mux: GPIO
 * PE: Pull-Enable
 * PS: Pull-Up
 */
#define pwrSwitchInit()     (PORTE->PCR[0] = PORT_PCR_MUX(1) | PORT_PCR_PE(1) | PORT_PCR_PS(1))


/**
 * Returns the state of the PowerSwitch
 * 0: Power Switch is in the default position
 * 1: Power Switch is enabled
 */
#define pwrSwitchEnabled()  (!(GPIOE->PDIR & (1<<0)))



#endif /* SOURCES_UTILS_PWRSWITCH_H_ */
