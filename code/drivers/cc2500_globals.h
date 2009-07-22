#ifndef _CC2500_GLOBALS_H
#define _CC2500_GLOBALS_H


/**
 * \name CC2500 time constants
 * @{
 */
#define CC2500_POWER_UP_DELAY_NS                40000
#define CC2500_POWER_UP_DELAY_US                40
#define CC2500_MANCAL_DELAY_NS                  721000
#define CC2500_MANCAL_DELAY_US                  721
#define CC2500_FS_WAKEUP_DELAY_NS               44200
#define CC2500_FS_WAKEUP_DELAY_US               45
#define CC2500_SETTLING_DELAY_NS                44200
#define CC2500_SETTLING_DELAY_US                44
#define CC2500_CALIBRATE_DELAY_NS               720600
#define CC2500_CALIBRATE_DELAY_US               720
#define CC2500_IDLE_NOCAL_DELAY_NS              100
#define CC2500_IDLE_NOCAL_DELAY_US              1
#define CC2500_TX_RX_DELAY_NS                   21500
#define CC2500_TX_RX_DELAY_US                   22
#define CC2500_RX_TX_DELAY_NS                   9600
#define CC2500_RX_TX_DELAY_US                   10
/**
 * @}
 */

/**
 * \name CC2500 State Machine in status byte
 * @{
 */
#define CC2500_STATUS_IDLE                      0x00
#define CC2500_STATUS_RX                        0x01
#define CC2500_STATUS_TX                        0x02
#define CC2500_STATUS_FSTXON                    0x03
#define CC2500_STATUS_CALIBRATE                 0x04
#define CC2500_STATUS_SETTLING                  0x05
#define CC2500_STATUS_RXFIFO_OVERFLOW           0x06
#define CC2500_STATUS_TXFIFO_UNDERFLOW          0x07
/**
 * @}
 */


/**
 * \name CC2500 internal states
 * @{
 */
#define CC2500_STATE_SLEEP                      0x00
#define CC2500_STATE_IDLE                       0x01
#define CC2500_STATE_XOFF                       0x02
#define CC2500_STATE_MANCAL                     0x03
#define CC2500_STATE_FS_WAKEUP                  0x06
#define CC2500_STATE_FS_CALIBRATE               0x08
#define CC2500_STATE_SETTLING                   0x09
#define CC2500_STATE_CALIBRATE                  0x12
#define CC2500_STATE_RX                         0x13
#define CC2500_STATE_TXRX_SETTLING              0x16
#define CC2500_STATE_RX_OVERFLOW                0x17
#define CC2500_STATE_FSTXON                     0x18
#define CC2500_STATE_TX                         0x19
#define CC2500_STATE_RXTX_SETTLING              0x21
#define CC2500_STATE_TX_UNDERFLOW               0x22
#define CC2500_STATE_IDLING                     0x23
/**
 * @}
 */


/**
 * \name CC2500 RAM & register Access
 * @{
 */
#define CC2500_REG_ACCESS_OP(x)                 (x & 0x80) 
#define CC2500_REG_ACCESS_OP_READ               0x80 
#define CC2500_REG_ACCESS_OP_WRITE              0x00 
#define CC2500_REG_ACCESS_TYPE(x)               (x & 0x40) 
#define CC2500_REG_ACCESS_BURST                 0x40 
#define CC2500_REG_ACCESS_NOBURST               0x00 
#define CC2500_REG_ACCESS_ADDRESS(x)            (x & 0x3F)
/**
 * @}
 */


/**
 * \name C1100 Strobe commands
 * @{
 */
