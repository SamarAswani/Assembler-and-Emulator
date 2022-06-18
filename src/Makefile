CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

ASSEMBLE = assembler/symbolTable.o
EMULATE = emulator/binFileLoader.o emulator/fetch.o emulator/decode.o emulator/execute.o

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o $(ASSEMBLE)

emulate: emulate.o $(EMULATE)

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
	cd emulator; make clean;
	cd assembler; make clean;
