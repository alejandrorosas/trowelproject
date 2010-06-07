#ifndef _RTC_H_
#define _RTC_H_

struct rtc_alarm {
	uint32_t date;
	struct rtc_alarm* next;
	int (*exec_cb)(void);
};

void rtc_init(void);

void rtc_set_aclk_frequency(uint16_t freq);

void rtc_set_time(uint32_t seconds);

uint32_t rtc_time(void);

void rtc_set_alarm(struct rtc_alarm *alarm);

void rtc_halt(void);

void rtc_resume(uint16_t elapsed);

#endif
