#ifndef _CC2500_H_
#define _CC2500_H_

// standard
/**
 * \brief radio initialization procedure
 */
void cc2500_init(void);

/**
 * \brief Re-initialize the SPI driver only
 */
void cc2500_reinit(void);

/**
 * \brief Get the CC2500 chip status
 * /return the status
 */
uint8_t cc2500_status(void);

// commands

/**
 * \brief force chip to reset all registers to default state
 */
void cc2500_cmd_reset(void);

/**
 * \brief stop cristal oscillator
 */
void cc2500_cmd_xoff(void);

/**
 * \brief calibrate frequency synthetizer
 */
void cc2500_cmd_calibrate(void);

/**
 * \brief enable rx.
 */
void cc2500_cmd_rx(void);

/**
 * \brief enable tx. if in rx with cca enabled, go to tx if channel clear
 */
void cc2500_cmd_tx(void);

/**
 * \brief stop tx/rx/calibration/wor
 */
void cc2500_cmd_idle(void);

/**
 * \brief start wake-on-radio : periodic channel sampling with RX polling
 */
void cc2500_cmd_wor(void);

/**
 * \brief enter power down
 */
void cc2500_cmd_pwd(void);

/**
 * \brief flush RX FIFO
 */
void cc2500_cmd_flush_rx(void);

/**
 * \brief flush TX FIFO
 */
void cc2500_cmd_flush_tx(void);

/**
 * \brief reset real time clock to Event1 value for WOR
 */
void cc2500_cmd_reset_wor(void);

/**
 * \brief does nothing, update status byte
 */
void cc2500_cmd_nop(void);

// FIFO access

/**
 * \brief copy a buffer to the radio TX FIFO
 * \param buffer a pointer to the buffer
 * \param length the number of bytes to copy
 */
void cc2500_fifo_put(uint8_t* buffer, uint16_t length);

/**
 * \brief copy the content of the radio RX FIFO to a buffer
 * \param buffer a pointer to the buffer
 * \param length the number of bytes to copy
 **/
void cc2500_fifo_get(uint8_t* buffer, uint16_t length);


// Power Table Config
/**
 * \brief configure the radio chip with the given power table
 * \param table pointer to the array containing the values
 * \param length number of elements (max = 8)
 */
void cc2500_cfg_patable(uint8_t table[], uint16_t length);

/**
 * \name GDOx configuration constants
 * @{
 */
#define CC2500_GDOx_RX_FIFO           0x00  /* assert above threshold, deassert when below         */
#define CC2500_GDOx_RX_FIFO_EOP       0x01  /* assert above threshold or EOP, deassert when empty  */
#define CC2500_GDOx_TX_FIFO           0x02  /* assert above threshold, deassert when below         */
#define CC2500_GDOx_TX_THR_FULL       0x03  /* asserts TX FIFO full. De-asserts when below thr     */
#define CC2500_GDOx_RX_OVER           0x04  /* asserts when RX overflow, deassert when flushed     */
#define CC2500_GDOx_TX_UNDER          0x05  /* asserts when RX underflow, deassert when flushed    */
#define CC2500_GDOx_SYNC_WORD         0x06  /* assert SYNC sent/recv, deasserts on EOP             */
                                            /* In RX, de-assert on overflow or bad address         */
                                            /* In TX, de-assert on underflow                       */
#define CC2500_GDOx_RX_OK             0x07  /* assert when RX PKT with CRC ok, de-assert on 1byte  */
                                            /* read from RX Fifo                                   */
#define CC2500_GDOx_PREAMB_OK         0x08  /* assert when preamble quality reached : PQI/PQT ok   */
#define CC2500_GDOx_CCA               0x09  /* Clear channel assessment. High when RSSI level is   */
                                            /* below threshold (dependent on the current CCA_MODE) */

#define CC2500_GDOx_CLK_XOSC_192      0x3F /* CLK_XOSC/192 */
/**
 * @}
 */

