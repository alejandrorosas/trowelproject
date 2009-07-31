#include <io.h>
#include <signal.h>

#include "cc2500.h"
#include "cc2500_globals.h"

#include "spi.h"


#define GDO0_PIN (1<<6)
#define GDO2_PIN (1<<7)

#define ACCESS_READ        0x80
#define ACCESS_READ_burst  0xC0
#define ACCESS_WRITE       0x00
#define ACCESS_WRITE_burst 0x40
#define ACCESS_STATUS      0xC0
#define ACCESS_STROBE      0x00

#define STATE_IDLE 0x0
#define STATE_RX   0x1
#define STATE_TX   0x2
#define STATE_CAL  0x4

static int (*gdo0_cb)(void);
static int (*gdo2_cb)(void);

void inline micro_delay(register unsigned int n) {
    __asm__ __volatile__ (
  "1: \n"
  " dec %[n] \n"
  " jne 1b \n"
        : [n] "+r"(n));
}
/* 
 * SPI local functions
 */
static uint8_t read_reg(uint8_t addr) {
    uint8_t ret;
    spi_radio_select(1);
    spi_write_single(addr | ACCESS_READ);
    ret = spi_read_single();
    spi_radio_select(0);
    return ret;
}
static void read_reg_burst(uint8_t addr, uint8_t *data, int len) {
    uint8_t ret;
    spi_radio_select(1);
    spi_write_single(addr | ACCESS_READ_burst);
    ret = spi_read(data, len, 0x0);
    spi_radio_select(0);
}

static void write_reg(uint8_t addr, uint8_t value) {
    spi_radio_select(1);
    spi_write_single(addr | ACCESS_WRITE);
    spi_write_single(value);
    spi_radio_select(0);
}

static void write_reg_burst(uint8_t addr, uint8_t *data, int len) {
    spi_radio_select(1);
    spi_write_single(addr | ACCESS_WRITE_burst);
    spi_write(data, len, 0x0);
    spi_radio_select(0);
}

static void strobe_cmd(uint8_t cmd) {
    spi_radio_select(1);
    spi_write_single(cmd | ACCESS_STROBE);
    spi_radio_select(0);
}

static uint8_t read_status(uint8_t addr) {
    uint8_t ret;
    spi_radio_select(1);
    spi_write_single(addr | ACCESS_STATUS);
    ret = spi_read_single();
    spi_radio_select(0);
    return ret;
}

/*
 * End SPI local functions
 */

uint8_t cc2500_status(void) {
  strobe_cmd(CC2500_STROBE_SNOP);
  return spi_last_read;
}

void cc2500_init(void) {
  gdo0_cb = 0x0;
  gdo2_cb = 0x0;
  
  P2SEL &= ~(GDO0_PIN | GDO2_PIN);
  P2DIR &= ~(GDO0_PIN | GDO2_PIN);
  P2IE  &= ~(GDO0_PIN | GDO2_PIN);
  
  spi_init();
  
  // reset
  spi_radio_select(1);
  spi_radio_select(0);
  micro_delay(200);
  spi_radio_select(1);
  while (spi_read_somi()) ;
  strobe_cmd(CC2500_STROBE_SRES);
  while (spi_read_somi()) ; 
  spi_radio_select(0);
  
  cc2500_status_partnum();
  cc2500_status_version();
  
  // config
  cc2500_cfg_pqt(4);
  cc2500_cfg_append_status(0);
  cc2500_cfg_mod_format(CC2500_MODULATION_MSK);
}

void cc2500_cmd_reset(void) {
  strobe_cmd(CC2500_STROBE_SRES);
}

void cc2500_cmd_xoff(void) {
  strobe_cmd(CC2500_STROBE_SXOFF);
}

void cc2500_cmd_calibrate(void) {
  strobe_cmd(CC2500_STROBE_SCAL);
}

void cc2500_cmd_rx(void) {
  strobe_cmd(CC2500_STROBE_SRX);
}

void cc2500_cmd_tx(void) {
  strobe_cmd(CC2500_STROBE_STX);
}

void cc2500_cmd_idle(void) {
  strobe_cmd(CC2500_STROBE_SIDLE);
}

void cc2500_cmd_wor(void) {
  strobe_cmd(CC2500_STROBE_SWOR);
}

