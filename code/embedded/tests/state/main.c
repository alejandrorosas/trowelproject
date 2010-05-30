#include <io.h>
#include "clock.h"
#include "leds.h"
#include "state.h"
#include "event.h"

volatile unsigned int i;                    // volatile to prevent optimization

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

    clock_dco_set(1); // DCO 1MHz
    clock_mclk_set(CLOCK_SOURCE_DCO, 8); // MCLK 125kHz
    clock_smclk_set(CLOCK_SOURCE_DCO, 4);
    clock_aclk_set(1);

    leds_init();
    leds_on(LEDS_ALL);

	event_init();
	st_init();

	i = 65535;
	do
		(i--);
	while (i != 0);
	
	first_init();

	
	while (1) {
		event_process();

	}
    
    return 0;
}
