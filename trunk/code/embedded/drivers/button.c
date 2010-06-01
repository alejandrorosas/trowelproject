#include <io.h>
#include <signal.h>
#include "button.h"


#define BUTTON_PIN 2

static button_cb_t button_cb = 0x0;

void button_init(enum button_edge edge, button_cb_t cb) {
	// Set input with pullup
	P1SEL &= ~BV(BUTTON_PIN);
	P1DIR &= ~BV(BUTTON_PIN);
	P1REN |= BV(BUTTON_PIN);
	P1OUT |= BV(BUTTON_PIN);

	// Set edge
	if (edge == HIGH_TO_LOW) {
		P1IES |= BV(BUTTON_PIN);
	} else {
		P1IES &= ~BV(BUTTON_PIN);
	}

	// Clear and enable interrupt
	P1IFG &= ~BV(BUTTON_PIN);
	P1IE |= BV(BUTTON_PIN);

	button_cb = cb;
}

void button_stop(void) {
	P1IE &= ~BV(BUTTON_PIN);
}

void port1irq(void);
/**
 * Interrupt service routine for PORT2.
 * Used for handling CC2500 interrupts triggered on
 * the GDOx pins.
 */
interrupt(PORT1_VECTOR) port1irq(void) {
	if (P1IFG & BV(BUTTON_PIN)) {
		P1IFG &= ~BV(BUTTON_PIN);
		
		if (button_cb && button_cb()) {
			LPM4_EXIT;
		}
	}
}
