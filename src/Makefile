CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

EMULATE = emulator/binFileLoader.o emulator/fetch.o emulator/decode.o

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o

emulate: emulate.o $(EMULATE)

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
	cd emulator; make clean;
	cd assembler; make clean;
