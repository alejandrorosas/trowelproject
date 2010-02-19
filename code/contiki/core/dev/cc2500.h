#ifndef _CC2500_H
#define _CC2500_H

/* ------------------------------------------------------------------------------------------------
 *                                          Defines
 * ------------------------------------------------------------------------------------------------
 */

/* configuration registers */
#define CC2500_REG_IOCFG2       0x00      /*  IOCFG2   - GDO2 output pin configuration  */
#define CC2500_REG_IOCFG1       0x01      /*  IOCFG1   - GDO1 output pin configuration  */
#define CC2500_REG_IOCFG0       0x02      /*  IOCFG0   - GDO0 output pin configuration  */
#define CC2500_REG_FIFOTHR      0x03      /*  FIFOTHR  - RX FIFO and TX FIFO thresholds */
#define CC2500_REG_SYNC1        0x04      /*  SYNC1    - Sync word, high byte */
#define CC2500_REG_SYNC0        0x05      /*  SYNC0    - Sync word, low byte */
#define CC2500_REG_PKTLEN       0x06      /*  PKTLEN   - Packet length */
#define CC2500_REG_PKTCTRL1     0x07      /*  PKTCTRL1 - Packet automation control */
#define CC2500_REG_PKTCTRL0     0x08      /*  PKTCTRL0 - Packet automation control */
#define CC2500_REG_ADDR         0x09      /*  ADDR     - Device address */
#define CC2500_REG_CHANNR       0x0A      /*  CHANNR   - Channel number */
#define CC2500_REG_FSCTRL1      0x0B      /*  FSCTRL1  - Frequency synthesizer control */
#define CC2500_REG_FSCTRL0      0x0C      /*  FSCTRL0  - Frequency synthesizer control */
#define CC2500_REG_FREQ2        0x0D      /*  FREQ2    - Frequency control word, high byte */
#define CC2500_REG_FREQ1        0x0E      /*  FREQ1    - Frequency control word, middle byte */
#define CC2500_REG_FREQ0        0x0F      /*  FREQ0    - Frequency control word, low byte */
#define CC2500_REG_MDMCFG4      0x10      /*  MDMCFG4  - Modem configuration */
#define CC2500_REG_MDMCFG3      0x11      /*  MDMCFG3  - Modem configuration */
#define CC2500_REG_MDMCFG2      0x12      /*  MDMCFG2  - Modem configuration */
#define CC2500_REG_MDMCFG1      0x13      /*  MDMCFG1  - Modem configuration */
#define CC2500_REG_MDMCFG0      0x14      /*  MDMCFG0  - Modem configuration */
#define CC2500_REG_DEVIATN      0x15      /*  DEVIATN  - Modem deviation setting */
#define CC2500_REG_MCSM2        0x16      /*  MCSM2    - Main Radio Control State Machine configuration */
#define CC2500_REG_MCSM1        0x17      /*  MCSM1    - Main Radio Control State Machine configuration */
#define CC2500_REG_MCSM0        0x18      /*  MCSM0    - Main Radio Control State Machine configuration */
#define CC2500_REG_FOCCFG       0x19      /*  FOCCFG   - Frequency Offset Compensation configuration */
#define CC2500_REG_BSCFG        0x1A      /*  BSCFG    - Bit Synchronization configuration */
#define CC2500_REG_AGCCTRL2     0x1B      /*  AGCCTRL2 - AGC control */
#define CC2500_REG_AGCCTRL1     0x1C      /*  AGCCTRL1 - AGC control */
#define CC2500_REG_AGCCTRL0     0x1D      /*  AGCCTRL0 - AGC control */
#define CC2500_REG_WOREVT1      0x1E      /*  WOREVT1  - High byte Event0 timeout */
#define CC2500_REG_WOREVT0      0x1F      /*  WOREVT0  - Low byte Event0 timeout */
#define CC2500_REG_WORCTRL      0x20      /*  WORCTRL  - Wake On Radio control */
#define CC2500_REG_FREND1       0x21      /*  FREND1   - Front end RX configuration */
#define CC2500_REG_FREND0       0x22      /*  FREDN0   - Front end TX configuration */
#define CC2500_REG_FSCAL3       0x23      /*  FSCAL3   - Frequency synthesizer calibration */
#define CC2500_REG_FSCAL2       0x24      /*  FSCAL2   - Frequency synthesizer calibration */
#define CC2500_REG_FSCAL1       0x25      /*  FSCAL1   - Frequency synthesizer calibration */
#define CC2500_REG_FSCAL0       0x26      /*  FSCAL0   - Frequency synthesizer calibration */
#define CC2500_REG_RCCTRL1      0x27      /*  RCCTRL1  - RC oscillator configuration */
#define CC2500_REG_RCCTRL0      0x28      /*  RCCTRL0  - RC oscillator configuration */
#define CC2500_REG_FSTEST       0x29      /*  FSTEST   - Frequency synthesizer calibration control */
#define CC2500_REG_PTEST        0x2A      /*  PTEST    - Production test */
#define CC2500_REG_AGCTEST      0x2B      /*  AGCTEST  - AGC test */
#define CC2500_REG_TEST2        0x2C      /*  TEST2    - Various test settings */
#define CC2500_REG_TEST1        0x2D      /*  TEST1    - Various test settings */
#define CC2500_REG_TEST0        0x2E      /*  TEST0    - Various test settings */

