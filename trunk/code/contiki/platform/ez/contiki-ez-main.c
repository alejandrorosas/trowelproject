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

#include "net/mac/framer-nullmac.h"
#include "net/mac/framer.h"

#include "net/rime.h"

#include "sys/autostart.h"

#define MAC_DRIVER ezmac_driver

extern const struct mac_driver MAC_DRIVER;
uint8_t ds2411_id[8] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07 };

/*---------------------------------------------------------------------------*/
static void set_rime_addr(void) {
	rimeaddr_t addr;
	int i;

	memset(&addr, 0, sizeof(rimeaddr_t));

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
	leds_off(LEDS_ALL);
	leds_on(LEDS_RED);

	uart_init();

	leds_on(LEDS_GREEN);
	leds_off(LEDS_RED);
	rtimer_init();
	/*
	 * Hardware initialization done!
	 */

	/* Restore node id if such has been stored in external mem */
	random_init((ds2411_id[0]<<8) + ds2411_id[1]);

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

	leds_off(LEDS_GREEN);

	print_processes(autostart_processes);
	autostart_start(autostart_processes);

	leds_on(LEDS_ALL);

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
