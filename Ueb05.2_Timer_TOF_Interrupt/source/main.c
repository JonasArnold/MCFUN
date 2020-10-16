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

uint8_t ledState = 1;

// flex timer 0 ISR
void FTM0_IRQHandler(void)
{
	ledState = ~ledState;
	GPIOC->PDDR = (ledState << 8);  // write led State to port C

	// TODO: Reset Flag??
}

/**
 * Toggles PTC8 (Led green Front Left) every 500 ms
 */
void main(void) {
	/* GPIO */
	// Pin Mux Control would need to be set per output (GPIO C)
	PORTC->PCR[8] = PORT_PCR_MUX(1);

	// Enable pull up resistors (PE = Pull Enable, PS = Pull Select (0 = down, 1 = up)) (mux = 1 => pin is gpio)
	PORTB->PCR[1] |= PORT_PCR_PE(1) | PORT_PCR_PS(1) | PORT_PCR_MUX(1);

	/* TIMER */
	// Reset Counter of Timer;
	FTM0->CNT = 0;

	// Set modulo of Timer 0 (end value) (7812 = 500ms)
	FTM0->MOD = 7812;

	// Start FTM0 with fixed Frequency 250kHz, Set Prescaler to 16, Enable overflow interrupt
	FTM0->SC = FTM_SC_CLKS(2) | FTM_SC_PS(4) | FTM_SC_TOIE_MASK;

    // Never leave main
	for (;;) {}
}

#endif
