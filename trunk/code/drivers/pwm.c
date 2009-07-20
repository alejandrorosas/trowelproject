#include <io.h>
#include "pwm.h"

void pwm_period_set(int source, int div, uint16_t period) {
    P4DIR |= 0x30; // P4.4 - P4.5 output
    P4SEL |= 0x30; // P4.5 - P4.5 TBx options
    
    TBCCR0 = period - 1; // PWM Period
    TBCCTL1 = CLLD_1 | OUTMOD_RESET_SET; // TBCCR1 reset/set
    TBCCR1 = period >> 1; // TBCCR1 PWM duty cycle
    TBCCTL2 = CLLD_1 | OUTMOD_RESET_SET;
    TBCCR2 = period >> 1;
    
    TBCTL = 0;
    
    switch (source) {
        case PWM_SOURCE_SMCLK:
            TBCTL |= TBSSEL_SMCLK;
            break;
        case PWM_SOURCE_ACLK:
            TBCTL |= TBSSEL_ACLK;
            break;
    }
    
    switch (div) {
        case 1:
            TBCTL |= ID_DIV1;
            break;
        case 2:
            TBCTL |= ID_DIV2;
            break;
        case 4:
            TBCTL |= ID_DIV4;
            break;
        case 8:
            TBCTL |= ID_DIV8;
            break;
    }
    
    
    
    TBCTL |= MC_UPTO_CCR0; // UP mode
}

void pwm_out1_set(uint16_t high_time) {
    TBCCR1 = high_time;
}

void pwm_out2_set(uint16_t high_time) {
    TBCCR2 = high_time;
}

void pwm_stop() {
    TBCTL &= ~MC_3;
}
