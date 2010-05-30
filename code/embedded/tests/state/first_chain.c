#include <io.h>
#include "state.h"
#include "leds.h"
#include "timer.h"

#define TIMER_SECOND 12000

static int16_t eventX_cb(void);
static int16_t eventY_cb(void);

struct st_chain first_chain;
struct st_state stateA, stateB, stateC;
struct st_event eventX, eventY;
struct st_transition t1, t2, t3, t4;

void A_exec(struct st_event* evt);
void B_exec(struct st_event* evt);
void C_exec(struct st_event* evt);

static volatile int delay;

void first_init() {
	st_init_chain(&first_chain);

	stateA.exec = A_exec;
	stateB.exec = B_exec;
	stateC.exec = C_exec;
	st_add_state(&first_chain, &stateA);
	st_add_state(&first_chain, &stateB);
	st_add_state(&first_chain, &stateC);

	
	t1.event = &eventX;
	t1.from_state = &stateA;
	t1.to_state = &stateB;

	t2.event = &eventX;
	t2.from_state = &stateB;
	t2.to_state = &stateC;
	
	t3.event = &eventX;
	t3.from_state = &stateC;
	t3.to_state = &stateB;

	t4.event = &eventY;
	t4.from_state = &stateB;
	t4.to_state = &stateA;

	st_add_transition(&first_chain, &t1);
	st_add_transition(&first_chain, &t2);
	st_add_transition(&first_chain, &t3);
	st_add_transition(&first_chain, &t4);

	leds_set(0);

	st_set_active_state(&first_chain, &stateA);
	timer_execute_once(eventX_cb, TIMER_SECOND);
}

static int16_t eventX_cb(void) {
	st_notify_event(&first_chain, &eventX);
	return 1;
}

static int16_t eventY_cb(void) {
	st_notify_event(&first_chain, &eventY);
	return 1;
}

void A_exec(struct st_event* evt) {
	leds_set(1);
	timer_execute_once(eventX_cb, TIMER_SECOND);
}

void B_exec(struct st_event* evt) {
	static int16_t count = 0;
	leds_set(2);

	timer_execute_once( count&1 ? eventY_cb : eventX_cb, TIMER_SECOND);
	
	count ++;
}

void C_exec(struct st_event* evt) {
	leds_set(3);
	timer_execute_once(eventX_cb, TIMER_SECOND);
}
