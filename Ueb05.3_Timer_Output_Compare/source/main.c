/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Exercise 05.1 - Poling, Interrupts & Timer
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          10.03.2020
 *
 *--------------------------------------------------------------------
 */
#include "platform.h"
#if !SOLUTION

#define TICKS_500MS	(15624)

// flex timer 3 ISR
void FTM3_IRQHandler(void)
{
	if(FTM3->CONTROLS[4].CnSC & FTM_CnSC_CHF_MASK)  // is compare interrupt ?
	{
		// clear CHF flag (=> hw is ready to get new interrupt)
		FTM3->CONTROLS[4].CnSC &= ~FTM_CnSC_CHF_MASK;
		// add value to terminate next point of interrupt
		FTM3->CONTROLS[4].CnV = FTM3->CNT + TICKS_500MS;
	}
}

/**
 * Toggles PTC8 (Led green Front Left) every 500 ms
 * Clock used: fixed frequency with prescaler 8
 *
 * Fixed frequency clock: MCGFFCLK p113, p542/p152
 */
void main(void) {
	/* GPIO */
	// set PTC8 to output
	GPIOC->PDDR = (1 << 8);
	// Pin Mux Control would need to be set to FTM3_CH4 (GPIO C) => Alt 3
	PORTC->PCR[8] = PORT_PCR_MUX(3);

	/* Interrupt */
	// Enable interrupt
	NVIC_SetPriority(FTM3_IRQn, 8);
	NVIC_EnableIRQ(FTM3_IRQn);

	/* TIMER */
	// Enable clock
	SIM->SCGC6 |= SIM_SCGC6_FTM3_MASK;

	// Set modulo of Timer 3 to max (0xFFFF)
	FTM3->MOD = 0xFFFF;

	FTM3->CONTROLS[4].CnV = TICKS_500MS;  // set compare value (for first match = value - 0)
	FTM3->CONTROLS[4].CnSC |= FTM_CnSC_ELSB(0) | FTM_CnSC_ELSA(1);   // toggle on match
	FTM3->CONTROLS[4].CnSC |= FTM_CnSC_MSB(0) | FTM_CnSC_MSA(1);   // output compare mode
	FTM3->CONTROLS[4].CnSC |= FTM_CnSC_CHIE(1);  // enable interrupt on compare

	// Start FTM0 with fixed Frequency 250kHz, Set Prescaler to 8, Enable overflow interrupt
	FTM3->SC = FTM_SC_CLKS(2) | FTM_SC_PS(3) | FTM_SC_TOIE_MASK;

    // Never leave main
	for (;;) {}
}

#endif