/* status registers */
#define CC2500_REG_PARTNUM      0x30      /*  PARTNUM    - Chip ID */
#define CC2500_REG_VERSION      0x31      /*  VERSION    - Chip ID */
#define CC2500_REG_FREQEST      0x32      /*  FREQEST    - Frequency Offset Estimate from demodulator */
#define CC2500_REG_LQI          0x33      /*  LQI        - Demodulator estimate for Link Quality */
#define CC2500_REG_RSSI         0x34      /*  RSSI       - Received signal strength indication */
#define CC2500_REG_MARCSTATE    0x35      /*  MARCSTATE  - Main Radio Control State Machine state */
#define CC2500_REG_WORTIME1     0x36      /*  WORTIME1   - High byte of WOR time */
#define CC2500_REG_WORTIME0     0x37      /*  WORTIME0   - Low byte of WOR time */
#define CC2500_REG_PKTSTATUS    0x38      /*  PKTSTATUS  - Current GDOx status and packet status */
#define CC2500_REG_VCO_VC_DAC   0x39      /*  VCO_VC_DAC - Current setting from PLL calibration module */
#define CC2500_REG_TXBYTES      0x3A      /*  TXBYTES    - Underflow and number of bytes */
#define CC2500_REG_RXBYTES      0x3B      /*  RXBYTES    - Overflow and number of bytes */

/* burst write registers */
#define CC2500_REG_PATABLE      0x3E      /*  PATABLE - PA control settings table */
#define CC2500_REG_TXFIFO       0x3F      /*  TXFIFO  - Transmit FIFO */
#define CC2500_REG_RXFIFO       0x3F      /*  RXFIFO  - Receive FIFO */

/* command strobe registers */
#define CC2500_STROBE_SRES      0x30      /*  SRES    - Reset chip. */
#define CC2500_STROBE_SFSTXON   0x31      /*  SFSTXON - Enable and calibrate frequency synthesizer. */
#define CC2500_STROBE_SXOFF     0x32      /*  SXOFF   - Turn off crystal oscillator. */
#define CC2500_STROBE_SCAL      0x33      /*  SCAL    - Calibrate frequency synthesizer and turn it off. */
#define CC2500_STROBE_SRX       0x34      /*  SRX     - Enable RX. Perform calibration if enabled. */
#define CC2500_STROBE_STX       0x35      /*  STX     - Enable TX. If in RX state, only enable TX if CCA passes. */
#define CC2500_STROBE_SIDLE     0x36      /*  SIDLE   - Exit RX / TX, turn off frequency synthesizer. */
#define CC2500_STROBE_SWOR      0x38      /*  SWOR    - Start automatic RX polling sequence (Wake-on-Radio) */
#define CC2500_STROBE_SPWD      0x39      /*  SPWD    - Enter power down mode when CSn goes high. */
#define CC2500_STROBE_SFRX      0x3A      /*  SFRX    - Flush the RX FIFO buffer. */
#define CC2500_STROBE_SFTX      0x3B      /*  SFTX    - Flush the TX FIFO buffer. */
#define CC2500_STROBE_SWORRST   0x3C      /*  SWORRST - Reset real time clock. */
#define CC2500_STROBE_SNOP      0x3D      /*  SNOP    - No operation. Returns status byte. */