void cc2500_cmd_pwd(void) {
  strobe_cmd(CC2500_STROBE_SPWD);
}

void cc2500_cmd_flush_rx(void) {
  strobe_cmd(CC2500_STROBE_SFRX);
}

void cc2500_cmd_flush_tx(void) {
  strobe_cmd(CC2500_STROBE_SFTX);
}

void cc2500_cmd_reset_wor(void) {
  strobe_cmd(CC2500_STROBE_SWORRST);
}

void cc2500_cmd_nop(void) {
  strobe_cmd(CC2500_STROBE_SNOP);
}

void cc2500_fifo_put(uint8_t* buffer, uint16_t length) {
  write_reg_burst(CC2500_DATA_FIFO_ADDR, buffer, length);
}

void cc2500_fifo_get(uint8_t* buffer, uint16_t length) {
  read_reg_burst(CC2500_DATA_FIFO_ADDR, buffer, length);
}

void cc2500_cfg_patable(uint8_t table[], uint16_t length) {
  length = (length > 8) ? 8 : length;
  write_reg_burst(CC2500_PATABLE_ADDR, table, length);
}

void cc2500_cfg_gdo0(uint8_t cfg) {
  write_reg(CC2500_REG_IOCFG0, (cfg&0x3F));
}

void cc2500_cfg_gdo2(uint8_t cfg) {
  write_reg(CC2500_REG_IOCFG2, (cfg&0x3F));
}

void cc2500_cfg_fifo_thr(uint8_t cfg) {
  write_reg(CC2500_REG_FIFOTHR, (cfg&0x0F));
}

void cc2500_cfg_packet_length(uint8_t cfg) {
  write_reg(CC2500_REG_PKTLEN, (cfg));
}

void cc2500_cfg_pqt(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_PKTCTRL1);
  reg = (reg & 0x1F) | ((cfg << 5) & 0xE0);
  write_reg(CC2500_REG_PKTCTRL1, reg);
}

void cc2500_cfg_crc_autoflush(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_PKTCTRL1);
  reg = (reg & 0xF7) | ((cfg << 3) & 0x08);
  write_reg(CC2500_REG_PKTCTRL1, reg);
}

void cc2500_cfg_append_status(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_PKTCTRL1);
  reg = (reg & 0xFB) | ((cfg << 2) & 0x04);
  write_reg(CC2500_REG_PKTCTRL1, reg);
}

void cc2500_cfg_adr_check(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_PKTCTRL1);
  reg = (reg & 0xFC) | ((cfg << 0) & 0x03);
  write_reg(CC2500_REG_PKTCTRL1, reg);
}

void cc2500_cfg_white_data(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_PKTCTRL0);
  reg = (reg & 0xBF) | ((cfg << 6) & 0x40);
  write_reg(CC2500_REG_PKTCTRL0, reg);
}

void cc2500_cfg_crc_en(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_PKTCTRL0);
  reg = (reg & 0xFB) | ((cfg << 2) & 0x04);
  write_reg(CC2500_REG_PKTCTRL0, reg);
}

void cc2500_cfg_length_config(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_PKTCTRL0);
  reg = (reg & 0xFC) | ((cfg << 0) & 0x03);
  write_reg(CC2500_REG_PKTCTRL0, reg);
}

void cc2500_cfg_device_addr(uint8_t cfg) {
  write_reg(CC2500_REG_ADDR, cfg);
}

void cc2500_cfg_chan(uint8_t cfg) {
  write_reg(CC2500_REG_CHANNR, cfg);
}

void cc2500_cfg_freq_if(uint8_t cfg) {
  write_reg(CC2500_REG_FSCTRL1, (cfg & 0x1F));
}

void cc2500_cfg_freq(uint32_t cfg) {
  uint8_t reg;
  reg = (uint8_t) ( (cfg>>16)&0xFF );
  write_reg(CC2500_REG_FREQ2, reg);
  reg = (uint8_t) ( (cfg>>8)&0xFF );
  write_reg(CC2500_REG_FREQ1, reg);
  reg = (uint8_t) ( cfg&0xFF );
  write_reg(CC2500_REG_FREQ0, reg);
}