#define CC2500_STROBE_SRES                      0x30 /* reset                */
#define CC2500_STROBE_SFSTXON                   0x31 /* enable and calibrate */
#define CC2500_STROBE_SXOFF                     0x32 /* crystall off         */
#define CC2500_STROBE_SCAL                      0x33 /* calibrate            */
#define CC2500_STROBE_SRX                       0x34 /* enable rx            */
#define CC2500_STROBE_STX                       0x35 /* enable tx            */
#define CC2500_STROBE_SIDLE                     0x36 /* go idle              */
#define CC2500_STROBE_SAFC                      0x37 /* AFC adjustment       */
#define CC2500_STROBE_SWOR                      0x38 /* wake on radio        */
#define CC2500_STROBE_SPWD                      0x39 /* power down           */
#define CC2500_STROBE_SFRX                      0x3A /* flush Rx fifo        */
#define CC2500_STROBE_SFTX                      0x3B /* flush Tx fifo        */
#define CC2500_STROBE_SWORRST                   0x3C /* reset WOR timer      */
#define CC2500_STROBE_SNOP                      0x3D /* no operation         */
/**
 * @}
 */


/**
 * \name C1100 Registers
 * @{
 */
#define CC2500_REG_IOCFG2                       0x00
#define CC2500_REG_IOCFG2_DEFAULT               0x29
#define CC2500_REG_IOCFG1                       0x01
#define CC2500_REG_IOCFG1_DEFAULT               0x2E
#define CC2500_REG_IOCFG0                       0x02
#define CC2500_REG_IOCFG0_DEFAULT               0x3F
#define CC2500_REG_FIFOTHR                      0x03
#define CC2500_REG_FIFOTHR_DEFAULT              0x07
#define CC2500_REG_SYNC1                        0x04
#define CC2500_REG_SYNC1_DEFAULT                0xD3
#define CC2500_REG_SYNC0                        0x05
#define CC2500_REG_SYNC0_DEFAULT                0x91
#define CC2500_REG_PKTLEN                       0x06
#define CC2500_REG_PKTLEN_DEFAULT               0xFF
#define CC2500_REG_PKTCTRL1                     0x07
#define CC2500_REG_PKTCTRL1_DEFAULT             0x04
#define CC2500_REG_PKTCTRL0                     0x08
#define CC2500_REG_PKTCTRL0_DEFAULT             0x45
#define CC2500_REG_ADDR                         0x09
#define CC2500_REG_ADDR_DEFAULT                 0x00
#define CC2500_REG_CHANNR                       0x0A
#define CC2500_REG_CHANNR_DEFAULT               0x00
#define CC2500_REG_FSCTRL1                      0x0B
#define CC2500_REG_FSCTRL1_DEFAULT              0x0F
#define CC2500_REG_FSCTRL0                      0x0C
#define CC2500_REG_FSCTRL0_DEFAULT              0x00
#define CC2500_REG_FREQ2                        0x0D
#define CC2500_REG_FREQ2_DEFAULT                0x1E
#define CC2500_REG_FREQ1                        0x0E
#define CC2500_REG_FREQ1_DEFAULT                0xC4
#define CC2500_REG_FREQ0                        0x0F
#define CC2500_REG_FREQ0_DEFAULT                0xEC
#define CC2500_REG_MDMCFG4                      0x10
#define CC2500_REG_MDMCFG4_DEFAULT              0x8C
#define CC2500_REG_MDMCFG3                      0x11
#define CC2500_REG_MDMCFG3_DEFAULT              0x22
#define CC2500_REG_MDMCFG2                      0x12
#define CC2500_REG_MDMCFG2_DEFAULT              0x02
#define CC2500_REG_MDMCFG1                      0x13
#define CC2500_REG_MDMCFG1_DEFAULT              0x22
#define CC2500_REG_MDMCFG0                      0x14
#define CC2500_REG_MDMCFG0_DEFAULT              0xF8
#define CC2500_REG_DEVIATN                      0x15
#define CC2500_REG_DEVIATN_DEFAULT              0x47
#define CC2500_REG_MCSM2                        0x16
#define CC2500_REG_MCSM2_DEFAULT                0x07
#define CC2500_REG_MCSM1                        0x17
#define CC2500_REG_MCSM1_DEFAULT                0x30
#define CC2500_REG_MCSM0                        0x18
#define CC2500_REG_MCSM0_DEFAULT                0x04
#define CC2500_REG_FOCCFG                       0x19
#define CC2500_REG_FOCCFG_DEFAULT               0x36
#define CC2500_REG_BSCFG                        0x1A
#define CC2500_REG_BSCFG_DEFAULT                0x6C
#define CC2500_REG_AGCCTRL2                     0x1B
#define CC2500_REG_AGCCTRL2_DEFAULT             0x03
#define CC2500_REG_AGCCTRL1                     0x1C
#define CC2500_REG_AGCCTRL1_DEFAULT             0x40
#define CC2500_REG_AGCCTRL0                     0x1D
#define CC2500_REG_AGCCTRL0_DEFAULT             0x91
#define CC2500_REG_WOREVT1                      0x1E
#define CC2500_REG_WOREVT1_DEFAULT              0x87
#define CC2500_REG_WOREVT0                      0x1F
#define CC2500_REG_WOREVT0_DEFAULT              0x6B
#define CC2500_REG_WORCTRL                      0x20
#define CC2500_REG_WORCTRL_DEFAULT              0xF8
#define CC2500_REG_FREND1                       0x21
#define CC2500_REG_FREND1_DEFAULT               0xA6
#define CC2500_REG_FREND0                       0x22
#define CC2500_REG_FREND0_DEFAULT               0x10
#define CC2500_REG_FSCAL3                       0x23
#define CC2500_REG_FSCAL3_DEFAULT               0xA9
#define CC2500_REG_FSCAL2                       0x24
#define CC2500_REG_FSCAL2_DEFAULT               0x0A
#define CC2500_REG_FSCAL1                       0x25
#define CC2500_REG_FSCAL1_DEFAULT               0x20
#define CC2500_REG_FSCAL0                       0x26
#define CC2500_REG_FSCAL0_DEFAULT               0x0D
#define CC2500_REG_RCCTRL1                      0x27
#define CC2500_REG_RCCTRL1_DEFAULT              0x41
#define CC2500_REG_RCCTRL0                      0x28
#define CC2500_REG_RCCTRL0_DEFAULT              0x00
/**
 * @}
 */


