/*
 * The main concept of this lib is to get the real tick rate of the ACLK
 * clock, that is the internal 12kHz VLO. To obtain this, we use the
 * cc2500 chip which will output a precise given frequency, and compare
 * it with the VLO.
 * 
 * The required steps are: 
 *  - configure the cc2500 to output a given frequency on gdo0
 *  - start the timerB module in continuous mode
 *  - count the cc2500 clock ticks for 1 second
 *  - stop the timerB and read the timer value
 *  - call the callback
 */

#include <io.h>
#include "cc2500.h"
#include "vlo_sync.h"

/**
 * Callback function called when a radio output clock tick
 * happens.
 */
static int radio_tick(void);

static const uint32_t ticks_second = CC2500_XOSC_FREQ / 128;
static uint32_t radio_tick_count;
static vlo_cb_t measure_done_cb;

void vlo_get_frequency(vlo_cb_t cb) {
	// store the callback
	measure_done_cb = cb;
	
	// init the radio
	cc2500_init(0x0, 0);
	
	// configure GDO0 to output CLK_XOSC / 128 = 26e6/128 = 203125Hz
	cc2500_write_reg(CC2500_REG_IOCFG0, 0x3E);
	
	// set callback
	cc2500_gdo0_int_set_cb(radio_tick);
	cc2500_gdo0_int_set_rising();
	
	// init timerB
	TBCTL = TBCLR;
	TBCTL |= TBSSEL_ACLK | ID_DIV1;
	TBR = 0;
	
	// reset overflow count and radio tick count
	radio_tick_count = 0;
	
	// clear and start radio interrupt
	cc2500_gdo0_int_clear();
	cc2500_gdo0_int_enable();
	
	// start timer
	TBCTL |= MC_CONT;
}

int radio_tick(void) {
	// increment the count
	radio_tick_count++;
	// check if done
	if (radio_tick_count == ticks_second) {
		uint16_t aclk_tick_count;
		
		// stop timer
		TBCTL &= ~MC_CONT;
		
		// store the TBR value;
		aclk_tick_count = TBR;
		
		// stop the radio interrupt, and reset default
		cc2500_gdo0_int_disable();
		cc2500_write_reg(CC2500_REG_IOCFG0, 0x3F);
		
		// if callback not null, call it
		if (measure_done_cb) {
			return measure_done_cb(aclk_tick_count);
		}
	}
	return 0;
}


