#include <io.h>
#include <signal.h>
#include "clock.h"
#include "leds.h"
#include "csma.h"
#include "uart.h"
#include "event.h"

static void print(void* data);

uint8_t msg[64];
uint16_t len;

static int rx() {
    uint16_t from;
    len = csma_read(&from, msg, 64);
    leds_toggle(LEDS_ALL);
    event_post(print, 0);
    return 1;
}

void print(void* data) {
    int i;
    for (i=0;i<len;i++) {
        uart_putchar(msg[i]);
    }
}

int main(void) {
    
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    eint();

    clock_dco_set(8); // DCO 8MHz
    clock_mclk_set(CLOCK_SOURCE_DCO, 1); // MCLK 8MHz
    clock_smclk_set(CLOCK_SOURCE_DCO, 8); // SMCLK 1MHz

    leds_init();
    leds_on(LEDS_ALL);

    csma_init();
    csma_set_rx_cb(rx);
    
    uart_init();
    {
        int i;
        for (i=0;i<sizeof("Radio print started\n");i++) {
            uart_putchar("Radio print started\n"[i]);
        }
    }
    
    event_init();
    
    while (1) {
        event_process();
        LPM0;
    }

    return 0;
}
