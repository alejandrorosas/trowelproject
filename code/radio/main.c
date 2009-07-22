#include <io.h>
#include <signal.h>
#include "clock.h"
#include "leds.h"
#include "spi.h"
#include "cc2500.h"

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
    eint();
    
    clock_dco_set(1); // DCO 1MHz
    clock_mclk_set(CLOCK_SOURCE_DCO, 1); // MCLK 1MHz
    clock_smclk_set(CLOCK_SOURCE_DCO, 2); // SMCLK 500kHz
    clock_smclk_set(CLOCK_SOURCE_DCO, 8); // SMCLK 125kHz
    clock_aclk_set(1);

    leds_init();
    leds_on(LED_RED);
    
    cc2500_init();    
    
    while (1) {
        LPM0;
    }
    
    return 0;
}
