# makefile configuration
NAMES		= ED_accel FD_coord

BUILD 		= build

CPU		= msp430x2274
INCLUDES 	= -Idrivers -Imac -Inet
CFLAGS          = -mmcu=${CPU} -O2 -Wall -g ${INCLUDES}
CC              = msp430-gcc

SRC   = drivers/clock.c \
	drivers/leds.c \
	drivers/usci.c \
	drivers/timer.c \
	drivers/cc2500.c \
	drivers/lis302.c \
	mac/csma.c

SRC_ED = net/netED.c ED_accel.c
SRC_FD = net/netFD.c FD_coord.c

OBJ = $(SRC:.c=.o)
OBJ_ED = $(SRC_ED:.c=.o)
OBJ_FD = $(SRC_FD:.c=.o)

.PHONY: all

#all should be the first target. it's built when make is run without args
all: $(patsubst %,%.elf,${NAMES})

#additional rules for files
ED_accel.elf: ${OBJ} ${OBJ_ED}
	${CC} -mmcu=${CPU} -o $@ ${OBJ} ${OBJ_ED}

FD_coord.elf: ${OBJ} ${OBJ_FD}
	${CC} -mmcu=${CPU} -o $@ ${OBJ} ${OBJ_FD}

clean:
	rm -f ${NAMES}.elf ${OBJ}

