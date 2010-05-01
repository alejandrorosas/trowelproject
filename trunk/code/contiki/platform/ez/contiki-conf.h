/* -*- C -*- */
/* @(#)$Id: contiki-conf.h,v 1.56 2009/06/26 12:01:23 joxe Exp $ */

#ifndef CONTIKI_CONF_H
#define CONTIKI_CONF_H

#define PACKETBUF_CONF_ATTRS_INLINE 1

#define HAVE_STDINT_H
#define MSP430_MEMCPY_WORKAROUND 1
#include "msp430def.h"

#define CCIF
#define CLIF

#define CC_CONF_INLINE inline

#define PROCESS_CONF_NUMEVENTS 8
#define PROCESS_CONF_STATS 1
/*#define PROCESS_CONF_FASTPOLL    4*/

/* CPU target speed in Hz */
#define F_CPU 8000000L

/* Our clock resolution, this is the same as Unix HZ. */
#define CLOCK_CONF_SECOND 128

typedef unsigned short clock_time_t;

typedef unsigned long off_t;

#ifdef PROJECT_CONF_H
#include PROJECT_CONF_H
#endif /* PROJECT_CONF_H */




#endif /* CONTIKI_CONF_H */
