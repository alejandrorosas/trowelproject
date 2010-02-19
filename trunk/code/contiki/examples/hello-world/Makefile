CONTIKI_PROJECT = hello-world
all: $(CONTIKI_PROJECT)

CONTIKI = ../..
include $(CONTIKI)/Makefile.include

prog:
	msp430-objcopy -O ihex $(CONTIKI_PROJECT).$(TARGET) $(CONTIKI_PROJECT).hex
	sudo msp430-ez "prog $(CONTIKI_PROJECT).hex"
