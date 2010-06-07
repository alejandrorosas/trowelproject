#include <io.h>
#include <signal.h>
#include "clock.h"
#include "leds.h"
#include "timer.h"
#include "vlo_sync.h"

int change_red(void) {
    leds_toggle(LED_RED);
    return 0;
}

int period_measured(uint16_t freq) {
	// start the timer at 1Hz
	timer_execute_several(change_red, freq, TIMER_COUNT_INFINITE);
	return 0;
}

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
    eint();
    
    clock_dco_set(1); // DCO 1MHz
    clock_mclk_set(CLOCK_SOURCE_DCO, 1); // MCLK 1MHz
    clock_smclk_set(CLOCK_SOURCE_DCO, 8); // SMCLK 125kHz
    clock_aclk_set(1);

    leds_init();
    leds_on(LEDS_ALL);
    
    // start the timer on ACLK
    timer_start(ACLK, DIV_1);

	// start measuring the period
	vlo_get_frequency(period_measured);

    while (1) {
        LPM0;
    }
    
    return 0;
}