/**
 * \brief Configure the gdo0 output pin.
 * 
 * Example : use 0x06 for sync/eop or 0x0 for rx fifo threshold
 * \param cfg the configuration value
 */
void cc2500_cfg_gdo0(uint8_t cfg);

/**
 * \brief Configure the gdo2 output pin.
 * 
 * Example : use 0x06 for sync/eop or 0x0 for rx fifo threshold
 * \param cfg the configuration value
 */
void cc2500_cfg_gdo2(uint8_t cfg);

/**
 * \brief Set the threshold for both RX and TX FIFOs.
 * corresponding values are : 
 * 
 * value   0 |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 | 10 | 11 | 12 | 13 | 14 | 15 \n
 * TX     61 | 57 | 53 | 49 | 45 | 41 | 37 | 33 | 29 | 25 | 21 | 17 | 13 |  9 |  5 | 1 \n
 * RX      4 |  8 | 12 | 16 | 20 | 24 | 28 | 32 | 36 | 40 | 44 | 48 | 52 | 56 | 60 | 64
 * 
 * \param cfg the configuration value
 */
void cc2500_cfg_fifo_thr(uint8_t cfg);

/**
 * \brief Set the packet length in fixed packet length mode
 * or the maximum packet length in variable length mode
 * \param cfg the configuration value
 */
void cc2500_cfg_packet_length(uint8_t cfg);

/**
 * \brief Set the preamble quality estimator threshold
 * (values are 0-7)
 * \param cfg the configuration value
 */
void cc2500_cfg_pqt(uint8_t cfg);

/**
 * \name CRC Autoflush configuration constants
 * @{
 */
#define CC2500_CRC_AUTOFLUSH_ENABLE  0x1
#define CC2500_CRC_AUTOFLUSH_DISABLE 0x0
/**
 * @}
 */

/**
 * \brief enable/disable the automatic flush of RX FIFO when CRC is not OK
 * \param cfg the configuration value
 */
void cc2500_cfg_crc_autoflush(uint8_t cfg);

/**
 * \name Append status configuration constants
 * @{
 */
#define CC2500_APPEND_STATUS_ENABLE  0x1
#define CC2500_APPEND_STATUS_DISABLE 0x0
/**
 * @}
 */

/**
 * \brief enable/disable the appending of 2 information bytes at the end of
 * a received packet.
 * 
 * Two extra bytes need to be read from the RXFIFO if the appending is set.
 * The first contains the RSSI of the received signal, the second contains
 * the CRC result on the most significant bit, and the LQI on the 7 others.
 * \param cfg the configuration value
 */
void cc2500_cfg_append_status(uint8_t cfg);

/**
 * \name Address check configuration constants
 * @{
 */
#define CC2500_ADDR_NO_CHECK                 0x0
#define CC2500_ADDR_CHECK_NO_BROADCAST       0x1
#define CC2500_ADDR_CHECK_BROADCAST_0        0x2
#define CC2500_ADDR_CHECK_NO_BROADCAST_0_255 0x3
/**
 * @}
 */

/**
 * \brief control the address check mode
 * \param cfg the configuration value
 */
void cc2500_cfg_adr_check(uint8_t cfg);

/**
 * \name Data whitening configuration constants
 * @{
 */
#define CC2500_DATA_WHITENING_ENABLE  0x1
#define CC2500_DATA_WHITENING_DISABLE 0x0
/**
 * @}
 */

/**
 * \brief turn data whitening on/off
 * \param cfg the configuration value
 */
void cc2500_cfg_white_data(uint8_t cfg);


/**
 * \name CRC calculation configuration constants
 * @{
 */
#define CC2500_CRC_CALCULATION_ENABLE  0x1
#define CC2500_CRC_CALCULATION_DISABLE 0x0
/**
 * @}
 */

/**
 * \brief turn CRC calculation on/off
 * \param cfg the configuration value
 */
void cc2500_cfg_crc_en(uint8_t cfg);

/**
 * \name Packet length configuration constants
 * @{
 */
