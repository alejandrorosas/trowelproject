# makfile configuration
NAME            = radioprint
CPU             = msp430x2274

DRIVERS		= ../../drivers
MAC		= ../../mac
INCLUDES 	= -I${DRIVERS} -I${MAC}
CFLAGS          = -mmcu=${CPU} -O2 -Wall -g ${INCLUDES}
CC              = msp430-gcc

SRC 	= main.c \
	${DRIVERS}/clock.c \
	${DRIVERS}/leds.c \
	${DRIVERS}/usci.c \
	${DRIVERS}/timer.c \
	${DRIVERS}/cc2500.c \
	${MAC}/csma.c

OBJECTS = $(SRC:.c=.o)

.PHONY: all

#all should be the first target. it's built when make is run without args
all: ${NAME}.elf

#additional rules for files
${NAME}.elf: ${OBJECTS}
	${CC} -mmcu=${CPU} -o $@ ${OBJECTS}

clean:
	rm -f ${NAME}.elf ${OBJECTS}

