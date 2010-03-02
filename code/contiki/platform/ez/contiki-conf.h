/* -*- C -*- */
/* @(#)$Id: contiki-conf.h,v 1.56 2009/06/26 12:01:23 joxe Exp $ */

#ifndef CONTIKI_CONF_H
#define CONTIKI_CONF_H



/* Specifies the default MAC driver */
#define MAC_CONF_CSMA               1

#define XMAC_CONF_COMPOWER          1
#define CXMAC_CONF_COMPOWER         1

#if WITH_UIP6
#define MAC_CONF_DRIVER             cxmac_driver
#define MAC_CONF_CHANNEL_CHECK_RATE 8
#define RIME_CONF_NO_POLITE_ANNOUCEMENTS 0
#define CXMAC_CONF_ANNOUNCEMENTS    0
#define XMAC_CONF_ANNOUNCEMENTS     0
#endif

#define QUEUEBUF_CONF_NUM          16

#define PACKETBUF_CONF_ATTRS_INLINE 1

#ifndef RF_CHANNEL
#define RF_CHANNEL              26
#endif /* RF_CHANNEL */

#define IEEE802154_CONF_PANID       0xABCD

#define PROFILE_CONF_ON 0
#define ENERGEST_CONF_ON 0


#define HAVE_STDINT_H
#define MSP430_MEMCPY_WORKAROUND 1
#include "msp430def.h"

#define CCIF
#define CLIF

#define CC_CONF_INLINE inline

#define WITH_ASCII

#define PROCESS_CONF_NUMEVENTS 1
#define PROCESS_CONF_STATS 0

/* CLOCK CONFIGURATION */
#define CLOCK_CONF_SECOND 128

/* IPv6 CONFIG */
#ifdef WITH_UIP6

#define RIMEADDR_CONF_SIZE              8

#define UIP_CONF_LL_802154              1
#define UIP_CONF_LLH_LEN                0

#ifndef UIP_CONF_ROUTER
#define UIP_CONF_ROUTER			0
#endif


#define UIP_CONF_IPV6                   1
#define UIP_CONF_IPV6_QUEUE_PKT         1
#define UIP_CONF_IPV6_CHECKS            1
#define UIP_CONF_IPV6_REASSEMBLY        0
#define UIP_CONF_NETIF_MAX_ADDRESSES    3
#define UIP_CONF_ND6_MAX_PREFIXES       3
#define UIP_CONF_ND6_MAX_NEIGHBORS      4
#define UIP_CONF_ND6_MAX_DEFROUTERS     2
#define UIP_CONF_IP_FORWARD             0
#define UIP_CONF_BUFFER_SIZE		240

#define SICSLOWPAN_CONF_COMPRESSION_IPV6        0
#define SICSLOWPAN_CONF_COMPRESSION_HC1         1
#define SICSLOWPAN_CONF_COMPRESSION_HC01        2
#define SICSLOWPAN_CONF_COMPRESSION             SICSLOWPAN_CONF_COMPRESSION_HC01
#ifndef SICSLOWPAN_CONF_FRAG
#define SICSLOWPAN_CONF_FRAG                    1
#define SICSLOWPAN_CONF_MAXAGE                  8
#endif /* SICSLOWPAN_CONF_FRAG */
#define SICSLOWPAN_CONF_CONVENTIONAL_MAC	1
#define SICSLOWPAN_CONF_MAX_ADDR_CONTEXTS       2
#endif


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


/* PACKETBUF Config */
#define PACKETBUF_CONF_ATTRS_INLINE 1
#define PACKETBUF_CONF_SIZE 128
#define PACKETBUF_CONF_HDR_SIZE 6

/* EZ430 optimizations */
#define SERIAL_LINE_CONF_BUFSIZE 2
#endif /* CONTIKI_CONF_H */
