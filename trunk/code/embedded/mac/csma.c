#include <io.h>
#include <string.h>
#include "mac.h"
#include "cc2500.h"

static const uint8_t csma_radio_reg[][2] = {
    {CC2500_REG_IOCFG2, 0x09}, // CCA
    {CC2500_REG_IOCFG0, 0x06}, // sync-EOP
    {CC2500_REG_FIFOTHR, 0x07}, // TX:33b, RX:32b
    {CC2500_REG_PKTLEN, 0xFF},
    {CC2500_REG_PKTCTRL1, 0x04}, // append status
    {CC2500_REG_PKTCTRL0, 0x45}, // whiten, crc, variable length
    {CC2500_REG_ADDR, 0x00},
    {CC2500_REG_CHANNR, 0x00},
    {CC2500_REG_FSCTRL1, 0x0A},
    {CC2500_REG_FSCTRL0, 0x00},
    {CC2500_REG_FREQ2, 0x5D},
    {CC2500_REG_FREQ1, 0x93},
    {CC2500_REG_FREQ0, 0xB1},
    {CC2500_REG_MDMCFG4, 0x2D},
    {CC2500_REG_MDMCFG3, 0x3B},
    {CC2500_REG_MDMCFG2, 0x73},
    {CC2500_REG_MDMCFG1, 0x22},
    {CC2500_REG_MDMCFG0, 0xF8},
    {CC2500_REG_DEVIATN, 0x00},
    {CC2500_REG_MCSM0, 0x08}, // no auto calib
    {CC2500_REG_FOCCFG, 0x1D},
    {CC2500_REG_BSCFG, 0x1C},
    {CC2500_REG_AGCCTRL2, 0xC7},
    {CC2500_REG_AGCCTRL1, 0x00},
    {CC2500_REG_AGCCTRL0, 0xB0},
    {CC2500_REG_FREND1, 0xB6},
    {CC2500_REG_FREND0, 0x10},
    {CC2500_REG_FSCAL3, 0xEA},
    {CC2500_REG_FSCAL2, 0x0A},
    {CC2500_REG_FSCAL1, 0x00},
    {CC2500_REG_FSCAL0, 0x11},
    {CC2500_REG_FSTEST, 0x59},
    {CC2500_REG_TEST2, 0x88},
    {CC2500_REG_TEST1, 0x31},
    {CC2500_REG_TEST0, 0x0B},
};

static void reset_rx(void);
static int16_t rx_cb(void);
static int16_t tx_cb(void);
static void force_idle(void);
static void force_calib(void);


static int16_t (*mac_rx_cb)(uint8_t* data, int16_t len, uint8_t from);
static int16_t (*mac_tx_cb)(int16_t success);

uint8_t mac_addr = 0x0;

static struct {
    uint8_t len;
    uint8_t src,
            dst;
    uint8_t msg[59];
} rx_msg;

static int16_t rx_ready;

static inline void micro_delay(register uint16_t n) {
    __asm__ __volatile__ (
  "1: \n"
  " dec %[n] \n"
  " jne 1b \n"
        : [n] "+r"(n));
}

void mac_init() {
    int16_t i;
    cc2500_init((uint8_t*) csma_radio_reg, sizeof(csma_radio_reg)/2);
    
    cc2500_gdo0_int_disable();
    cc2500_gdo2_int_disable();

    rx_ready = 0;
    reset_rx();
    
    for (i=0;i<8;i++) {
        micro_delay(500);
        mac_addr |= (cc2500_status_rssi() & 0x1);
        mac_addr <<= 1;
    }
}

int16_t mac_send(uint8_t* data, int16_t len, uint8_t to) {
    uint8_t tx_len, state;
    if (len > 59) {
        return 0;
    }
    cc2500_strobe_idle();
    
    tx_len = len + 2;
    cc2500_write_fifo(&tx_len, 1);
    cc2500_write_fifo((uint8_t*)&mac_addr, 1);
    cc2500_write_fifo((uint8_t*)&to, 1);
    cc2500_write_fifo(data, len);
    
    cc2500_gdo0_int_set_cb(tx_cb);
    cc2500_strobe_tx();
    
    state = cc2500_strobe_nop();
    if ( (state & CC2500_NOP_STATE_MASK) == CC2500_NOP_STATE_RX ) {
        return 0;
    }
    
    return len;
}

void mac_set_rx_cb(int16_t (*cb) (uint8_t* data, int16_t len, uint8_t from)) {
    mac_rx_cb = cb;
}

void mac_set_tx_cb(int16_t (*cb) (int16_t success)) {
    mac_tx_cb = cb;
}

static void reset_rx() {
    uint8_t state;
    state = cc2500_strobe_nop();

    force_idle();

    cc2500_strobe_flush_rx();
    cc2500_strobe_flush_tx();
    
    force_calib();

    cc2500_gdo0_int_clear();
    cc2500_gdo0_int_set_falling();
    cc2500_gdo0_int_enable();
    cc2500_gdo0_int_set_cb(rx_cb);

    cc2500_strobe_rx();
}


static int16_t rx_cb() {
    if (! (cc2500_status_crclqi() & CC2500_CRC_MASK) ) {
        reset_rx();
        return 0;
    }
    
    uint8_t len;
    cc2500_read_fifo(&len, 1);
    if (len > 63 || len < 2) {
        reset_rx();
        return 0;
    }
    
    rx_msg.len = len;
    
    cc2500_read_fifo((uint8_t*)&rx_msg.src, rx_msg.len);
    
    uint8_t data[2];
    cc2500_read_fifo(data, 2);
    
    reset_rx();
    
    if (rx_msg.dst == mac_addr || rx_msg.dst == MAC_BROADCAST) {
    
        if (mac_rx_cb) {
            return mac_rx_cb(rx_msg.msg, (int16_t) rx_msg.len - 2, rx_msg.src);
        }
    }
    return 0;
}

static int16_t tx_cb() {
    reset_rx();
    
    if (mac_tx_cb) {
        return mac_tx_cb(1);
    }
    
    return 0;
}

static void force_idle(void) {
    switch (cc2500_strobe_nop() & CC2500_NOP_STATE_MASK) {
        case CC2500_NOP_STATE_RXOVER:
            cc2500_strobe_flush_rx();
            break;
        case CC2500_NOP_STATE_TXUNDER:
            cc2500_strobe_flush_tx();
            break;
    }
    
    cc2500_strobe_idle();
    do {} while ((cc2500_strobe_nop() & CC2500_NOP_STATE_MASK)!=CC2500_NOP_STATE_IDLE);
}

static void force_calib(void) {
    cc2500_strobe_calib();
    do {} while ((cc2500_strobe_nop() & CC2500_NOP_STATE_MASK)!=CC2500_NOP_STATE_IDLE);
}
