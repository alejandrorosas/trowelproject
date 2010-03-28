/* -*- C -*- */
/* @(#)$Id: contiki-conf.h,v 1.56 2009/06/26 12:01:23 joxe Exp $ */

#ifndef CONTIKI_CONF_H
#define CONTIKI_CONF_H

#define QUEUEBUF_CONF_NUM          1
#define QUEUEBUF_CONF_REF_NUM      1
#define UIP_CONF_ACTIVE_OPEN 0
#define UIP_CONF_TCP 0


#define PACKETBUF_CONF_ATTRS_INLINE 1


#define IEEE802154_CONF_PANID       0xABCD

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

#ifdef WITH_UIP6

#define RIMEADDR_CONF_SIZE              2

#define UIP_CONF_LL_802154              1
#define UIP_CONF_LLH_LEN                0

#ifndef UIP_CONF_ROUTER
#define UIP_CONF_ROUTER			0
#endif

#define UIP_CONF_IPV6                   1
#define UIP_CONF_IPV6_QUEUE_PKT         1
#define UIP_CONF_IPV6_CHECKS            1
#define UIP_CONF_IPV6_REASSEMBLY        0
#define UIP_CONF_NETIF_MAX_ADDRESSES    1
#define UIP_CONF_ND6_MAX_PREFIXES       1
#define UIP_CONF_ND6_MAX_NEIGHBORS      0
#define UIP_CONF_ND6_MAX_DEFROUTERS     0
#define UIP_CONF_IP_FORWARD             0
#define UIP_CONF_BUFFER_SIZE			120

#define SICSLOWPAN_CONF_COMPRESSION_IPV6        0
#define SICSLOWPAN_CONF_COMPRESSION_HC1         1
#define SICSLOWPAN_CONF_COMPRESSION_HC01        2
#define SICSLOWPAN_CONF_COMPRESSION             SICSLOWPAN_CONF_COMPRESSION_HC01
#ifndef SICSLOWPAN_CONF_FRAG
#define SICSLOWPAN_CONF_FRAG                    0
#define SICSLOWPAN_CONF_MAXAGE                  8
#endif /* SICSLOWPAN_CONF_FRAG */
#define SICSLOWPAN_CONF_CONVENTIONAL_MAC	1
#define SICSLOWPAN_CONF_MAX_ADDR_CONTEXTS       2
#endif /* WITH_UIP6 */

#define UIP_CONF_ICMP_DEST_UNREACH 1

#define UIP_CONF_DHCP_LIGHT
#define UIP_CONF_LLH_LEN         0
#define UIP_CONF_RECEIVE_WINDOW  48
#define UIP_CONF_TCP_MSS         48
#define UIP_CONF_MAX_CONNECTIONS 4
#define UIP_CONF_MAX_LISTENPORTS 8
#define UIP_CONF_UDP_CONNS       12
#define UIP_CONF_FWCACHE_SIZE    30
#define UIP_CONF_BROADCAST       1
#define UIP_ARCH_IPCHKSUM        1
#define UIP_CONF_UDP             1
#define UIP_CONF_UDP_CHECKSUMS   1
#define UIP_CONF_PINGADDRCONF    0
#define UIP_CONF_LOGGING         0

#define UIP_CONF_TCP_SPLIT       0

typedef unsigned short uip_stats_t;
typedef unsigned short clock_time_t;

typedef unsigned long off_t;

#ifdef PROJECT_CONF_H
#include PROJECT_CONF_H
#endif /* PROJECT_CONF_H */




#endif /* CONTIKI_CONF_H */