#define CC2500_PACKET_LENGTH_FIXED    0x0
#define CC2500_PACKET_LENGTH_VARIABLE 0x1
#define CC2500_PACKET_LENGTH_INFINITE 0x2
/**
 * @}
 */

/**
 * \brief configure the packet length mode
 * \param cfg the configuration value
 */
void cc2500_cfg_length_config(uint8_t cfg);

/**
 * \brief Set the device address for packet filtration
 * \param cfg the configuration value
 */
void cc2500_cfg_device_addr(uint8_t cfg);

/**
 * \brief Set the channel number.
 * \param cfg the configuration value
 */
void cc2500_cfg_chan(uint8_t cfg);

/**
 * \brief Set the desired IF frequency.
 * (values are 0-31)
 * \param cfg the configuration value
 */
void cc2500_cfg_freq_if(uint8_t cfg);

/**
 * \brief Set the desired base frequency.
 * \param cfg the configuration value (22bits)
 */
void cc2500_cfg_freq(uint32_t cfg);

/**
 * \brief Set the exponent of the channel bandwidth
 * (values are 0-3)
 * \param cfg the configuration value
 */
void cc2500_cfg_chanbw_e(uint8_t cfg);

/**
 * \brief Set mantissa of the channel bandwidth
 * (values are 0-3)
 * \param cfg the configuration value
 */
void cc2500_cfg_chanbw_m(uint8_t cfg);

/**
 * \brief Set the exponent of the data symbol rate
 * (values are 0-16)
 * \param cfg the configuration value
 */
void cc2500_cfg_drate_e(uint8_t cfg);

/**
 * \brief Set the mantissa of the data symbol rate
 * (values are 0-255)
 * \param cfg the configuration value
 */
void cc2500_cfg_drate_m(uint8_t cfg);

/**
 * \name Modulation configuration constants
 * @{
 */
#define CC2500_MODULATION_2FSK 0x00
#define CC2500_MODULATION_GFSK 0x01
#define CC2500_MODULATION_ASK  0x03
#define CC2500_MODULATION_MSK  0x07
/**
 * @}
 */
/**
 * \brief Set the signal modulation
 * \param cfg the configuration value
 */
void cc2500_cfg_mod_format(uint8_t cfg);

/**
 * \name Manchester encoding configuration constants
 * @{
 */
#define CC2500_MANCHESTER_ENABLE  0x1
#define CC2500_MANCHESTER_DISABLE 0x0
/**
 * @}
 */
/**
 * \brief Set manchester encoding on/off
 * \param cfg the configuration value
 */
void cc2500_cfg_manchester_en(uint8_t cfg);


/**
 * \name Sync mode configuration constants
 * @{
 */
#define CC2500_SYNCMODE_NO_PREAMB      0x0
#define CC2500_SYNCMODE_15_16          0x1
#define CC2500_SYNCMODE_16_16          0x2
#define CC2500_SYNCMODE_30_32          0x3
#define CC2500_SYNCMODE_NO_PREAMB_CS   0x4
#define CC2500_SYNCMODE_15_16_CS       0x5
#define CC2500_SYNCMODE_16_16_CS       0x6
#define CC2500_SYNCMODE_30_32_CS       0x7
/**
 * @}
 */
/**
 * \brief select the sync-word qualifier mode
 * \param cfg the configuration value
 */
void cc2500_cfg_sync_mode(uint8_t cfg);

/**
 * \name FEC configuration constants
 * @{
 */
#define CC2500_FEC_ENABLE  0x1
#define CC2500_FEC_DISABLE 0x0
/**
 * @}
 */
/**
 * \brief Set forward error correction on/off
 * supported in fixed packet length mode only
 * \param cfg the configuration value
 */
void cc2500_cfg_fec_en(uint8_t cfg);

/**
 * \brief Set the minimum number of preamble bytes to be tramsitted \n
 * Setting :      0  |  1  |  2  |  3  |  4  |  5  |  6  |  7 \n
 * nb. of bytes : 2  |  3  |  4  |  6  |  8  |  12 |  16 |  24 
 * \param cfg the configuration value
 */
