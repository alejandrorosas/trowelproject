#ifndef _TIMER_H
#define _TIMER_H

typedef int (*timer_cb)(void);

#define TIMER_SOURCE_SMCLK 1
#define TIMER_SOURCE_ACLK 2

uint16_t timer_start(int source, int div);

#define TIMER_ALARM_NUMBER 3

#define TIMER_ALARM_0 0
#define TIMER_ALARM_1 1
#define TIMER_ALARM_2 2
#define TIMER_ALARM_OVERFLOW 3

uint16_t timer_register_cb(int alarm, timer_cb cb);
uint16_t timer_time(void);

#define TIMER_MODE_FROM_NOW 1
#define TIMER_MODE_FROM_REF 2

uint16_t timer_set_alarm(int alarm, uint16_t delay, uint16_t period, int mode, uint16_t ref);
uint16_t timer_unset_alarm(int alarm);
void timer_stop(void);

#endif
