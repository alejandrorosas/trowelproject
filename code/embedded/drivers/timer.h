#ifndef _TIMER_H
#define _TIMER_H

typedef int16_t (*timer_cb)(void);

enum timer_source {
	SMCLK = 1,
	ACLK = 2
};

enum timer_div {
	DIV_1 = 1,
	DIV_2 = 2,
	DIV_4 = 4,
	DIV_8 = 8
};

void timer_init(void);
uint16_t timer_start(enum timer_source source, enum timer_div div);
void timer_pause();
void timer_unpause();
void timer_stop();

uint16_t timer_time(void);
uint16_t timer_mastertime(void);

int16_t timer_execute_once(timer_cb function, uint16_t wait);

#define TIMER_COUNT_INFINITE 0xFFFF

int16_t timer_execute_several(timer_cb function, uint16_t period, uint16_t count);

int16_t timer_cancel_execute(int16_t id);

#endif