void cc2500_cfg_chanbw_e(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_MDMCFG4);
  reg = (reg & 0x3F) | ((cfg << 6) & 0xC0);
  write_reg(CC2500_REG_MDMCFG4, reg);
}

void cc2500_cfg_chanbw_m(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_MDMCFG4);
  reg = (reg & 0xCF) | ((cfg<<4) & 0x30);
  write_reg(CC2500_REG_MDMCFG4, reg);
}

void cc2500_cfg_drate_e(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_MDMCFG4);
  reg = (reg & 0xF0) | ((cfg) & 0x0F);
  write_reg(CC2500_REG_MDMCFG4, reg);
}

void cc2500_cfg_drate_m(uint8_t cfg) {
  write_reg(CC2500_REG_MDMCFG3, cfg);
}

void cc2500_cfg_mod_format(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_MDMCFG2);
  reg = (reg & 0x8F) | ((cfg << 4) & 0x70);
  write_reg(CC2500_REG_MDMCFG2, reg);
}

void cc2500_cfg_manchester_en(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_MDMCFG2);
  reg = (reg & 0xF7) | ((cfg << 3) & 0x08);
  write_reg(CC2500_REG_MDMCFG2, reg);
}

void cc2500_cfg_sync_mode(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_MDMCFG2);
  reg = (reg & 0xF8) | ((cfg << 0) & 0x07);
  write_reg(CC2500_REG_MDMCFG2, reg);
}

void cc2500_cfg_fec_en(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_MDMCFG1);
  reg = (reg & 0x7F) | ((cfg << 7) & 0x80);
  write_reg(CC2500_REG_MDMCFG1, reg);
}

void cc2500_cfg_num_preamble(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_MDMCFG1);
  reg = (reg & 0x8F) | ((cfg << 4) & 0x70);
  write_reg(CC2500_REG_MDMCFG1, reg);
}

void cc2500_cfg_chanspc_e(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_MDMCFG1);
  reg = (reg & 0xFE) | ((cfg << 0) & 0x01);
  write_reg(CC2500_REG_MDMCFG1, reg);
}

void cc2500_cfg_chanspc_m(uint8_t cfg) {
  write_reg(CC2500_REG_MDMCFG0, cfg);
}

void cc2500_cfg_rx_time_rssi(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_MCSM2);
  reg = (reg & 0xEF) | ((cfg << 4) & 0x10);
  write_reg(CC2500_REG_MCSM2, reg);
}

void cc2500_cfg_rx_time(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_MCSM2);
  reg = (reg & 0xF8) | ((cfg << 0) & 0x07);
  write_reg(CC2500_REG_MCSM2, reg);
}

void cc2500_cfg_cca_mode(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_MCSM1);
  reg = (reg & 0xCF) | ((cfg << 4) & 0x30);
  write_reg(CC2500_REG_MCSM1, reg);
}

void cc2500_cfg_rxoff_mode(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_MCSM1);
  reg = (reg & 0xF3) | ((cfg << 2) & 0x0C);
  write_reg(CC2500_REG_MCSM1, reg);
}

void cc2500_cfg_txoff_mode(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_MCSM1);
  reg = (reg & 0xFC) | ((cfg << 0) & 0x03);
  write_reg(CC2500_REG_MCSM1, reg);
}

void cc2500_cfg_fs_autocal(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_MCSM0);
  reg = (reg & 0xCF) | ((cfg << 4) & 0x30);
  write_reg(CC2500_REG_MCSM0, reg);
}

void cc2500_cfg_carrier_sense_rel_thr(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_AGCCTRL1);
  reg = (reg & 0xCF) | ((cfg << 4) & 0x30);
  write_reg(CC2500_REG_AGCCTRL1, reg);
}

void cc2500_cfg_carrier_sense_abs_thr(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_AGCCTRL1);
  reg = (reg & 0xF0) | ((cfg << 0) & 0x0F);
  write_reg(CC2500_REG_AGCCTRL1, reg);
}

void cc2500_cfg_event0(uint16_t cfg) {
  uint8_t reg;
  reg = (uint8_t)((cfg >> 8) & 0x00FF);
  write_reg(CC2500_REG_WOREVT1, cfg);
  reg = (uint8_t)((cfg) & 0x00FF);
  write_reg(CC2500_REG_WOREVT0, cfg);
}

