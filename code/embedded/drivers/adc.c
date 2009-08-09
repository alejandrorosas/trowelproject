#include <io.h>
#include <signal.h>
#include "adc.h"

static adc_cb_t adc_cb = 0x0;

void adc_init(void) {
    ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE; // ADC10ON, interrupt enabled
    ADC10AE0 |= 0x01;                         // P2.0 ADC option select
}

void adc_start(void) {
    ADC10CTL0 |= ENC + ADC10SC;             // Sampling and conversion start
}

void adc_stop(void) {
    
}
void adc_register_callback(adc_cb_t cb) {
    adc_cb = cb;
}

void adc10irq(void);
interrupt (ADC10_VECTOR) adc10irq(void) {
    if ( (adc_cb!=0x0) && adc_cb(ADC10MEM))
        LPM4_EXIT;
}
