CC=gcc
DEPS = costs.h receiver.h
CFLAGS = -Wall -std=c99 -pthread -Wno-missing-braces

default: main

main: main.o costs.o receiver.o sender.o
	$(CC) $(CFLAGS) -o main main.o costs.o receiver.o sender.o

main.o: main.c receiver.h machine.h
	$(CC) $(CFLAGS) -c main.c

costs.o: costs.h
receiver.o: receiver.h
sender.o: sender.h

clean:
	-rm -f *.o
