#ifndef _BUTTON_H_
#define _BUTTON_H_
typedef int (*button_cb_t)();

enum button_edge {
	HIGH_TO_LOW,
	LOW_TO_HIGH
};

void button_init(enum button_edge edge, button_cb_t cb);
void button_stop(void);

#endif
