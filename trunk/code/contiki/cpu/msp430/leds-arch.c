/*
 * Copyright (c) 2005, Swedish Institute of Computer Science.
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
 * This file is part of the Configurable Sensor Network Application
 * Architecture for sensor nodes running the Contiki operating system.
 *
 * $Id: leds-arch.c,v 1.1 2006/06/17 22:41:21 adamdunkels Exp $
 *
 * -----------------------------------------------------------------
 *
 * Author  : Adam Dunkels, Joakim Eriksson, Niclas Finne
 * Created : 2005-11-03
 * Updated : $Date: 2006/06/17 22:41:21 $
 *           $Revision: 1.1 $
 */

#include "contiki-conf.h"
#include "dev/leds.h"

#include <io.h>

#define LED_ARCH_RED 1
#define LED_ARCH_GREEN 2
#define LEDS_ARCH_ALL (LED_ARCH_RED | LED_ARCH_GREEN)

/*---------------------------------------------------------------------------*/
void
leds_arch_init(void)
{
	P1SEL &= ~LEDS_ARCH_ALL;
	P1REN &= ~LEDS_ARCH_ALL;
	P1DIR |= LED_ARCH_RED | LED_ARCH_GREEN;
  P1OUT = (P1OUT|~LEDS_ARCH_ALL);
}
/*---------------------------------------------------------------------------*/
unsigned char
leds_arch_get(void)
{
  return (P1OUT & LEDS_ARCH_ALL);
}
/*---------------------------------------------------------------------------*/
void
leds_arch_set(unsigned char leds)
{
  P1OUT = leds&LEDS_ARCH_ALL;
}
/*---------------------------------------------------------------------------*/
