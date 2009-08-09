#include <io.h>
#include <signal.h>
#include "clock.h"
#include "leds.h"
#include "uart.h"
#include "lis302.h"


int putchar(int c) {
  return uart_putchar(c);
}

int x, y, z;
int main(void) {
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    eint();
    
    clock_dco_set(1); // DCO 1MHz
    clock_mclk_set(CLOCK_SOURCE_DCO, 1); // MCLK 1MHz
    clock_smclk_set(CLOCK_SOURCE_DCO, 1); // SMCLK 1MHz
    clock_aclk_set(1);

    leds_init();
    leds_on(LED_RED);
    
    uart_init();
    uart_register_cb(0x0);
    
    lis302_init();
    
    
    int j = 0;
    while (1) {
        x = lis302_getx();
        y = lis302_gety();
        z = lis302_getz();
        printf("%.3i %.3i %.3i\n", x, y, z);
        while (j++) ;
    }
    
    return 0;
}