void cc2500_cfg_num_preamble(uint8_t cfg);

/**
 * \brief Set the channel spacing exponent
 * (values are 0-3)
 * \param cfg the configuration value
 */
void cc2500_cfg_chanspc_e(uint8_t cfg);

/**
 * \brief Set the channel spacing mantissa
 * (values are 0-255)
 * \param cfg the configuration value
 */
void cc2500_cfg_chanspc_m(uint8_t cfg);


/**
 * \name RC oscillator configuration constants
 * @{
 */
#define CC2500_RX_TIME_RSSI_ENABLE  0x1
#define CC2500_RX_TIME_RSSI_DISABLE 0x0
/**
 * @}
 */
/**
 * \brief Set direct RX termination based on rssi measurement
 * \param cfg the configuration value
 */
void cc2500_cfg_rx_time_rssi(uint8_t cfg);

/**
 * \brief Set timeout for syncword search in RX for WOR and normal op
 * (values are 0-7)
 * \param cfg the configuration value
 */
void cc2500_cfg_rx_time(uint8_t cfg);

/**
 * \name CCA mode configuration constants
 * @{
 */
#define CC2500_CCA_MODE_ALWAYS      0x0
#define CC2500_CCA_MODE_RSSI        0x1
#define CC2500_CCA_MODE_PKT_RX      0x2
#define CC2500_CCA_MODE_RSSI_PKT_RX 0x3
/**
 * @}
 */
/**
 * \brief Set the CCA mode reflected in CCA signal
 * \param cfg the configuration value
 */
void cc2500_cfg_cca_mode(uint8_t cfg);

/**
 * \name RXOFF mode configuration constants
 * @{
 */
#define CC2500_RXOFF_MODE_IDLE     0x00
#define CC2500_RXOFF_MODE_FSTXON   0x01 /* freq synth on, ready to Tx */
#define CC2500_RXOFF_MODE_TX       0x02 
#define CC2500_RXOFF_MODE_STAY_RX  0x03
/**
 * @}
 */
/**
 * \brief Set the behavior after a packet RX
 * \param cfg the configuration value
 */
void cc2500_cfg_rxoff_mode(uint8_t cfg);

/**
 * \name TXOFF mode configuration constants
 * @{
 */
#define CC2500_TXOFF_MODE_IDLE     0x00
#define CC2500_TXOFF_MODE_FSTXON   0x01 /* freq synth on, ready to Tx */
#define CC2500_TXOFF_MODE_STAY_TX  0x02
#define CC2500_TXOFF_MODE_RX       0x03
/**
 * @}
 */
/**
 * \brief Set the behavior after packet TX
 * \param cfg the configuration value
 */
void cc2500_cfg_txoff_mode(uint8_t cfg);


/**
 * \name Automatic calibration configuration constants
 * @{
 */
#define CC2500_AUTOCAL_NEVER             0x00
#define CC2500_AUTOCAL_IDLE_TO_TX_RX     0x01
#define CC2500_AUTOCAL_TX_RX_TO_IDLE     0x02
#define CC2500_AUTOCAL_4TH_TX_RX_TO_IDLE 0x03
/**
 * @}
 */
/**
 * \brief Set auto calibration policy
 * \param cfg the configuration value
 */
void cc2500_cfg_fs_autocal(uint8_t cfg);

/**
 * \brief Set the relative threshold for asserting Carrier Sense \n
 * Setting :    0     |  1  |  2   |  3 \n
 * thr     : disabled | 6dB | 10dB | 14dB \n
 * \param cfg the configuration value
 */
void cc2500_cfg_carrier_sense_rel_thr(uint8_t cfg);

/**
 * \brief Set the absolute threshold for asserting Carrier Sense
 * referenced to MAGN_TARGET \n
 * Setting :    -8    |  -7  |  -1  |       0        |  1  |   7 \n
 * thr     : disabled | -7dB | -1dB | at MAGN_TARGET | 1dB |  7dB \n
 * \param cfg the configuration value
 */
