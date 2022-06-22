CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic

.PHONY: clean

symbolTable.o: symbolTable.h ../definitions.h

loadArmLines.o: loadArmLines.h symbolTable.h constants.h ../definitions.h

assemblerFunctions.o: assemblerFunctions.h symbolTable.h loadArmLines.h constants.h ../definitions.h ../emulator/instruction.h

clean:
	rm -f $(wildcard *.o)
