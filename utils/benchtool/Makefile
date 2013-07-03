build: lightkv.o driver.o
	g++ -o driver lightkv.o driver.o

driver.o:
	g++ -c driver.cc -I"../../"

lightkv.o:
	gcc -c ../../lightkv.c

clean:
	rm -f *.o driver
