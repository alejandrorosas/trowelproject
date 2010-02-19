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
 * @(#)$Id: contiki-sky-main.c,v 1.56 2009/06/22 11:14:11 nifi Exp $
 */

#include <signal.h>
#include <stdio.h>
#include <string.h>

#include <io.h>

#include "contiki.h"

#include "dev/leds.h"
#include "dev/uart.h"
#include "dev/serial-line.h"
#include "dev/watchdog.h"

#include "lib/random.h"

#include "net/mac/frame802154.h"
#include "net/sicslowpan.h"
#include "net/uip-netif.h"
#include "net/mac/ezmac.h"

#include "sys/autostart.h"
#include "sys/profile.h"

extern const struct mac_driver MAC_DRIVER;
const struct radio_driver cc2500_driver;
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
/*---------------------------------------------------------------------------*/
int main(int argc, char **argv) {
	/*
	 * Initalize hardware.
	 */
	msp430_cpu_init();
	clock_init();
	leds_init();
	leds_on(LEDS_RED);

	uart_init(); /* Must come before first printf */

	leds_on(LEDS_GREEN);
	leds_off(LEDS_RED);
	rtimer_init();

	random_init(0x1234);

	/*
	 * Initialize Contiki and our processes.
	 */
	process_init();
	process_start(&etimer_process, NULL);

	printf(CONTIKI_VERSION_STRING " started. ");

	uip_lladdr.addr[0] = 0x12;
	uip_lladdr.addr[1] = 0x34;
	uip_lladdr.addr[2] = 0x56;
	uip_lladdr.addr[3] = 0x78;
	uip_lladdr.addr[4] = 0x9A;
	uip_lladdr.addr[5] = 0xBC;
	uip_lladdr.addr[6] = 0xDE;
	uip_lladdr.addr[7] = 0xF0;

	sicslowpan_init(ezmac_init(NULL));
	process_start(&tcpip_process, NULL);

	// this should be removed some day TODO
	uart_set_input(serial_line_input_byte);
	serial_line_init();

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

			watchdog_stop();
			LPM1;
			/* _BIS_SR(GIE | SCG0 | SCG1 | CPUOFF); LPM3 sleep. This
			 statement will block
			 until the CPU is
			 woken up by an
			 interrupt that sets
			 the wake up flag. */

			watchdog_start();
		}
	}

	return 0;
}
