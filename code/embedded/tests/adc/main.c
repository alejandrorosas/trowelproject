#include <io.h>
#include <signal.h>
#include "clock.h"
#include "timer.h"
#include "leds.h"
#include "adc.h"

static int data_ready(uint16_t value);
static int launch_adc();

int main(void) {
	WDTCTL = WDTPW + WDTHOLD;				 // Stop WDT
	eint();
	
	clock_dco_set(8); // DCO @ 8MHz
	clock_mclk_set(CLOCK_SOURCE_DCO, 1); // MCLK @ 8MHz
	clock_smclk_set(CLOCK_SOURCE_DCO, 4); // SMCLK @ 2MHz
	clock_aclk_set(1); // ACLK @ 12kHz

	leds_init();
	leds_off(LEDS_ALL);

	adc_init();
	timer_init();
	timer_start(ACLK, 1);
	timer_execute_several(launch_adc, 12000, -1);

	while (1) LPM0;

	return 0;
}

static int data_ready(uint16_t value) {
	leds_toggle(LED_GREEN);
	return 0;
}

static int launch_adc(void) {
	leds_toggle(LED_RED);
	adc_sample_temp(data_ready);
	return 0;
}
