#include <io.h>
#include <signal.h>

#include "clock.h"
#include "leds.h"
#include "csma.h"
#include "timer.h"
#include "lis302.h"

#define SAMPLE_NUM 12

struct msg {
  uint32_t t;
  int8_t x[SAMPLE_NUM], y[SAMPLE_NUM], z[SAMPLE_NUM];
} tx_msg;

int fill = 0;

int sample(void) {
  leds_toggle(LED_RED);
  
  tx_msg.x[fill] = lis302_getx();
  tx_msg.y[fill] = lis302_gety();
  tx_msg.z[fill] = lis302_getz();
  fill++;
  if (fill == SAMPLE_NUM) {
    tx_msg.t += 10*SAMPLE_NUM; // ms
    fill = 0;
    csma_send(CSMA_BROADCAST, (uint8_t*)&tx_msg, sizeof(tx_msg));
  }

  return 0;
}

int sent() {
  leds_toggle(LED_GREEN);
  return 0;
}

int main(void) {
    
  WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
  eint();
  
  clock_dco_set(8); // DCO 8MHz
  clock_mclk_set(CLOCK_SOURCE_DCO, 1); // MCLK 8MHz
  clock_smclk_set(CLOCK_SOURCE_DCO, 1); // SMCLK 8MHz
  clock_aclk_set(1);
  
  leds_init();
  leds_off(LEDS_ALL);
  
  lis302_init();
  
  csma_init();
  csma_set_tx_cb(sent);
  
  timer_start(TIMER_SOURCE_SMCLK, 8); // timer tick 1us
  timer_register_cb(TIMER_ALARM_0, sample);
  timer_set_alarm(TIMER_ALARM_0, 10000, 10000, TIMER_MODE_FROM_NOW, 0);
  
  leds_on(LEDS_ALL);
  
  tx_msg.t = 0;
  
  while (1) {
    LPM0;
  }
    
  return 0;
}
