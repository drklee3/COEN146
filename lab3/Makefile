CC=gcc
DEPS = tfv2.h
CFLAGS = -Wall

default: udps.o udpc.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	-rm -f *.o
