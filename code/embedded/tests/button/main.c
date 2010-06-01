#include <io.h>
#include <signal.h>
#include "clock.h"
#include "leds.h"
#include "button.h"

static int click();

int main(void) {
	WDTCTL = WDTPW + WDTHOLD;				 // Stop WDT
	
	clock_dco_set(8); // DCO @ 8MHz
	clock_mclk_set(CLOCK_SOURCE_DCO, 1); // MCLK @ 8MHz
	clock_smclk_set(CLOCK_SOURCE_DCO, 4); // SMCLK @ 2MHz
	clock_aclk_set(1); // ACLK @ 12kHz

	leds_init();
	leds_on(LEDS_ALL);

	button_init(HIGH_TO_LOW, click);
	eint();

	
	while (1) {
		if (P1IN & 0x4) {
			leds_on(LED_RED);
		} else {
			leds_off(LED_RED);
		}
	}
	return 0;
}

static int click() {
	leds_toggle(LED_GREEN);
	return 0;
}
