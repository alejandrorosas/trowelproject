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
    clock_smclk_set(CLOCK_SOURCE_DCO, 2); // SMCLK 500kHz
    clock_smclk_set(CLOCK_SOURCE_DCO, 8); // SMCLK 125kHz
    clock_aclk_set(1);

    leds_init();
    leds_on(LED_RED);
    
    timer_start(TIMER_SOURCE_SMCLK, 8); // 
    timer_register_cb(TIMER_ALARM_0, change_red);
    timer_set_alarm(TIMER_ALARM_0, 0, 15625, TIMER_MODE_FROM_NOW, 0);
    timer_register_cb(TIMER_ALARM_1, change_green);
    timer_set_alarm(TIMER_ALARM_1, 0, 7812, TIMER_MODE_FROM_NOW, 0);
    
    while (1) {
        LPM0;
    }
    
    return 0;
}
