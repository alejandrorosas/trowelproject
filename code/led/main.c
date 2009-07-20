#include <io.h>
#include "clock.h"
#include "leds.h"

volatile unsigned int i;                    // volatile to prevent optimization

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

    clock_dco_set(1); // DCO 1MHz
    clock_mclk_set(CLOCK_SOURCE_DCO, 8); // MCLK 125kHz
    clock_smclk_set(CLOCK_SOURCE_DCO, 4);
    clock_aclk_set(1);

    leds_init();
    leds_on(LEDS_ALL);
    for (;;) {
        i = 50000;
        do
            (i--);
        while (i != 0);
        
        leds_toggle(LEDS_ALL);
    }
    return 0;
}
