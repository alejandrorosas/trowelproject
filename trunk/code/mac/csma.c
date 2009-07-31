#include <io.h>
#include <string.h>
#include "csma.h"
#include "cc2500.h"

static void reset_rx();

static int rx_cb();
static int fifo_cb();
static int tx_cb();

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
    cc2500_init();
    
    cc2500_gdo0_int_disable();
    cc2500_gdo2_int_disable();
    
    rx_ready = 0;
    reset_rx();
    
    //~ csma_addr = (cc2500_status_rssi()<<8) + cc2500_status_rssi();
}
static uint8_t tx_fifo_bytes;

int csma_send(uint16_t dest_addr, uint8_t* data, int len) {
    uint8_t tx_len;
    if (len > 59) {
        return 0;
    }
    
    tx_fifo_bytes = cc2500_read_reg(CC2500_REG_TXBYTES);
    
    tx_len = len + 5;
    cc2500_write_fifo(&tx_len, 1);
    cc2500_write_fifo((uint8_t*)&csma_addr, 2);
    cc2500_write_fifo((uint8_t*)&dest_addr, 2);
    cc2500_write_fifo(data, len);
    
    tx_fifo_bytes = cc2500_read_reg(CC2500_REG_TXBYTES);
    
    cc2500_gdo0_int_set_cb(tx_cb);
    cc2500_cmd_strobe(CC2500_STROBE_STX);
    
    if ( (cc2500_cmd_strobe(CC2500_STROBE_SNOP) & 0x60) == 0 ) {
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
  cc2500_cmd_strobe(CC2500_STROBE_SIDLE);
  cc2500_cmd_strobe(CC2500_STROBE_SFRX);
  cc2500_cmd_strobe(CC2500_STROBE_SCAL);
  
  cc2500_gdo0_int_clear();
  cc2500_gdo0_int_set_falling();
  cc2500_gdo0_int_enable();
  cc2500_gdo0_int_set_cb(rx_cb);
  
  cc2500_cmd_strobe(CC2500_STROBE_SRX);
}


static int rx_cb() {
    //~ if (! (cc2500_status_crc_lqi() & 0x80) ) {
        //~ reset_rx();
        //~ return 0;
    //~ }
    //~ 
    //~ uint8_t len;
    //~ cc2500_fifo_get(&len, 1);
    //~ if (len > 63) {
        //~ reset_rx();
        //~ return 0;
    //~ }
    //~ 
    //~ rx_msg.len = len;
    //~ 
    //~ cc2500_fifo_get((uint8_t*)&rx_msg.src, rx_msg.len);
    //~ rx_ready = 1;
    //~ 
    //~ if (csma_rx_cb) {
        //~ return csma_rx_cb();
    //~ }
    //~ 
    //~ reset_rx();
    
    return 0;
}

static int fifo_cb() {
    reset_rx();
    return 0;
}

static int tx_cb() {
    reset_rx();
    
    if (csma_tx_cb) {
        return csma_tx_cb();
    }
    
    return 0;
}
