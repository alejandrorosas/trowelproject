#include <io.h>
#include <signal.h>
#include "clock.h"
#include "pwm.h"
#include "timer.h"
#include "leds.h"
#include "adc.h"

static int data_ready(uint16_t value);
static int launch_adc();

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    eint();
    
    clock_dco_set(8); // DCO @ 1MHz
    clock_mclk_set(CLOCK_SOURCE_DCO, 1); // MCLK @ 8MHz
    clock_smclk_set(CLOCK_SOURCE_DCO, 4); // SMCLK @ 2MHz
    clock_aclk_set(1); // ACLK @ 12kHz

    leds_init();
    leds_off(LEDS_ALL);

    pwm_period_set(PWM_SOURCE_SMCLK, 1, 40000); // PWM clock 2, period = 20ms
    // 1.5ms = 
    pwm_out1_set(3000);
    pwm_out2_set(20000);
LPM0;
    adc_init();
    adc_register_callback(data_ready);

    timer_start(TIMER_SOURCE_ACLK, 1);
    timer_set_alarm(TIMER_ALARM_0, 3000, 120, TIMER_MODE_FROM_NOW, 0);
    timer_register_cb(TIMER_ALARM_0, launch_adc);

    while (1) LPM0;

    return 0;
}

static int data_ready(uint16_t value) {
    pwm_out1_set(2000+(value<<1));
    pwm_out2_set(40*value);
    leds_toggle(LED_GREEN);
    return 0;
}

static int launch_adc(void) {
    ADC10CTL0 |= ENC + ADC10SC;
    leds_toggle(LED_RED);
    return 0;
}
