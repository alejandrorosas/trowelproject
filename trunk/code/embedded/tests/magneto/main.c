#include <io.h>
#include <signal.h>
#include <stdio.h>
#include "clock.h"
#include "leds.h"
#include "uart.h"
#include "hmc5843.h"


int putchar(int c) {
  return uart_putchar(c);
}

int main(void) {
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    eint();
    
    clock_dco_set(8); // DCO 8MHz
    clock_mclk_set(CLOCK_SOURCE_DCO, 1); // MCLK 8MHz
    clock_smclk_set(CLOCK_SOURCE_DCO, 8); // SMCLK 1MHz
    clock_aclk_set(1);

    leds_init();
    leds_on(LED_RED);
    
    uart_init();
    uart_register_cb(0x0);
    
    printf("HMC5843 test program\n");
    
    if (hmc5843_init()<0) {
        LPM4;
    } else {
        LPM3;
    }
    
    while (1) {
        LPM4;
    }
    
    return 0;
}
