#include <io.h>
#include <string.h>
#include "csma.h"
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
static int rx_cb(void);
static int cca_cb(void);
static int tx_cb(void);

static int (*csma_rx_cb) (void);
static int (*csma_tx_cb) (void);

uint16_t csma_addr = 0xABCD;

static struct {
    uint8_t len;
    uint16_t src,
             dst;
    uint8_t msg[59];
} rx_msg;

static int rx_ready;

void csma_init() {
    cc2500_init((uint8_t*) csma_radio_reg, sizeof(csma_radio_reg)/2);
    
    cc2500_gdo0_int_disable();
    cc2500_gdo2_int_disable();
    
    rx_ready = 0;
    reset_rx();
    
    csma_addr = (cc2500_status_rssi()<<8) + cc2500_status_rssi();
}
static uint8_t tx_fifo_bytes;

int csma_send(uint16_t dest_addr, uint8_t* data, int len) {
    uint8_t tx_len;
    if (len > 59) {
        return 0;
    }
    
    tx_fifo_bytes = cc2500_status_txbytes();
    
    tx_len = len + 5;
    cc2500_write_fifo(&tx_len, 1);
    cc2500_write_fifo((uint8_t*)&csma_addr, 2);
    cc2500_write_fifo((uint8_t*)&dest_addr, 2);
    cc2500_write_fifo(data, len);
    
    tx_fifo_bytes = cc2500_status_txbytes();
    
    cc2500_gdo0_int_set_cb(tx_cb);
    cc2500_cmd_strobe(CC2500_STROBE_STX);
    
    if ( (cc2500_strobe_nop() & CC2500_NOP_STATE_MASK) == CC2500_NOP_STATE_RX ) {
        return 0;
    }
    
    return len;
}

int csma_read(uint16_t* from, uint8_t* data, int len) {
    if (rx_ready == 0) {
        return 0;
    }
    
    if (rx_msg.len-4 > len) {
        return 0;
    }
    
    *from = rx_msg.src;
    memcpy((uint8_t*)data, rx_msg.msg, rx_msg.len-4);
    
    rx_ready = 0;
    
    return rx_msg.len-4;
}

void csma_set_rx_cb(int (*cb) (void)) {
    csma_rx_cb = cb;
}

void csma_set_tx_cb(int (*cb) (void)) {
    csma_tx_cb = cb;
}

static void reset_rx() {
  uint8_t state;
  state = cc2500_strobe_nop();
  
  cc2500_strobe_idle();
  while ( (state & CC2500_NOP_STATE_MASK) != CC2500_NOP_STATE_IDLE) {
    state = cc2500_strobe_nop();
  }
  
  cc2500_strobe_flush_rx();
  cc2500_strobe_flush_tx();
  cc2500_strobe_calib();
  
  state = cc2500_strobe_nop();
  while ( (state & CC2500_NOP_STATE_MASK) != CC2500_NOP_STATE_IDLE) {
    state = cc2500_strobe_nop();
  }
  
  cc2500_gdo0_int_clear();
  cc2500_gdo0_int_set_falling();
  cc2500_gdo0_int_enable();
  cc2500_gdo0_int_set_cb(rx_cb);
  
  cc2500_strobe_rx();
}


static int rx_cb() {
    if (! (cc2500_status_crclqi() & CC2500_CRC_MASK) ) {
        reset_rx();
        return 0;
    }
    
    uint8_t len;
    cc2500_fifo_get(&len, 1);
    if (len > 63) {
        reset_rx();
        return 0;
    }
    
    rx_msg.len = len;
    
    cc2500_read_fifo((uint8_t*)&rx_msg.src, rx_msg.len);
    rx_ready = 1;
    
    uint8_t data[2];
    cc2500_read_fifo(data, 2);
    
    if (csma_rx_cb) {
        return csma_rx_cb();
    }
    reset_rx();
    return 0;
}

static int cca_cb() {
    return 0;
}

static int tx_cb() {
    reset_rx();
    
    if (csma_tx_cb) {
        return csma_tx_cb();
    }
    
    return 0;
}
