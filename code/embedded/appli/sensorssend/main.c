#include <stdio.h>
#include <io.h>
#include <signal.h>
#include "clock.h"
#include "leds.h"
#include "timer.h"
#include "lis302.h"
#include "hmc5843.h"
#include "csma.h"
#include "event.h"

static int sample_irq(void);
static void sample_evt(void* data);

static int sample_irq(void) {
    event_post(sample_evt, 0);
    leds_toggle(LED_RED);
    return 1;
}

static void sample_evt(void* data) {
    static int16_t accel = 1;
    int x, y, z;
    uint8_t msg[32];
    int len;
    
    if (accel) {
        lis302_reinit();
        x = lis302_getx();
        y = lis302_gety();
        z = lis302_getz();
    } else {
        hmc5843_reinit();
        hmc5843_getXYZ(&x, &y, &z);
    }
    
    accel = ! accel;
    
    len = sprintf(msg, "%c %i %i %i\n",accel?'a':'m', x, y, z);
    csma_send(CSMA_BROADCAST, msg, len);
    leds_toggle(LED_GREEN);
}

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer
    eint();
    
    clock_dco_set(8); // DCO 8MHz
    clock_mclk_set(CLOCK_SOURCE_DCO, 1); // MCLK 8MHz
    clock_smclk_set(CLOCK_SOURCE_DCO, 8); // SMCLK 1MHz
    clock_aclk_set(1);
    
    event_init();
    
    csma_init();
    csma_send(CSMA_BROADCAST, "Sensor Test Program\n", sizeof("Sensor Test Program"));
    
    leds_init();
    leds_off(LEDS_ALL);
    
    hmc5843_init();
    lis302_init();
    
    timer_start(TIMER_SOURCE_ACLK, 1);
    timer_register_cb(TIMER_ALARM_0, sample_irq);
    timer_set_alarm(TIMER_ALARM_0, 12000, 1000, TIMER_MODE_FROM_NOW, 0);
    
    
    
    while (1) {
        event_process();
        LPM0;
    }
    
    return 0;
}
