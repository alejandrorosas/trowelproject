# makfile configuration
NAME            = state_test
CPU             = msp430x2274

DRIVERS		= ../../drivers
SYS			= ../../sys
INCLUDES 	= -I${DRIVERS} -I${SYS}

CFLAGS          = -mmcu=${CPU} -O2 -Wall -g ${INCLUDES}
CC              = msp430-gcc

SRC 	= main.c first_chain.c \
		${DRIVERS}/clock.c \
		${DRIVERS}/leds.c \
		${DRIVERS}/timer.c \
		${SYS}/event.c \
		${SYS}/state.c

OBJECTS = $(SRC:.c=.o)

.PHONY: all

#all should be the first target. it's built when make is run without args
all: ${NAME}.elf

#additional rules for files
${NAME}.elf: ${OBJECTS}
	${CC} -mmcu=${CPU} -o $@ ${OBJECTS}

clean:
	rm -f ${NAME}.elf ${OBJECTS}

