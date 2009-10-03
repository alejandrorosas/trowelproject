#include <io.h>
#include <signal.h>

#include "clock.h"
#include "event.h"
#include "leds.h"
#include "timer.h"
#include "netED.h"

#include "leds_service.h"
#include "accel_service.h"
#include "magneto_service.h"

#define ALARM_TIMEOUT TIMER_ALARM_0

/*--PROTOTYPES--*/
static int try_register(void);
static int registered(void);

int main(void) {
    // Stop watchdog timer
    WDTCTL = WDTPW + WDTHOLD;
    eint();
    
    // configure clock
    clock_dco_set(8); // DCO 8MHz
    clock_mclk_set(CLOCK_SOURCE_DCO, 1); // MCLK 8MHz
    clock_smclk_set(CLOCK_SOURCE_DCO, 8); // SMCLK 1MHz
    clock_aclk_set(1);
    
    // init event system
    event_init();
    
    // init timer
    timer_init();
    timer_start(TIMER_SOURCE_ACLK, 1);
    
    // set a timeout for registration
    timer_set_alarm(ALARM_TIMEOUT, 6000, 6000, TIMER_MODE_FROM_NOW, 0);
    timer_register_cb(ALARM_TIMEOUT, try_register);
    
    // start network
    net_init();
    net_set_register_cb(registered);
    net_add_service(leds_service_init());
    net_add_service(accel_service_init());
    net_add_service(magneto_service_init());
    net_register();
    leds_off(LED_GREEN);
    
    // wait until end of the world
    while (1) event_process();
    return 0;
}

int try_register_evt(void) {
    event_post(try_register);
    return 1;
}

int try_register(void) {
    net_register();
    leds_toggle(LED_GREEN);
}

int registered(void) {
    timer_unset_alarm(ALARM_TIMEOUT);
    return 0;
}

// ACCEL service
uint16_t accel_service_get(uint16_t param) {
    switch (param) {
    case SERVICE_ACCEL_X:
        return lis302_getx();
    case SERVICE_ACCEL_Y:
        return lis302_gety();
    case SERVICE_ACCEL_Z:
        return lis302_getz();
    }
    return 0x1000;
}

void accel_service_set(uint16_t param, uint16_t config) {
    
}

// LEDS service
uint16_t leds_service_get(uint16_t param) {
    switch (param) {
    case SERVICE_LEDS_GET:
        return leds_get();
    }
    
    return 0x1000;
}

void leds_service_set(uint16_t param, uint16_t config) {
    switch (param) {
    case SERVICE_LEDS_SET:
        leds_set(config);
        break;
    }
}
