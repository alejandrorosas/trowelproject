# makfile configuration
NAME            = sense
CPU             = msp430x2274

DRIVERS		= ../../drivers
MAC			= ../../mac
SYS			= ../../sys

INCLUDES 	= -I${DRIVERS} -I${MAC} -I${SYS}
CFLAGS          = -mmcu=${CPU} -O2 -Wall -g ${INCLUDES}
CC              = msp430-gcc

SRC 	= main.c \
	${DRIVERS}/clock.c \
	${DRIVERS}/leds.c \
	${DRIVERS}/timer.c \
	${DRIVERS}/usci.c \
	${DRIVERS}/lis302.c \
	${DRIVERS}/hmc5843.c \
	${DRIVERS}/cc2500.c \
	${MAC}/csma.c \
	${SYS}/event.c

OBJECTS = $(SRC:.c=.o)

.PHONY: all

#all should be the first target. it's built when make is run without args
all: ${NAME}.elf

#additional rules for files
${NAME}.elf: ${OBJECTS}
	${CC} -mmcu=${CPU} -o $@ ${OBJECTS}

clean:
	rm -f ${NAME}.elf ${OBJECTS}

