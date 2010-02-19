/* -*- C -*- */
/* @(#)$Id: contiki-conf.h,v 1.56 2009/06/26 12:01:23 joxe Exp $ */

#ifndef CONTIKI_CONF_H
#define CONTIKI_CONF_H

#define HAVE_STDINT_H
#define MSP430_MEMCPY_WORKAROUND 1
#include "msp430def.h"

#define CCIF
#define CLIF
#define CC_CONF_INLINE inline
typedef unsigned short clock_time_t;

/* CLOCK CONFIGURATION */
#define CLOCK_CONF_SECOND 128

/* PACKETBUF Config */
#define PACKETBUF_CONF_ATTRS_INLINE 1
#define PACKETBUF_CONF_SIZE 128
#define PACKETBUF_CONF_HDR_SIZE 6

/* UIP6 Config */
#define UIP_CONF_IPV6	1
#define UIP_CONF_TCP 0
#define UIP_CONF_UDP 1
#define UIP_CONF_LL_802154	1
#define UIP_CONF_LLH_LEN	0
#define UIP_CONF_NETIF_MAX_ADDRESSES 1
#define UIP_CONF_ND6_MAX_NEIGHBORS 1
#define UIP_CONF_ND6_MAX_DEFROUTERS 1
#define UIP_CONF_ND6_MAX_PREFIXES 1
#define UIP_CONF_BUFFER_SIZE 128
#define UIP_CONF_UDP_CONNS 1
#define UIP_CONF_MAX_CONNECTIONS 1
#define UIP_CONF_MAX_LISTENPORTS 1

/* 6loWPAN Config */
#define SICSLOWPAN_CONF_COMPRESSION_IPV6        0
#define SICSLOWPAN_CONF_COMPRESSION_HC1         1
#define SICSLOWPAN_CONF_COMPRESSION_HC01        2
#define SICSLOWPAN_CONF_COMPRESSION             SICSLOWPAN_CONF_COMPRESSION_HC01
#define SICSLOWPAN_CONF_MAX_ADDR_CONTEXTS 1

/* EZ430 optimizations */
#define PROCESS_CONF_NUMEVENTS 1
#define SERIAL_LINE_CONF_BUFSIZE 2
#endif /* CONTIKI_CONF_H */
