CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: clean

symbolTable.o: symbolTable.h ../definitions.h

clean:
	rm -f $(wildcard *.o)
