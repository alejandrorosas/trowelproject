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

int16_t x,y,z;

static inline void micro_delay(register unsigned int n) {
    __asm__ __volatile__ (
  "1: \n"
  " dec %[n] \n"
  " jne 1b \n"
        : [n] "+r"(n));
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
        printf("HMC5843 init error\n");
        LPM4;
    }
    
    while (1) {
        int i;
        for (i=0;i<20;i++) micro_delay(0xFFFF);
        if ( hmc5843_getXYZ(&x, &y, &z)<0 ) {
            printf("HMC5843 read error\n");
        } else {
            printf("X=%i,Y=%i, Z=%i\n", x, y, z);
        }
    }
    
    while (1) {
        LPM4;
    }
    
    return 0;
}
