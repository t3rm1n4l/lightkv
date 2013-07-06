CFLAGS= -g -Wall -D_DEBUG

test: lightkv.o
	gcc $(CFLAGS) -o test test.c lightkv.o

lightkv.o: lightkv.c
	gcc $(CFLAGS) -c lightkv.c

clean:
	rm -f lightkv.o
