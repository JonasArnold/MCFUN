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

/**
 * Toggles PTC8 (Led green Front Left) every 500 ms
 */
void main(void) {

	uint8_t ledState = 1;

	/* GPIO */
	// Pin Mux Control would need to be set per output (GPIO C)
	PORTC->PCR[8] = PORT_PCR_MUX(1);

	// Enable pull up resistors (PE = Pull Enable, PS = Pull Select (0 = down, 1 = up)) (mux = 1 => pin is gpio)
	PORTB->PCR[1] |= PORT_PCR_PE(1) | PORT_PCR_PS(1) | PORT_PCR_MUX(1);

	/* TIMER */
	// Start FTM0 with fixed Frequency 250kHz, Set Prescaler to 16
	FTM0->SC = FTM_SC_CLKS(2) | FTM_SC_PS(4);

	/* Endless */
	while (true) {
		FTM0->MOD = 7812; // reset count of FTM0 (7812 = 500ms)
		GPIOC->PDDR = (ledState << 8);  // write led State to port C

		while ((FTM0->SC & FTM_SC_TOF_MASK) == 0) { }  // wait for TOF

		ledState = ~ledState;
	}

    // Never leave main
	for (;;) {}
}

#endif
