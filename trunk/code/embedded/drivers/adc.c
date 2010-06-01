#include <io.h>
#include <signal.h>
#include "adc.h"

static adc_cb_t adc_temp_cb = 0x0;

void adc_init(void) {
	ADC10CTL0 = SREF_VREF_AVSS | ADC10SHT_DIV64 | REFON | ADC10SR | ADC10ON | ADC10IE;
	ADC10CTL1 = INCH_TEMP | ADC10DIV_3;
}


void adc_stop(void) {
	ADC10CTL0 = 0;
}

void adc_sample_temp(adc_cb_t cb) {
	ADC10CTL0 |= ENC + ADC10SC;
	adc_temp_cb = cb;
}

void adc10irq(void);
interrupt (ADC10_VECTOR) adc10irq(void) {
	if ( (adc_temp_cb!=0x0) && adc_temp_cb(ADC10MEM))
		LPM4_EXIT;

	adc_temp_cb = 0x0;
}
