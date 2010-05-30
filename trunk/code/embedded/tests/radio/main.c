#include <io.h>
#include <signal.h>
#include <string.h>
#include "clock.h"
#include "leds.h"
#include "mac.h"
#include "timer.h"

#define EVENT_NONE 0
#define EVENT_SEND 1
#define EVENT_SENT 2
#define EVENT_RX   3

volatile int event = 0;
uint8_t msg[14] = "hello";
uint16_t rx_from;
uint8_t rx_msg[14];

int send(void) {
  event = EVENT_SEND;
  return 1;
}

int sent(int success) {
  event = EVENT_SENT;
  return 1;
}

int rx(uint8_t* data, int16_t len, uint8_t src) {
    if (len == strlen(msg) && !strcmp(msg, data)) {
        event = EVENT_RX;
        return 1;
    }
    return 0;
}

int main(void) {
    
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    eint();

    clock_dco_set(8); // DCO 8MHz
    clock_mclk_set(CLOCK_SOURCE_DCO, 1); // MCLK 8MHz
    clock_smclk_set(CLOCK_SOURCE_DCO, 8); // SMCLK 1MHz
    clock_aclk_set(1);

    leds_init();
    leds_off(LEDS_ALL);

    mac_init();
    mac_set_tx_cb(sent);
    mac_set_rx_cb(rx);

    timer_start(TIMER_SOURCE_ACLK, 1);
    timer_register_cb(TIMER_ALARM_0, send);
    timer_set_alarm(TIMER_ALARM_0, 12000, 12000, TIMER_MODE_FROM_NOW, 0);

    leds_on(LEDS_ALL);


    while (1) {
        event = 0;
        LPM0;
        switch (event) {
        case EVENT_SEND:
            mac_send(msg, strlen(msg), MAC_BROADCAST);
            break;
        case EVENT_SENT:
            leds_toggle(LED_GREEN);
            break;
        case EVENT_RX:
            leds_toggle(LED_RED);
            break;
        }
    }
    return 0;
}
