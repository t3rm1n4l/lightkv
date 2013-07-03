test: lightkv.o
	gcc -o test test.c lightkv.o

lightkv.o: lightkv.c
	gcc -Wall -c lightkv.c

clean:
	rm -f lightkv.o
