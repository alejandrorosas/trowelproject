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
#include "dev/ezmac.h"
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

#define MAC_BROADCAST 0xFF

// declare the process
PROCESS(ezmac_process, "mac")
;

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


static void (* receiver_callback)(const struct mac_driver *);
static process_event_t event_ezmac_rx;

/*---------------------------------------------------------------------------*/
static int send_packet(void) {
	return 0;
}
/*---------------------------------------------------------------------------*/
static int read_packet(void) {
	return 0;
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
static unsigned short channel_check_interval(void) {
	return 0;
}
/*---------------------------------------------------------------------------*/

const struct mac_driver ezmac_driver = { "ez", ezmac_init, send_packet,
		read_packet, set_receive_function, on, off, channel_check_interval };
/*---------------------------------------------------------------------------*/
const struct mac_driver * ezmac_init(const struct radio_driver *d) {
	// initialize th registers
	cc2500_init((uint8_t*) csma_radio_reg, sizeof(csma_radio_reg) / 2);

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
		}
		// any process must end with this, even if it is never reached.
		PROCESS_END();
	}
