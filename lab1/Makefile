CC=g++
CFLAGS = -Wall

default:
	$(CC) $(CFLAGS) main.c -o main.o

gen:
	-head -c 12345678 < /dev/urandom > rand.bin

clean:
	-rm -f *.o
	-rm -f *.bin