/**
 * \name Configuration registers
 * @{
 */
#define CC2500_REG_FSTEST                       0x29
#define CC2500_REG_FSTEST_DEFAULT               0x57
#define CC2500_REG_PTEST                        0x2A
#define CC2500_REG_PTEST_DEFAULT                0x7F
#define CC2500_REG_AGCTEST                      0x2B
#define CC2500_REG_AGCTEST_DEFAULT              0x3F
#define CC2500_REG_TEST2                        0x2B
#define CC2500_REG_TEST2_DEFAULT                0x88
#define CC2500_REG_TEST1                        0x2C
#define CC2500_REG_TEST1_DEFAULT                0x31
#define CC2500_REG_TEST0                        0x2D
#define CC2500_REG_TEST0_DEFAULT                0x0B
/**
 * @}
 */


/**
 * \name Read only registers
 * @{
 */
#define CC2500_REG_PARTNUM                      0x30
#define CC2500_REG_PARTNUM_DEFAULT              0x00
#define CC2500_REG_VERSION                      0x31
#define CC2500_REG_VERSION_DEFAULT              0x01
#define CC2500_REG_FREQEST                      0x32
#define CC2500_REG_FREQEST_DEFAULT              0x00
#define CC2500_REG_LQI                          0x33
#define CC2500_REG_LQI_DEFAULT                  0x7F
#define CC2500_REG_RSSI                         0x34
#define CC2500_REG_RSSI_DEFAULT                 0x80
#define CC2500_REG_MARCSTATE                    0x35
#define CC2500_REG_MARCSTATE_DEFAULT            0x01
#define CC2500_REG_WORTIME1                     0x36
#define CC2500_REG_WORTIME1_DEFAULT             0x00
#define CC2500_REG_WORTIME0                     0x37
#define CC2500_REG_WORTIME0_DEFAULT             0x00
#define CC2500_REG_PKTSTATUS                    0x38
#define CC2500_REG_PKTSTATUS_DEFAULT            0x00
#define CC2500_REG_VCO_VC_DAC                   0x39
#define CC2500_REG_VCO_VC_DAC_DEFAULT           0x94
#define CC2500_REG_TXBYTES                      0x3A
#define CC2500_REG_TXBYTES_DEFAULT              0x00
#define CC2500_REG_RXBYTES                      0x3B
#define CC2500_REG_RXBYTES_DEFAULT              0x00


