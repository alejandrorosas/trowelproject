#include <io.h>
#include "clock.h"

volatile unsigned int i;                    // volatile to prevent optimization

int main(void) {
    WDTCTL = WDTPW + WDTHOLD;                 // Stop watchdog timer

    clock_dco_set(1); // DCO 1MHz
    clock_mclk_set(CLOCK_SOURCE_DCO, 1); // MCLK 1MHz
    clock_smclk_set(CLOCK_SOURCE_DCO, 1);
    clock_aclk_set(1);
    
    uart_init();
    
    for (;;) {
        int i = 0;
        while (i < 30000) i++;
    }
    return 0;
}
