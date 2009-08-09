# makfile configuration
NAME            = pwm
CPU             = msp430x2274

DRIVERS		= ../../drivers
INCLUDES 	= -I${DRIVERS}
CFLAGS          = -mmcu=${CPU} -O2 -Wall -g ${INCLUDES}
CC              = msp430-gcc

SRC =	main.c \
		${DRIVERS}/clock.c \
		${DRIVERS}/pwm.c \
		${DRIVERS}/leds.c

OBJECTS = $(SRC:.c=.o)

.PHONY: all

#all should be the first target. it's built when make is run without args
all: ${NAME}.elf

#additional rules for files
${NAME}.elf: ${OBJECTS}
	${CC} -mmcu=${CPU} -o $@ ${OBJECTS}

clean:
	rm -f ${NAME}.elf ${OBJECTS}

