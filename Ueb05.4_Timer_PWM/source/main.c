/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Exercise 05.4 - Timer PWM (TESTAT)
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          10.03.2020
 *
 *--------------------------------------------------------------------
 */
#include "platform.h"
#if !SOLUTION

#define PWM_VALUE	((0xFFFF*1)/100) // 40 percent

/**
 * PWM for LED (Led green Front Left)
 *
 * Fixed frequency clock: MCGFFCLK p113, p542/p152
 */
void main(void) {
	/* TIMER */
	// Enable clock
	SIM->SCGC6 |= SIM_SCGC6_FTM3_MASK;

	// Start FTM3 with fixed Frequency 250kHz, Set Prescaler to 8
	FTM3->SC = FTM_SC_CLKS(2) | FTM_SC_PS(3) | FTM_SC_CPWMS(0) ;

	/* GPIO */
	// Pin Mux Control would need to be set to FTM3_CH4 (GPIO C) => Alt 3
	PORTC->PCR[8] = PORT_PCR_MUX(3);
	PORTC->PCR[11] = PORT_PCR_MUX(3);

	FTM3->MOD = 100;
	FTM3->CONTROLS[4].CnSC = FTM_CnSC_ELSA(1) | FTM_CnSC_MSB(1);  // pwm mode
	FTM3->CONTROLS[4].CnV = 10;  // pwm value

	FTM3->CONTROLS[7].CnSC = FTM_CnSC_MSB(1) | FTM_CnSC_ELSA(1);
	FTM3->CONTROLS[7].CnV = 60;

    // Never leave main	for (;;) {}
}

#endif
