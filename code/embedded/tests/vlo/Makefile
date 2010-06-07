# makfile configuration
NAME            = vlo
CPU             = msp430x2274

DRIVERS		= ../../drivers
LIB			= ../../lib
INCLUDES 	= -I${DRIVERS} -I ${LIB}
CFLAGS          = -mmcu=${CPU} -O2 -Wall -g ${INCLUDES}
CC              = msp430-gcc

SRC 	= main.c \
	${DRIVERS}/clock.c \
	${DRIVERS}/leds.c \
	${DRIVERS}/usci.c \
	${DRIVERS}/cc2500.c \
	${DRIVERS}/timer.c \
	${LIB}/vlo_sync.c

OBJECTS = $(SRC:.c=.o)

.PHONY: all

#all should be the first target. it's built when make is run without args
all: ${NAME}.elf

#additional rules for files
${NAME}.elf: ${OBJECTS}
	${CC} -mmcu=${CPU} -o $@ ${OBJECTS}

clean:
	rm -f ${NAME}.elf ${OBJECTS}