#define CC2500_GDO0_PIN 6
#define CC2500_GDO2_PIN 7
/* ------------------------------------------------------------------------------------------------
 *                                         Prototypes
 * ------------------------------------------------------------------------------------------------
 */
void cc2500_init(uint8_t* init_cfg, int len);

uint8_t cc2500_cmd_strobe(uint8_t addr);

uint8_t cc2500_read_reg(uint8_t addr);
void cc2500_write_reg(uint8_t addr, uint8_t value);

void cc2500_write_fifo(uint8_t * pWriteData, uint8_t len);
void cc2500_read_fifo(uint8_t * pReadData, uint8_t len);

#define cc2500_gdo0_int_enable() P2IE |= BV(CC2500_GDO0_PIN)
#define cc2500_gdo0_int_disable() P2IE &= ~BV(CC2500_GDO0_PIN)
#define cc2500_gdo0_int_set_rising() P2IES &= ~BV(CC2500_GDO0_PIN)
#define cc2500_gdo0_int_set_falling() P2IES |= BV(CC2500_GDO0_PIN)
#define cc2500_gdo0_int_clear() P2IFG &= ~BV(CC2500_GDO0_PIN)
#define cc2500_gdo0_read() P2IN & BV(CC2500_GDO0_PIN)
void cc2500_gdo0_int_set_cb(int(*cb)(void));

#define cc2500_gdo2_int_enable() P2IE |= BV(CC2500_GDO2_PIN)
#define cc2500_gdo2_int_disable() P2IE &= ~BV(CC2500_GDO2_PIN)
#define cc2500_gdo2_int_set_rising() P2IES &= ~BV(CC2500_GDO2_PIN)
#define cc2500_gdo2_int_set_falling() P2IES |= BV(CC2500_GDO2_PIN)
#define cc2500_gdo2_int_clear() P2IFG &= ~BV(CC2500_GDO2_PIN)
#define cc2500_gdo2_read() P2IN & BV(CC2500_GDO2_PIN)
void cc2500_gdo2_int_set_cb(int(*cb)(void));

// Defines for standard commands
#define cc2500_strobe_nop()      cc2500_cmd_strobe(CC2500_STROBE_SNOP)
#define cc2500_strobe_idle()     cc2500_cmd_strobe(CC2500_STROBE_SIDLE)
#define cc2500_strobe_tx()       cc2500_cmd_strobe(CC2500_STROBE_STX)
#define cc2500_strobe_rx()       cc2500_cmd_strobe(CC2500_STROBE_SRX)
#define cc2500_strobe_flush_tx() cc2500_cmd_strobe(CC2500_STROBE_SFTX)
#define cc2500_strobe_flush_rx() cc2500_cmd_strobe(CC2500_STROBE_SFRX)
#define cc2500_strobe_calib()    cc2500_cmd_strobe(CC2500_STROBE_SCAL)

#define CC2500_NOP_STATE_MASK    0x70
#define CC2500_NOP_STATE_IDLE    0x00
#define CC2500_NOP_STATE_RX      0x10
#define CC2500_NOP_STATE_TX      0x20
#define CC2500_NOP_STATE_FSTXON  0x30
#define CC2500_NOP_STATE_CALIB   0x40
#define CC2500_NOP_STATE_SETTL   0x50
#define CC2500_NOP_STATE_RXOVER  0x60
#define CC2500_NOP_STATE_TXUNDER 0x70

// defines for statuses
#define cc2500_status_txbytes() cc2500_read_reg(CC2500_REG_TXBYTES)
#define cc2500_status_rxbytes() cc2500_read_reg(CC2500_REG_RXBYTES)
#define cc2500_status_rssi()    cc2500_read_reg(CC2500_REG_RSSI)
#define cc2500_status_crclqi()  cc2500_read_reg(CC2500_REG_LQI)
#define CC2500_CRC_MASK 0x80

/***********************************************************************/
#endif
