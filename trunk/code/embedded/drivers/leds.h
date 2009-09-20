#ifndef _LEDS_H_
#define _LEDS_H_

#define LED_RED 1
#define LED_GREEN 2
#define LEDS_ALL (LED_RED | LED_GREEN)

void leds_init(void);
void leds_set(uint16_t leds);
uint16_t leds_get(void);
void leds_on(uint16_t leds);
void leds_off(uint16_t leds);
void leds_toggle(uint16_t leds);

#endif