void cc2500_cfg_rc_pd(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_WORCTRL);
  reg = (reg & 0x7F) | ((cfg << 7) & 0x80);
  write_reg(CC2500_REG_WORCTRL, reg);
}

void cc2500_cfg_event1(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_WORCTRL);
  reg = (reg & 0x8F) | ((cfg << 4) & 0x70);
  write_reg(CC2500_REG_WORCTRL, reg);
}

void cc2500_cfg_wor_res(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_WORCTRL);
  reg = (reg & 0xFC) | ((cfg << 0) & 0x03);
  write_reg(CC2500_REG_WORCTRL, reg);
}

void cc2500_cfg_pa_power(uint8_t cfg) {
  uint8_t reg;
  reg = read_reg(CC2500_REG_FREND0);
  reg = (reg & 0xF8) | ((cfg << 0) & 0x07);
  write_reg(CC2500_REG_FREND0, reg);
}

uint8_t cc2500_status_partnum(void) {
  return read_status(CC2500_REG_PARTNUM);
}
uint8_t cc2500_status_version(void) {
  return read_status(CC2500_REG_VERSION);
}
uint8_t cc2500_status_crc_lqi(void) {
  return read_status(CC2500_REG_LQI);
}

uint8_t cc2500_status_rssi(void) {
  return read_status(CC2500_REG_RSSI);
}

uint8_t cc2500_status_marcstate(void) {
  return read_status(CC2500_REG_MARCSTATE);
}

uint8_t cc2500_status_wortime1(void) {
  return read_status(CC2500_REG_WORTIME1);
}

uint8_t cc2500_status_wortime0(void) {
  return read_status(CC2500_REG_WORTIME0);
}

uint8_t cc2500_status_pktstatus(void) {
  return read_status(CC2500_REG_PKTSTATUS);
}

uint8_t cc2500_status_txbytes(void) {
  return read_status(CC2500_REG_TXBYTES);
}

uint8_t cc2500_status_rxbytes(void) {
  return read_status(CC2500_REG_RXBYTES);
}

void cc2500_gdo0_int_enable(void) {
  P2IE |= GDO0_PIN;
}

void cc2500_gdo0_int_disable(void) {
  P2IE &= ~GDO0_PIN;
}

void cc2500_gdo0_int_clear(void) {
  P2IFG &= ~GDO0_PIN;
}

void cc2500_gdo0_int_set_falling_edge(void) {
  P2IES |=  GDO0_PIN;
}

void cc2500_gdo0_int_set_rising_edge(void) {
  P2IES &= ~GDO0_PIN;
}

uint16_t cc2500_gdo0_read(void) {
  return P2IN & GDO0_PIN;
}

void cc2500_gdo0_register_callback(int (*cb)(void)) {
  gdo0_cb = cb;
}


void cc2500_gdo2_int_enable(void) {
  P2IE |= GDO2_PIN;
}

void cc2500_gdo2_int_disable(void) {
  P2IE &= ~GDO0_PIN;
}

void cc2500_gdo2_int_clear(void) {
  P2IFG &= ~GDO0_PIN;
}

void cc2500_gdo2_int_set_falling_edge(void) {
  P2IES |=  GDO2_PIN;
}

void cc2500_gdo2_int_set_rising_edge(void) {
  P2IES &= ~GDO2_PIN;
}

uint16_t cc2500_gdo2_read(void) {
  return P2IN & GDO2_PIN;
}

void cc2500_gdo2_register_callback(int (*cb)(void)) {
  gdo2_cb = cb;
}

void port2irq(void);
/**
 * Interrupt service routine for PORT2.
 * Used for handling CC2500 interrupts triggered on
 * the GDOx pins.
 */
interrupt(PORT2_VECTOR) port1irq(void) {
  if (P2IFG & GDO0_PIN) {
    P2IFG &= ~GDO0_PIN;
    if (gdo0_cb != 0x0) {
      if (gdo0_cb()) {
          LPM4_EXIT;
      }
    }
  }

  if (P2IFG & GDO2_PIN) {
    P2IFG &= ~GDO2_PIN;
    if (gdo2_cb != 0x0) {
      if (gdo2_cb()) {
          LPM4_EXIT;
      }
    }
  } 
}
