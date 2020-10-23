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

// flex timer 0 ISR
void FTM0_IRQHandler(void)
{
	GPIOC->PTOR = (1 << 8);        // toggle LED (Toggle Register)
	FTM0->SC &= ~FTM_SC_TOF_MASK;  // clear TOF
}

/**
 * Toggles PTC8 (Led green Front Left) every 500 ms
 * Clock used: fixed frequency with prescaler 8
 */
void main(void) {
	/* GPIO */
	// set PTC8 to output
	GPIOC->PDDR = (1 << 8);
	// Pin Mux Control would need to be set per output (GPIO C)
	PORTC->PCR[8] = PORT_PCR_MUX(1);

	/* INTERRUPT */
	NVIC_SetPriority(FTM0_IRQn, 8);
	NVIC_EnableIRQ(FTM0_IRQn);

	/* TIMER */
	// Enable clock
	SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;

	// Set modulo of Timer 0 (end value) (7812 = 500ms)
	FTM0->MOD = TICKS_500MS;

	// Start FTM0 with fixed Frequency 250kHz, Set Prescaler to 8, Enable overflow interrupt
	FTM0->SC = FTM_SC_CLKS(2) | FTM_SC_PS(3) | FTM_SC_TOIE_MASK;

    // Never leave main
	for (;;) {}
}

#endif
