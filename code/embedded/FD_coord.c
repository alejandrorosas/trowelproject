#include <io.h>
#include <signal.h>

#include <stdio.h>
#include <string.h>

#include "clock.h"
#include "leds.h"
#include "csma.h"
#include "timer.h"
#include "uart.h"


uint16_t rx_from;
#define SAMPLE_NUM 12
struct msg {
  uint32_t t;
  int8_t x[SAMPLE_NUM], y[SAMPLE_NUM], z[SAMPLE_NUM];
} rx_msg;

int putchar(int c) {
  return uart_putchar(c);
}

int rx() {
  int len, i;
  len = csma_read(&rx_from, &rx_msg, sizeof(rx_msg));
  leds_toggle(LED_RED);
  for (i=0; i<SAMPLE_NUM; i++) {
    printf("t=%lu, x=%i, y=%i, z=%i\n", rx_msg.t, rx_msg.x[i], rx_msg.y[i], rx_msg.z[i]);
  }
  return 0;
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
  csma_set_rx_cb(rx);
  
  leds_on(LEDS_ALL);
  
  uart_init();
  
  while (1) {
    LPM0;
  }
    
  return 0;
}
