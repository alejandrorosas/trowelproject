/*
 * Copyright (c) 2006, Swedish Institute of Computer Science
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * @(#)$Id: contiki-sky-main.c,v 1.68 2010/02/06 10:00:49 adamdunkels Exp $
 */
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include "contiki.h"

#include "dev/leds.h"
#include "dev/ezmac.h"

#include "dev/uart.h"
#include "dev/watchdog.h"

#include "lib/random.h"

#include "net/mac/frame802154.h"
#include "net/mac/framer-802154.h"
#include "net/mac/framer-nullmac.h"
#include "net/mac/framer.h"

#if WITH_UIP6
#include "net/sicslowpan.h"
#include "net/uip-netif.h"
#include "net/mac/sicslowmac.h"
#if UIP_CONF_ROUTER
#include "net/routing/rimeroute.h"
#endif /* UIP_CONF_ROUTER*/
#endif /* WITH_UIP6 */

#include "net/rime.h"

#include "sys/autostart.h"

#ifndef WITH_UIP
#define WITH_UIP 0
#else
#error "UIP6 ONLY"
#endif

#define UIP_OVER_MESH_CHANNEL 8

#ifdef EXPERIMENT_SETUP
#include "experiment-setup.h"
#endif

#define MAC_DRIVER ezmac_driver

#ifndef MAC_CSMA
#ifdef MAC_CONF_CSMA
#define MAC_CSMA MAC_CONF_CSMA
#else
#define MAC_CSMA 1
#endif /* MAC_CONF_CSMA */
#endif /* MAC_CSMA */

extern const struct mac_driver MAC_DRIVER;
uint8_t ds2411_id[8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };

/*---------------------------------------------------------------------------*/
void uip_log(char *msg) {
	puts(msg);
}
/*---------------------------------------------------------------------------*/
static void set_rime_addr(void) {
	rimeaddr_t addr;
	int i;

	memset(&addr, 0, sizeof(rimeaddr_t));
#if UIP_CONF_IPV6
	memcpy(addr.u8, ds2411_id, sizeof(addr.u8));
#endif
	rimeaddr_set_node_addr(&addr);
	printf("Rime started with address ");
	for (i = 0; i < sizeof(addr.u8) - 1; i++) {
		printf("%d.", addr.u8[i]);
	}
	printf("%d\n", addr.u8[i]);
}
/*---------------------------------------------------------------------------*/
static void print_processes(struct process * const processes[]) {
	/*  const struct process * const * p = processes;*/
	printf("Starting");
	while (*processes != NULL) {
		printf(" '%s'", (*processes)->name);
		processes++;
	}
	putchar('\n');
}
/*--------------------------------------------------------------------------*/
int main(int argc, char **argv) {
	/*
	 * Initalize hardware.
	 */
	msp430_cpu_init();
	clock_init();
	leds_init();
	leds_on(LEDS_RED);

	//uart1_init(BAUD2UBR(115200)); /* Must come before first printf */
	uart_init();

	leds_on(LEDS_GREEN);
	//ds2411_init();

	/* XXX hack: Fix it so that the 802.15.4 MAC address is compatible
	 with an Ethernet MAC address - byte 0 (byte 2 in the DS ID)
	 cannot be odd. */
	ds2411_id[2] &= 0xfe;

	leds_on(LEDS_BLUE);

	leds_off(LEDS_RED);
	rtimer_init();
	/*
	 * Hardware initialization done!
	 */

	/* Restore node id if such has been stored in external mem */
	random_init((ds2411_id[0]<<8) + ds2411_id[1]);

	leds_off(LEDS_BLUE);
	/*
	 * Initialize Contiki and our processes.
	 */
	process_init();
	process_start(&etimer_process, NULL);

	ctimer_init();

	ezmac_init(0x0);

	printf(CONTIKI_VERSION_STRING " started. ");
	set_rime_addr();
	printf("MAC %02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x", ds2411_id[0],
			ds2411_id[1], ds2411_id[2], ds2411_id[3], ds2411_id[4],
			ds2411_id[5], ds2411_id[6], ds2411_id[7]);

	framer_set(&framer_802154);

#if WITH_UIP6
	memcpy(&uip_lladdr.addr, ds2411_id, sizeof(uip_lladdr.addr));
	/* Setup nullmac-like MAC for 802.15.4 */
	/*   sicslowpan_init(sicslowmac_init(&cc2420_driver)); */
	/*   printf(" %s channel %u\n", sicslowmac_driver.name, RF_CHANNEL); */

	/* Setup X-MAC for 802.15.4 */
	queuebuf_init();
	sicslowpan_init(&MAC_DRIVER);

	process_start(&tcpip_process, NULL);

	printf("Tentative link-local IPv6 address ");
	{
		int i;
		for(i = 0; i < 7; ++i) {
			printf("%02x%02x:",
					uip_netif_physical_if.addresses[0].ipaddr.u8[i * 2],
					uip_netif_physical_if.addresses[0].ipaddr.u8[i * 2 + 1]);
		}
		printf("%02x%02x\n",
				uip_netif_physical_if.addresses[0].ipaddr.u8[14],
				uip_netif_physical_if.addresses[0].ipaddr.u8[15]);
	}

#if UIP_CONF_ROUTER
	uip_router_register(&rimeroute);
#endif /* UIP_CONF_ROUTER */
#endif /* WITH_UIP6 */

	leds_off(LEDS_GREEN);

	print_processes(autostart_processes);
	autostart_start(autostart_processes);

	/*
	 * This is the scheduler loop.
	 */
	watchdog_start();
	/*  watchdog_stop();*/
	while (1) {
		int r;
		do {
			/* Reset watchdog. */
			watchdog_periodic();
			r = process_run();
		} while (r > 0);

		/*
		 * Idle processing.
		 */
		int s = splhigh(); /* Disable interrupts. */
		/* uart1_active is for avoiding LPM3 when still sending or receiving */
		if (process_nevents() != 0) {
			splx(s);
			/* Re-enable interrupts. */
		} else {
			/* Re-enable interrupts and go to sleep atomically. */
			watchdog_stop();
			_BIS_SR(GIE | SCG0 | SCG1 | CPUOFF);
			/* LPM3 sleep. This
			 statement will block
			 until the CPU is
			 woken up by an
			 interrupt that sets
			 the wake up flag. */

			/* We get the current processing time for interrupts that was
			 done during the LPM and store it for next time around.  */
			watchdog_start();

		}
	}

	return 0;
}
/*---------------------------------------------------------------------------*/
#if LOG_CONF_ENABLED
void
log_message(char *m1, char *m2)
{
	printf("%s%s\n", m1, m2);
}
#endif /* LOG_CONF_ENABLED */