void cc2500_cfg_carrier_sense_abs_thr(uint8_t cfg);

/**
 * \brief Set event0 timeout register for WOR operation
 * \param cfg the configuration value
 */
void cc2500_cfg_event0(uint16_t cfg);

/**
 * \name RC oscillator configuration constants
 * @{
 */
#define CC2500_RC_OSC_ENABLE  0x0
#define CC2500_RC_OSC_DISABLE 0x1
/**
 * @}
 */

/**
 * \brief Set the RC oscillator on/off, needed by WOR
 * \param cfg the configuration value
 */
void cc2500_cfg_rc_pd(uint8_t cfg);

/**
 * \brief Set the event1 timeout register
 * \param cfg the configuration value
 */
void cc2500_cfg_event1(uint8_t cfg);

/**
 * \brief Set the WOR resolution
 * \param cfg the configuration value
 */
void cc2500_cfg_wor_res(uint8_t cfg);

/**
 * \brief select the PA power setting, index of the patable
 * \param cfg the configuration value
 */
void cc2500_cfg_pa_power(uint8_t cfg);

// Status Registers access
/**
 * \brief read the partnum register, should be 128
 */
uint8_t cc2500_status_partnum(void);
/**
 * \brief read the version register, should be 3
 */
uint8_t cc2500_status_version(void);
/**
 * \brief read the register containing the last CRC calculation match
 * and LQI estimate
 */
uint8_t cc2500_status_crc_lqi(void);

/**
 * \brief read the RSSI
 */
uint8_t cc2500_status_rssi(void);

/**
 * \brief read the main radio state machine state
 */
uint8_t cc2500_status_marcstate(void);

/**
 * \brief read the high byte of the WOR timer
 */
uint8_t cc2500_status_wortime1(void);

/**
 * \brief read the low byte of the WOR timer 
 */
uint8_t cc2500_status_wortime0(void);

/**
 * \brief read the packet status register
 */
uint8_t cc2500_status_pktstatus(void);

/**
 * \brief read the number of bytes in TX FIFO
 */
uint8_t cc2500_status_txbytes(void);

/**
 * \brief read the number of bytes in RX FIFO
 */
uint8_t cc2500_status_rxbytes(void);


// GDOx int config & access
/**
 * \brief enable interrupt for GDO0
 */
void cc2500_gdo0_int_enable(void);
/**
 * \brief disable interrupt for GDO0
 */
void cc2500_gdo0_int_disable(void);
/**
 * \brief clear interrupt for GDO0
 */
void cc2500_gdo0_int_clear(void);
/**
 * \brief configure interrupt for GDO0 on high to low transition
 */
void cc2500_gdo0_int_set_falling_edge(void);
/**
 * \brief configure interrupt for GDO0 on low to high transition
 */
void cc2500_gdo0_int_set_rising_edge(void);
/**
 * \brief read the state of GDO0
 */
uint16_t cc2500_gdo0_read(void);
/**
 * \brief register a callback function for GDO0 interrupt
 * \param cb a function pointer
 */
void cc2500_gdo0_register_callback(int (*cb)(void));

/**
 * \brief enable interrupt for GDO2
 */
void cc2500_gdo2_int_enable(void);
/**
 * \brief disable interrupt for GDO2
 */
void cc2500_gdo2_int_disable(void);
/**
 * \brief clear interrupt for GDO2
 */
void cc2500_gdo2_int_clear(void);
/**
 * \brief configure interrupt for GDO2 on high to low transition
 */
void cc2500_gdo2_int_set_falling_edge(void);
/**
 * \brief configure interrupt for GDO2 on low to high transition
 */
void cc2500_gdo2_int_set_rising_edge(void);
/**
 * \brief read the state of GDO2
 */
uint16_t cc2500_gdo2_read(void);
/**
 * \brief register a callback function for GDO2 interrupt
 * \param cb a function pointer
 */
void cc2500_gdo2_register_callback(int (*cb)(void));

#endif

/**
 * @}
 */
