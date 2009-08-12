# makefile configuration
NAMES		= ED_accel FD_coord

BUILD 		= build

CPU		= msp430x2274
INCLUDES 	= -Idrivers -Imac
CFLAGS          = -mmcu=${CPU} -O2 -Wall -g ${INCLUDES}
CC              = msp430-gcc

SRC	= drivers/clock.c \
	drivers/leds.c \
	drivers/usci.c \
	drivers/timer.c \
	drivers/cc2500.c \
	drivers/lis302.c \
	mac/csma.c

OBJ = $(SRC:.c=.o)


.PHONY: all

#all should be the first target. it's built when make is run without args
all: $(patsubst %,%.elf,${NAMES})

#additional rules for files
ED_accel.elf: ${OBJ} ED_accel.o
	${CC} -mmcu=${CPU} -o $@ ${OBJ} ED_accel.o

FD_coord.elf: ${OBJ} FD_coord.o
	${CC} -mmcu=${CPU} -o $@ ${OBJ} FD_coord.o

clean:
	rm -f ${NAMES}.elf ${OBJ}

