/**
 *--------------------------------------------------------------------\n
 *          HSLU T&A Hochschule Luzern Technik+Architektur            \n
 *--------------------------------------------------------------------\n
 *
 * \brief         Exercise 02 - GPIO C
 * \file
 * \author        Christian Jost, christian.jost@hslu.ch
 * \date          18.02.20018
 *
 *--------------------------------------------------------------------
 */
#include "platform.h"

#if !SOLUTION

/**
 * Application entry point.
 */
int main(void)
{
	uint32_t data;

	// Direction data register (DDR) (1 = input)
	GPIOC->PDDR |= (1 << 8) | (1 << 9) | (1 << 10);

	// Pin Mux Control would need to be set per output (GPIO C)
	PORTC->PCR[8] = PORT_PCR_MUX(1);
	PORTC->PCR[9] = PORT_PCR_MUX(1);
	PORTC->PCR[10] = PORT_PCR_MUX(1);

	// Enable pull up resistors (PE = Pull Enable, PS = Pull Select (0 = down, 1 = up)) (mux = 1 => pin is gpio)
	PORTB->PCR[1] |= PORT_PCR_PE(1) | PORT_PCR_PS(1) | PORT_PCR_MUX(1);
	PORTB->PCR[2] |= PORT_PCR_PE(1) | PORT_PCR_PS(1) | PORT_PCR_MUX(1);
	PORTB->PCR[3] |= PORT_PCR_PE(1) | PORT_PCR_PS(1) | PORT_PCR_MUX(1);

	for (;;) {
		data = GPIOB->PDIR;  // read data register from port B
		data <<= 7;         // shift up
		data = ~data;       // led lit when 0 => therefore invert
		GPIOC->PDDR = data;  // write data to port C
		// it is ok to set all 32bits because data direction was set for the 3 relevant pins
		// it is ok to set bits that are defined as inputs
	}

	// Never leave main
	while (1);
}

#endif
