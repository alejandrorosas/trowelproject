#include <io.h>
#include <signal.h>
#include "rtc.h"

#define DEFAULT_FREQUENCY 12000

void timerb0irq(void);

static uint32_t time_seconds;
static uint16_t frequency;

static struct rtc_alarm* next_alarm;

void rtc_init() {
	frequency = DEFAULT_FREQUENCY;
	time_seconds = 0;
	next_alarm = 0x0;
	
	rtc_resume(0);
}


void rtc_set_aclk_frequency(uint16_t freq) {
	TBCCR0 = freq;
	
	// if the timerB count is above the new period
	if (TBR > freq) {
		// reset and increment seconds
		TBR = 0;
		timerb0irq();
	}
	
	// store value
	frequency = freq;
}

void rtc_set_time(uint32_t seconds) {
	time_seconds = seconds;
}

uint32_t rtc_time(void) {
	return time_seconds;
}

void rtc_halt(void) {
	TBCTL = TBCLR;
}

void rtc_resume(uint16_t elapsed) {
	for (;elapsed!=0;elapsed--) {
		timerb0irq();
	}
	
	// start timer
	TBCTL = TBCLR;
	TBCCR0 = frequency;
	TBCTL |= TBSSEL_ACLK | ID_DIV1 | MC_UPTO_CCR0;
	TBCCTL0 = CCIE;
	
}

void rtc_set_alarm(struct rtc_alarm *alarm) {
	struct rtc_alarm *tmp_alarm;
	
	// set next to null
	alarm->next = 0x0;
	
	// if date in the past, call now
	if (alarm->date <= time_seconds) {
		alarm->exec_cb();
		return;
	}
	
	// if alarm is the next, place it
	if ( (next_alarm == 0x0) || (next_alarm->date > alarm->date) ) {
		next_alarm = alarm;
		alarm->next = next_alarm;
		return;
	}
	
	// find the right position
	tmp_alarm = next_alarm;
	
	while ( (tmp_alarm -> next != 0x0) && (tmp_alarm->date < alarm->date) ) {
		tmp_alarm = tmp_alarm->next;
	}
	alarm->next = tmp_alarm->next;
	tmp_alarm->next = alarm;
}

interrupt (TIMERB0_VECTOR) timerb0irq( void ) {
	time_seconds ++;
	
	if ( (next_alarm != 0) && (next_alarm->date == time_seconds) ) {
		next_alarm->exec_cb();
		next_alarm = next_alarm->next;
	}
}
