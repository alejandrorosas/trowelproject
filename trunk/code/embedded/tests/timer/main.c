#include <io.h>
#include <signal.h>
#include "clock.h"
#include "leds.h"
#include "timer.h"

int change_red(void) {
    leds_toggle(LED_RED);
    return 0;
}

int change_green(void) {
    leds_toggle(LED_GREEN);
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
    leds_on(LED_RED);
    
    timer_start(SMCLK, DIV_4);

    timer_execute_several(change_red, 15625, -1);

    timer_execute_several(change_green, 7812, 15);
    
    while (1) {
        LPM0;
    }
    
    return 0;
}
