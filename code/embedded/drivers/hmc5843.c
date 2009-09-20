#include <io.h>
#include <signal.h>
#include <stdio.h>

#include "leds.h"
#include "i2c.h"
uint8_t data[13];

static inline void micro_delay(register unsigned int n) {
    __asm__ __volatile__ (
  "1: \n"
  " dec %[n] \n"
  " jne 1b \n"
        : [n] "+r"(n));
}

int16_t hmc5843_init(void) {
    int16_t a;
    uint8_t val;
    
    i2c_init();
    
    micro_delay(0xFFFF);
    
    data[0] = 0xFF;
    data[1] = 0xFF;
    data[2] = 0xFF;
    data[3] = 0xFF;
    data[4] = 0xFF;
    data[5] = 0xFF;
    
    val = 0x01;
    
    a = i2c_write(0x1, &val, 1);
    a = i2c_read(0x1E, data, 13);
    
    a = 2;
    a+=3;
    
    
    return 0;
}
