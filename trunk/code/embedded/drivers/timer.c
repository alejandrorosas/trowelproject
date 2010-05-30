#include <io.h>
#include <signal.h>
#include "timer.h"

#define TIMER_ALARM_NUMBER 3

static int16_t get_available_slot();

struct timer_alarm {
	int8_t active, period_count;
	int16_t period;
	timer_cb function;
};

static struct timer_alarm timer_alarms[TIMER_ALARM_NUMBER];
static uint16_t *TACCTLx = (uint16_t*) 0x162;
static uint16_t *TACCRx  = (uint16_t*) 0x172;

static uint16_t master_time;

void timer_init() {
	uint16_t i;
	
	// stop everything
	TACTL = TACLR;
	
	// clear the CCR and CCTL registers, and the associated callbacks
	for (i=0;i<TIMER_ALARM_NUMBER;i++) {
		TACCTLx[i] = 0;
		TACCRx[i] = 0;
		timer_alarms[i].active = 0;
		timer_alarms[i].period_count = 0;
		timer_alarms[i].period = 0;
		timer_alarms[i].function = 0x0;
	}
}

uint16_t timer_start(enum timer_source source, enum timer_div div) {
	// clear
	TACTL = TACLR;

	// set clock source
	switch (source) {
		case ACLK:
			TACTL |= TASSEL_ACLK;
			break;
		case SMCLK:
			TACTL |= TASSEL_SMCLK;
			break;
		default:
			return 0;
	}

	// set clock division
	switch (div) {
		case DIV_1:
			TACTL |= ID_DIV1;
			break;
		case DIV_2:
			TACTL |= ID_DIV2;
			break;
		case DIV_4:
			TACTL |= ID_DIV4;
			break;
		case DIV_8:
			TACTL |= ID_DIV8;
			break;
		default:
			return 0;
	}
	
	// start timer
	TACTL |= MC_CONT;

	// set overflow interrupt for mastertime
	TACTL |= TAIE;

	// reset master time
	master_time = 0;
	
	return 1;
}

uint16_t timer_time() {
	return TAR;
}

uint16_t timer_mastertime() {
	return master_time;
}

int16_t timer_execute_once(timer_cb function, uint16_t wait) {
	int16_t alarm_id;
	uint16_t now;
	now = TAR;

	alarm_id = get_available_slot();

	if (alarm_id < 0) {
		return -1;
	}
	
	TACCRx[alarm_id] = now + wait;
	TACCTLx[alarm_id] = CCIE;
	
	timer_alarms[alarm_id].active = 1;
	timer_alarms[alarm_id].period_count = 0;
	timer_alarms[alarm_id].period = 0;
	timer_alarms[alarm_id].function = function;

	return 0;
}

int16_t timer_execute_several(timer_cb function, uint16_t period, uint16_t count) {
	int16_t alarm_id;
	uint16_t now;
	now = TAR;

	alarm_id = get_available_slot();

	if (alarm_id < 0) {
		return -1;
	}
	
	TACCRx[alarm_id] = now + period;
	TACCTLx[alarm_id] = CCIE;
	
	timer_alarms[alarm_id].active = 1;
	timer_alarms[alarm_id].period_count = count;
	timer_alarms[alarm_id].period = period;
	timer_alarms[alarm_id].function = function;
	
	return 0;
}

int16_t timer_cancel_execute(int16_t id) {
	if (id < 0 ||  id >= TIMER_ALARM_NUMBER)
		return 0;
	
	TACCRx[id] = 0;
	TACCTLx[id] = 0;
	timer_alarms[id].active = 0;
	timer_alarms[id].period_count = 0;
	timer_alarms[id].period = 0;
	timer_alarms[id].function = 0x0;
	
	return 1;
}

void timer_stop() {
	TACTL = TACLR;
}

void timer_pause() {
	TACTL &= ~MC_UPDOWN;
}

void timer_unpause() {
	TACTL |= MC_CONT;
}

void timer0irq(void);
interrupt (TIMERA0_VECTOR) timer0irq( void ) {
	if (timer_alarms[0].period_count != 0)
		TACCRx[0] += timer_alarms[0].period;
	else {
		TACCRx[0] = 0;
		TACCTLx[0] = 0;
		timer_alarms[0].active = 0;
	}

	if (timer_alarms[0].period_count > 0) {
		timer_alarms[0].period_count--;
	}
	
	if (timer_alarms[0].function && timer_alarms[0].function())
		LPM4_EXIT;
}

void timer1irq(void);
interrupt (TIMERA1_VECTOR) timer1irq( void ) {
	uint16_t alarm;
	
	alarm = TAIV >> 1;
	
	if (alarm == 0x05) {
		master_time ++;
	} else if (alarm <= 2) {
		if (timer_alarms[alarm].period_count != 0)
			TACCRx[alarm] += timer_alarms[alarm].period;
		else {
			TACCRx[alarm] = 0;
			TACCTLx[alarm] = 0;
			timer_alarms[alarm].active = 0;
		}

		if (timer_alarms[alarm].period_count > 0) {
			timer_alarms[alarm].period_count--;
		}
		
		if (timer_alarms[alarm].function && timer_alarms[alarm].function())
			LPM4_EXIT;
	}
}

static int16_t get_available_slot() {
	int16_t i;
	
	for (i=0; i<TIMER_ALARM_NUMBER; i++) {
		if (timer_alarms[i].active == 0) {
			return i;
		}
	}

	return -1;
}
