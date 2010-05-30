#include <io.h>
#include <signal.h>
#include "clock.h"
#include "leds.h"
#include "state.h"
#include "event.h"
#include "timer.h"

volatile unsigned int i;                    // volatile to prevent optimization

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
    eint();

    clock_dco_set(1); // DCO 1MHz
    clock_mclk_set(CLOCK_SOURCE_DCO, 8); // MCLK 125kHz
    clock_smclk_set(CLOCK_SOURCE_DCO, 4);
    clock_aclk_set(1); // 12 kHz

    leds_init();

	timer_init();
	timer_start(ACLK, 1);

	event_init();
	st_init();

	first_init();

	while (1) {
		event_process();
	}
    
    return 0;
}
