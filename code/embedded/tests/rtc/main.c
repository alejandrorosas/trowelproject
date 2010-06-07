#include <io.h>
#include <signal.h>
#include "clock.h"
#include "leds.h"
#include "rtc.h"

int change_red(void) {
    leds_toggle(LED_RED);
    return 0;
}

struct rtc_alarm alarm;

int exec_alarm(void) {
	leds_toggle(LED_RED);
	alarm.date = rtc_time() + 5;
	alarm.exec_cb = exec_alarm;
	
	rtc_set_alarm(&alarm);
	
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
    
    // init the rtc
    rtc_init();
    
    exec_alarm();
    
    while (1) {
        LPM0;
    }
    
    return 0;
}
