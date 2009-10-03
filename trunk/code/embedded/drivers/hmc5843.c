#include <io.h>

#include "leds.h"
#include "i2c.h"

#define HMC_ADDR 0x1E

enum {
    REG_CONF_A = 0x0,
    REG_CONF_B = 0x1,
    REG_MODE   = 0x2,
    REG_X_M    = 0x3,
    REG_X_L    = 0x4,
    REG_Y_M    = 0x5,
    REG_Y_L    = 0x6,
    REG_Z_M    = 0x7,
    REG_Z_L    = 0x8,
    REG_STATUS = 0x9,
    REG_ID_A   = 0xA,
    REG_ID_B   = 0xB,
    REG_ID_C   = 0xC
};

enum {
    CONF_A_DRATE_MASK = 0x1C,
    CONF_A_DRATE_0_5 = 0x00,
    CONF_A_DRATE_1   = 0x04,
    CONF_A_DRATE_2   = 0x08,
    CONF_A_DRATE_5   = 0x0C,
    CONF_A_DRATE_10  = 0x10,
    CONF_A_DRATE_20  = 0x14,
    CONF_A_DRATE_50  = 0x18,
    CONF_A_MEAS_MASK = 0x3,
    CONF_A_MEAS_NORM = 0x0,
    CONF_A_MEAS_POS  = 0x1,
    CONF_A_MEAS_NEG  = 0x2
};

enum {
    CONF_B_GAIN_MASK = 0xE0,
    CONF_B_GAIN_0_7  = 0x00,
    CONF_B_GAIN_1_0  = 0x20,
    CONF_B_GAIN_1_5  = 0x40,
    CONF_B_GAIN_2_0  = 0x60,
    CONF_B_GAIN_3_2  = 0x80,
    CONF_B_GAIN_3_8  = 0xA0,
    CONF_B_GAIN_4_5  = 0xC0,
    CONF_B_GAIN_6_5  = 0xE0
};

enum {
    MODE_MASK   = 0x3,
    MODE_CONT   = 0x0,
    MODE_SINGLE = 0x1,
    MODE_IDLE   = 0x2,
    MODE_SLEEP  = 0x3
};

enum {
    STATUS_RDY  = 0x1,
    STATUS_LOCK = 0x2,
    STATUS_REN  = 0x4
};

#define SWAP(x) (x)=((x)>>8)+((x)<<8)

static inline void micro_delay(register unsigned int n) {
    __asm__ __volatile__ (
  "1: \n"
  " dec %[n] \n"
  " jne 1b \n"
        : [n] "+r"(n));
}

int16_t hmc5843_init(void) {
    uint8_t id[3], conf[2];
    int16_t ret;
    
    // flush I2C
    //~ hmc5843_reinit();
    
    // write mod
    conf[0] = REG_MODE;
    conf[1] = 0;
    ret = i2c_write(HMC_ADDR, conf, 2);
    if (ret<0) {
        return ret;
    }
    
    // get ID registers
    ret = i2c_write_read(HMC_ADDR, REG_ID_A, id, 3);
    if (ret<0) {
        return ret;
    }
    
    if ( (id[0]!='H') || (id[1]!='4') || (id[2]!='3') ) {
        // error
        return -1;
    }
    
    micro_delay(0xFFFF);
    
    return 0;
}

void hmc5843_reinit(void) {
    uint8_t dummy;
    
    // send dummy byte, to flush
    dummy = 0x0;
    i2c_write(0x0, &dummy, 1);
}

int16_t hmc5843_getXYZ(int16_t* x, int16_t* y, int16_t* z) {
    uint8_t tmp[2], *fake;
    // get X
    i2c_write_read(HMC_ADDR, REG_X_M, tmp, 2);
    fake = (uint8_t *)x;
    fake[0] = tmp[1];
    fake[1] = tmp[0];
    
    // get Y
    i2c_write_read(HMC_ADDR, REG_Y_M, tmp, 2);
    fake = (uint8_t *)y;
    fake[0] = tmp[1];
    fake[1] = tmp[0];
    
    // get Z
    i2c_write_read(HMC_ADDR, REG_Z_M, tmp, 2);
    fake = (uint8_t *)z;
    fake[0] = tmp[1];
    fake[1] = tmp[0];
    
    return 0;
}
