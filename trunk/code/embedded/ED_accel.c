#include <io.h>
#include <signal.h>

#include "clock.h"
#include "leds.h"
#include "timer.h"
#include "netED.h"
#include "lis302.h"

#define ALARM_TIMEOUT TIMER_ALARM_0

/*--PROTOTYPES--*/
int try_register(void);
int registered(void);
uint16_t accel_service_get(uint16_t param);
void accel_service_set(uint16_t param, uint16_t config);
uint16_t leds_service_get(uint16_t param);
void leds_service_set(uint16_t param, uint16_t config);

/*---DATA---*/
volatile int is_registered;
struct net_service accel_service;
struct net_service leds_service;


int main(void) {
    // Stop watchdog timer
    WDTCTL = WDTPW + WDTHOLD;
    eint();
    
    // configure clock
    clock_dco_set(8); // DCO 8MHz
    clock_mclk_set(CLOCK_SOURCE_DCO, 1); // MCLK 8MHz
    clock_smclk_set(CLOCK_SOURCE_DCO, 1); // SMCLK 8MHz
    clock_aclk_set(1);
    
    // init drivers
    leds_init();
    leds_off(LEDS_ALL);
    lis302_init();
    leds_on(LEDS_ALL);
    timer_init();
    timer_start(TIMER_SOURCE_ACLK, 1);
    
    //configure accel service
    accel_service.type = SERVICE_TYPE_ACCEL;
    accel_service.name = "3axisAccel";
    accel_service.get = accel_service_get;
    accel_service.set = accel_service_set;
    
    //configure LEDs service
    leds_service.type = SERVICE_TYPE_LEDS;
    leds_service.name = "2leds";
    leds_service.get = leds_service_get;
    leds_service.set = leds_service_set;
    
    // set a timeout for registration
    
    timer_set_alarm(ALARM_TIMEOUT, 6000, 6000, TIMER_MODE_FROM_NOW, 0);
    timer_register_cb(ALARM_TIMEOUT, try_register);
    
    // start network
    net_init();
    net_set_register_cb(registered);
    is_registered = 0;
    net_register();
    leds_off(LED_GREEN);
    
    // wait for network registration
    while (!is_registered) {
        LPM3;
    }
    
    leds_off(LEDS_ALL);
    
    // wait until end of the world
    while (1) LPM3;
    return 0;
}

int try_register(void) {
    net_register();
    leds_toggle(LED_GREEN);
    return 0;
}

int registered(void) {
    is_registered = 1;
    timer_unset_alarm(ALARM_TIMEOUT);
    return 1;
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