#define CC2500_PATABLE_ADDR                     0x3E
#define CC2500_DATA_FIFO_ADDR                   0x3F
/**
 * @}
 */


/**
 * \name GDOx configuration
 * @{
 */
#define CC2500_GDOx_RX_FIFO           0x00 /* assert above threshold, deassert when below         */
#define CC2500_GDOx_RX_FIFO_EOP       0x01 /* assert above threshold or EOP                       */
#define CC2500_GDOx_TX_FIFO           0x02 /* assert above threshold, deassert below thr          */
#define CC2500_GDOx_TX_THR_FULL       0x03 /* asserts when TX FIFO is full. De-asserts when       */
					   /* the TX FIFO is drained below TXFIFO_THR.            */
#define CC2500_GDOx_RX_OVER           0x04 /* asserts when RX overflow, deassert when flushed     */
#define CC2500_GDOx_TX_UNDER          0x05 /* asserts when RX underflow, deassert when flushed    */
#define CC2500_GDOx_SYNC_WORD         0x06 /* assert SYNC sent/recv, deasserts on EOP             */
					   /* In RX, de-assert on overflow or bad address         */
					   /* In TX, de-assert on underflow                       */
#define CC2500_GDOx_RX_OK             0x07 /* assert when RX PKT with CRC ok, de-assert on 1byte  */
                                           /* read from RX Fifo                                   */
#define CC2500_GDOx_PREAMB_OK         0x08 /* assert when preamble quality reached : PQI/PQT ok   */
#define CC2500_GDOx_CCA               0x09 /* Clear channel assessment. High when RSSI level is   */
					   /* below threshold (dependent on the current CCA_MODE) */

#define CC2500_GDOx_CHIP_RDY          0x29 /* CHIP_RDY     */

#define CC2500_GDOx_XOSC_STABLE       0x2B /* XOSC_STABLE  */

#define CC2500_GDOx_CLK_XOSC_1        0x30 /* CLK_XOSC/1   */
#define CC2500_GDOx_CLK_XOSC_1p5      0x31 /* CLK_XOSC/1.5 */
#define CC2500_GDOx_CLK_XOSC_2        0x32 /* CLK_XOSC/2   */
#define CC2500_GDOx_CLK_XOSC_3        0x33 /* CLK_XOSC/3   */
#define CC2500_GDOx_CLK_XOSC_4        0x34 /* CLK_XOSC/4   */
#define CC2500_GDOx_CLK_XOSC_6        0x35 /* CLK_XOSC/6   */
#define CC2500_GDOx_CLK_XOSC_8        0x36 /* CLK_XOSC/8   */
#define CC2500_GDOx_CLK_XOSC_12       0x37 /* CLK_XOSC/12  */
#define CC2500_GDOx_CLK_XOSC_16       0x38 /* CLK_XOSC/16  */
#define CC2500_GDOx_CLK_XOSC_24       0x39 /* CLK_XOSC/24  */
#define CC2500_GDOx_CLK_XOSC_32       0x3A /* CLK_XOSC/32  */
#define CC2500_GDOx_CLK_XOSC_48       0x3B /* CLK_XOSC/48  */
#define CC2500_GDOx_CLK_XOSC_64       0x3C /* CLK_XOSC/64  */
#define CC2500_GDOx_CLK_XOSC_96       0x3D /* CLK_XOSC/96  */
#define CC2500_GDOx_CLK_XOSC_128      0x3E /* CLK_XOSC/128 */
#define CC2500_GDOx_CLK_XOSC_192      0x3F /* CLK_XOSC/192 */
/**
 * @}
 */

#endif
