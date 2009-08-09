#include <io.h>
#include <signal.h>

#include <stdio.h>
#include <string.h>

#include "clock.h"
#include "leds.h"
#include "csma.h"
#include "timer.h"
#include "uart.h"

#define EVENT_NONE 0
#define EVENT_SEND 1
#define EVENT_SENT 2
#define EVENT_RX   3

volatile int event = 0;
uint16_t rx_from;
struct msg {
  uint32_t t;
  int16_t x, y, z;
} rx_msg;

int putchar(int c) {
  return uart_putchar(c);
}

int send(void) {
  event = EVENT_SEND;
  return 1;
}

int sent() {
  event = EVENT_SENT;
  return 1;
}

int rx() {
  event = EVENT_RX;
  return 1;
}

int main(void) {
    
  WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
  eint();
  
  clock_dco_set(1); // DCO 1MHz
  clock_mclk_set(CLOCK_SOURCE_DCO, 1); // MCLK 1MHz
  clock_smclk_set(CLOCK_SOURCE_DCO, 1); // SMCLK 1MHz
  clock_aclk_set(1);
  
  leds_init();
  leds_off(LEDS_ALL);
  
  csma_init();
  csma_set_tx_cb(sent);
  csma_set_rx_cb(rx);
  
  leds_on(LEDS_ALL);
  
  uart_init();
  
  int len;
  while (1) {
    event = 0;
    LPM0;
    switch (event) {
    case EVENT_SEND:
      //~ csma_send(CSMA_BROADCAST, msg, strlen(msg));
      break;
    case EVENT_SENT:
      leds_toggle(LED_GREEN);
      break;
    case EVENT_RX:
      len = csma_read(&rx_from, &rx_msg, sizeof(rx_msg));
      leds_toggle(LED_RED);
      printf("t=%lu, x=%i, y=%i, z=%i\n", rx_msg.t, rx_msg.x, rx_msg.y, rx_msg.z);
      break;
    }
  }
    
  return 0;
}
