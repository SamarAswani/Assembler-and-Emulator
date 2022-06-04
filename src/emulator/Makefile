CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: clean

binFileLoader.o: binFileLoader.h ../definitions.h

fetch.o: binFileLoader.h fetch.h ../definitions.h

decode.o: decode.h instruction.h ../definitions.h

clean:
	rm -f $(wildcard *.o)
