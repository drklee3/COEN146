CC=gcc
DEPS = tfv2.h
CFLAGS = -Wall

default: udps.o udpc.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	-rm -f *.o
