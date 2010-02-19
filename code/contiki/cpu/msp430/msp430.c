/*
 * Copyright (c) 2005, Swedish Institute of Computer Science
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
 * This file is part of the Contiki operating system.
 *
 * @(#)$Id: msp430.c,v 1.10 2009/02/04 18:28:44 joxe Exp $
 */
#include <io.h>
#include <signal.h>
#include <sys/unistd.h>
#include "msp430.h"
#include "dev/watchdog.h"

/*---------------------------------------------------------------------------*/
void msp430_init_dco(void) {
	// configure DC0 12MHz
	DCOCTL = CALDCO_12MHZ;
	BCSCTL1 = CALBC1_12MHZ;
	BCSCTL3 = LFXT1S_2;

	// configure MCLK 12MHz
	BCSCTL2 = (BCSCTL2 & ~SELM_3) | SELM_DCOCLK;
	BCSCTL2 |= DIVM_1;

	// configure SMCLK 12MHz
	BCSCTL2 &= SELS;
	BCSCTL2 |= DIVS_DIV1;

	// configure ACLK 12KHz
	BCSCTL1 &= ~DIVA_3;
	BCSCTL1 |= DIVA_DIV1;

}
/*---------------------------------------------------------------------------*/

static void init_ports(void) {
	P1SEL = 0;
	P1DIR = 0xFC;
	P1OUT = 0;

	P2SEL = 0;
	P2DIR = 0x3F;
	P2OUT = 0;

	P3SEL = 0;
	P3DIR = 0xFF;
	P3OUT = 1;

	P4SEL = 0;
	P4DIR = 0xDF;
	P4OUT = 0;

	P1IE = 0;
	P2IE = 0;
}
/*---------------------------------------------------------------------------*/
/* msp430-ld may align _end incorrectly. Workaround in cpu_init. */
extern int _end; /* Not in sys/unistd.h */
static char *cur_break = (char *) &_end;

void msp430_cpu_init(void) {
	dint();
	watchdog_init();
	init_ports();
	msp430_init_dco();
	eint();
	if ((uintptr_t) cur_break & 1) { /* Workaround for msp430-ld bug! */
		cur_break++;
	}
}
/*---------------------------------------------------------------------------*/
#define asmv(arg) __asm__ __volatile__(arg)

#define STACK_EXTRA 32

/*
 * Allocate memory from the heap. Check that we don't collide with the
 * stack right now (some other routine might later). A watchdog might
 * be used to check if cur_break and the stack pointer meet during
 * runtime.
 */
void *
sbrk(int incr) {
	char *stack_pointer;

	asmv("mov r1, %0" : "=r" (stack_pointer));
	stack_pointer -= STACK_EXTRA;
	if (incr > (stack_pointer - cur_break))
		return (void *) -1; /* ENOMEM */

	void *old_break = cur_break;
	cur_break += incr;
	/*
	 * If the stack was never here then [old_break .. cur_break] should
	 * be filled with zeros.
	 */
	return old_break;
}
/*---------------------------------------------------------------------------*/
/*
 * Mask all interrupts that can be masked.
 */
int splhigh_(void) {
	/* Clear the GIE (General Interrupt Enable) flag. */
	int sr;
	asmv("mov r2, %0" : "=r" (sr));
	asmv("bic %0, r2" : : "i" (GIE));
	return sr & GIE; /* Ignore other sr bits. */
}
/*---------------------------------------------------------------------------*/
/*
 * Restore previous interrupt mask.
 */
void splx_(int sr) {
	/* If GIE was set, restore it. */
	asmv("bis %0, r2" : : "r" (sr));
}
/*---------------------------------------------------------------------------*/
