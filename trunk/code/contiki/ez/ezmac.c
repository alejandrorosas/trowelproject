/**
 * \file
 *         MAC interface for low-power low-memory EZ430 devices
 *
 * \author
 *         Clément Burin des Roziers
 */

#include <io.h>
#include <string.h>
#include "contiki.h"
#include "net/rime/packetbuf.h"
#include "net/mac/ezmac.h"
#include "dev/cc2500.h"
#include "lib/random.h"

#define DEBUG 0

#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#define PRINTADDR(addr) PRINTF(" %02x%02x:%02x%02x:%02x%02x:%02x%02x ", ((uint8_t *)addr)[0], ((uint8_t *)addr)[1], ((uint8_t *)addr)[2], ((uint8_t *)addr)[3], ((uint8_t *)addr)[4], ((uint8_t *)addr)[5], ((uint8_t *)addr)[6], ((uint8_t *)addr)[7])
#else
#define PRINTF(...)
#define PRINTADDR(addr)
#endif

// declare the process
PROCESS(ezmac_process, "mac")
;

// declare the static functions
static void reset_rx(void);
static int16_t rx_cb(void);
static int16_t tx_cb(void);
static void force_idle(void);
static void force_calib(void);

// declare the static variables
static const uint8_t csma_radio_reg[][2] = { { CC2500_REG_IOCFG2, 0x09 }, // CCA
		{ CC2500_REG_IOCFG0, 0x06 }, // sync-EOP
		{ CC2500_REG_FIFOTHR, 0x07 }, // TX:33b, RX:32b
		{ CC2500_REG_PKTLEN, 0xFF }, { CC2500_REG_PKTCTRL1, 0x04 }, // append status
		{ CC2500_REG_PKTCTRL0, 0x45 }, // whiten, crc, variable length
		{ CC2500_REG_ADDR, 0x00 }, { CC2500_REG_CHANNR, 0x00 }, {
				CC2500_REG_FSCTRL1, 0x0A }, { CC2500_REG_FSCTRL0, 0x00 }, {
				CC2500_REG_FREQ2, 0x5D }, { CC2500_REG_FREQ1, 0x93 }, {
				CC2500_REG_FREQ0, 0xB1 }, { CC2500_REG_MDMCFG4, 0x2D }, {
				CC2500_REG_MDMCFG3, 0x3B }, { CC2500_REG_MDMCFG2, 0x73 }, {
				CC2500_REG_MDMCFG1, 0x22 }, { CC2500_REG_MDMCFG0, 0xF8 }, {
				CC2500_REG_DEVIATN, 0x00 }, { CC2500_REG_MCSM0, 0x08 }, // no auto calib
		{ CC2500_REG_FOCCFG, 0x1D }, { CC2500_REG_BSCFG, 0x1C }, {
				CC2500_REG_AGCCTRL2, 0xC7 }, { CC2500_REG_AGCCTRL1, 0x00 }, {
				CC2500_REG_AGCCTRL0, 0xB0 }, { CC2500_REG_FREND1, 0xB6 }, {
				CC2500_REG_FREND0, 0x10 }, { CC2500_REG_FSCAL3, 0xEA }, {
				CC2500_REG_FSCAL2, 0x0A }, { CC2500_REG_FSCAL1, 0x00 }, {
				CC2500_REG_FSCAL0, 0x11 }, { CC2500_REG_FSTEST, 0x59 }, {
				CC2500_REG_TEST2, 0x88 }, { CC2500_REG_TEST1, 0x31 }, {
				CC2500_REG_TEST0, 0x0B }, };

static const struct radio_driver *radio;
static void (* receiver_callback)(const struct mac_driver *);
static struct {
	uint8_t len, src, dst, msg[59];
} rx_msg;
static process_event_t event_ezmac_rx;
static uint8_t mac_addr;
/*---------------------------------------------------------------------------*/
static int send_packet(void) {
	uint8_t tx_len, to;

	if (packetbuf_totlen() > 59) {
		return 0;
	}
	cc2500_strobe_idle();

	tx_len = packetbuf_totlen() + 2;
	cc2500_write_fifo(&tx_len, 1);
	cc2500_write_fifo((uint8_t*) &mac_addr, 1);
	to = MAC_BROADCAST;
	cc2500_write_fifo((uint8_t*) &to, 1);
	cc2500_write_fifo(packetbuf_hdrptr(), packetbuf_totlen());

	cc2500_gdo0_int_set_cb(tx_cb);
	cc2500_strobe_tx();
	return 0;
}
/*---------------------------------------------------------------------------*/
static int read_packet(void) {
	int16_t len;

	packetbuf_clear();
	len = rx_msg.len;
	memcpy(packetbuf_dataptr(), rx_msg.msg, len);
	packetbuf_set_datalen(len);
	return len;
}
/*---------------------------------------------------------------------------*/
static void set_receive_function(void(* recv)(const struct mac_driver *)) {
	receiver_callback = recv;
}
/*---------------------------------------------------------------------------*/
static int on(void) {
	return 0;
}
/*---------------------------------------------------------------------------*/
static int off(int keep_radio_on) {
	return 0;
}
/*---------------------------------------------------------------------------*/
const struct mac_driver ezmac_driver = { "ez", ezmac_init, send_packet,
		read_packet, set_receive_function, on, off, };
/*---------------------------------------------------------------------------*/
const struct mac_driver * ezmac_init(const struct radio_driver *d) {
	// initialize th registers
	cc2500_init((uint8_t*) csma_radio_reg, sizeof(csma_radio_reg) / 2);

	// initialize the IRQ
	cc2500_gdo0_int_disable();
	cc2500_gdo2_int_disable();

	// start the reception
	reset_rx();

	// allocate the RX event
	event_ezmac_rx = process_alloc_event();

	// start the process
	process_start(&ezmac_process, NULL);

	return &ezmac_driver;
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(ezmac_process, ev, data) {
	// any process must start with this.
	PROCESS_BEGIN();
		while (1) {
			// wait here for an event to happen
			PROCESS_WAIT_EVENT_UNTIL(ev==event_ezmac_rx);
			if(receiver_callback != NULL) {
				receiver_callback(&ezmac_driver);
			}
			// any process must end with this, even if it is never reached.
			PROCESS_END();
		}
	}

static void reset_rx(void) {
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
	do {
	} while ((cc2500_strobe_nop() & CC2500_NOP_STATE_MASK)
			!= CC2500_NOP_STATE_IDLE);
}

static void force_calib(void) {
	cc2500_strobe_calib();
	do {
	} while ((cc2500_strobe_nop() & CC2500_NOP_STATE_MASK)
			!= CC2500_NOP_STATE_IDLE);
}

static int16_t rx_cb() {
	if (!(cc2500_status_crclqi() & CC2500_CRC_MASK)) {
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

	cc2500_read_fifo((uint8_t*) &rx_msg.src, rx_msg.len);

	uint8_t data[2];
	cc2500_read_fifo(data, 2);

	reset_rx();

	if (rx_msg.dst == mac_addr || rx_msg.dst == MAC_BROADCAST) {
		process_post(&ezmac_process, event_ezmac_rx, 0x0);
	}
	return 0;
}

static int16_t tx_cb() {
	reset_rx();
	return 0;
}
