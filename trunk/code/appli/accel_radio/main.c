#include <io.h>
#include <signal.h>
#include <string.h>
#include "clock.h"
#include "leds.h"
#include "csma.h"
#include "timer.h"
#include "lis302.h"

#define EVENT_NONE 0
#define EVENT_SEND 1
#define EVENT_SENT 2
#define EVENT_RX   3

volatile int event = 0;
struct msg {
  int x, y, z;
} msg;

int send(void) {
  event = EVENT_SEND;
  return 1;
}

int sent() {
  event = EVENT_SENT;
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
  
  lis302_init();
  
  csma_init();
  csma_set_tx_cb(sent);
  
  timer_start(TIMER_SOURCE_SMCLK, 8);
  timer_register_cb(TIMER_ALARM_0, send);
  timer_set_alarm(TIMER_ALARM_0, 65000, 65000, TIMER_MODE_FROM_NOW, 0);
  
  leds_on(LEDS_ALL);
  
  
  while (1) {
    event = 0;
    LPM0;
    switch (event) {
    case EVENT_SEND:
      msg.x = lis302_getx();
      msg.y = lis302_gety();
      msg.z = lis302_getz();
      csma_send(CSMA_BROADCAST, (uint8_t*)&msg, sizeof(msg));
      break;
    case EVENT_SENT:
      leds_toggle(LED_GREEN);
      break;
    }
  }
    
  return 0;
}
