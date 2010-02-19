ifndef CONTIKI
CONTIKI=../../..
endif

DEFINES=WITH_UIP6=1,WITH_SLIP=1,PROJECT_CONF_H=\"bridge-conf.h\"
UIP_CONF_IPV6=1

ifndef TARGET
TARGET=ez
endif

PROJECTDIRS = dev
PROJECT_SOURCEFILES = fakeuip.c sicslow_ethernet.c slip_uart.c slip.c

all:	uip6-bridge-tap

include $(CONTIKI)/Makefile.include
