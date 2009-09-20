#include <io.h>
#include <signal.h>

#include <stdio.h>
#include <string.h>

#include "clock.h"
#include "leds.h"
#include "timer.h"
#include "uart.h"
#include "netFD.h"

/*----PROTOTYPES----*/
int putchar(int c);
int new_node(uint16_t node);


int main(void) {
    // Stop watchdog timer
    WDTCTL = WDTPW + WDTHOLD;
    eint();
    
    // init drivers
    clock_dco_set(1); // DCO 1MHz
    clock_mclk_set(CLOCK_SOURCE_DCO, 1); // MCLK 1MHz
    clock_smclk_set(CLOCK_SOURCE_DCO, 1); // SMCLK 1MHz
    clock_aclk_set(1);
    leds_init();
    leds_off(LEDS_ALL);
    uart_init();
    
    // config and start net
    net_init();
    net_set_newnode_cb(new_node);
    
    
    while (1) {
        LPM3;
    }

    return 0;
}

int putchar(int c) {
  return uart_putchar(c);
}

int new_node(uint16_t node) {
    printf("New node discovered: %.4x\n", node);
    return 0;
}
