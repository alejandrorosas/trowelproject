#include <io.h>
#include "leds.h"

void leds_init(void) {
    P1SEL &= ~LEDS_ALL;
    P1REN &= ~LEDS_ALL;
    P1DIR |= LED_RED | LED_GREEN;
    P1OUT = (P1OUT|~LEDS_ALL);
}

void leds_set(uint16_t leds) {
    P1OUT = (P1OUT|~LEDS_ALL) | (leds & LEDS_ALL);
}

void leds_on(uint16_t leds) {
    P1OUT |= (leds & LEDS_ALL); 
}

void leds_off(uint16_t leds) {
    P1OUT &= ~(leds & LEDS_ALL);
}

void leds_toggle(uint16_t leds) {
    P1OUT ^= (leds & LEDS_ALL);
}
