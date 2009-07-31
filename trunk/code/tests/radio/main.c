#include <io.h>
#include <signal.h>
#include "clock.h"
#include "leds.h"
#include "csma.h"
#include "timer.h"

volatile int event = 0;

int change_red(void) {
    leds_toggle(LED_RED);
    event = 1;
    return 1;
}

int change_green() {
    leds_toggle(LED_GREEN);
    return 0;
}

int main(void) {
    
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    eint();
    
    clock_dco_set(8); // DCO 8MHz
    clock_mclk_set(CLOCK_SOURCE_DCO, 1); // MCLK 8MHz
    clock_smclk_set(CLOCK_SOURCE_DCO, 8); // SMCLK 1MHz
    clock_aclk_set(1);
    
    leds_init();
    leds_off(LEDS_ALL);
    
    csma_init();
    csma_set_tx_cb(change_green);
    
    timer_start(TIMER_SOURCE_ACLK, 1);
    timer_register_cb(TIMER_ALARM_0, change_red);
    timer_set_alarm(TIMER_ALARM_0, 12000, 12000, TIMER_MODE_FROM_NOW, 0);
    
    leds_on(LEDS_ALL);
    
    uint8_t msg[14] = "hello";
    while (1) {
        event = 0;
        LPM0;
        if (event == 1) {
            csma_send(CSMA_BROADCAST, msg, 5);
        }
    }
    
    return 0;
}
