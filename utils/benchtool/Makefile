LIGHTDB_SRC=../../
SQLITE_SRC=third_party/sqlite3
FLAGS = -Wall

build: lightkv.o driver.o
	g++ $(FLAGS) -o driver lightkv.o driver.o sqlite3.o sqlite-objects.o

driver.o: lightkv.o sqlite-objects.o sqlite3.o
	g++ $(FLAGS) -Wall -c driver.cc -I$(LIGHTDB_SRC)

lightkv.o: $(LIGHTDB_SRC)/lightkv.c $(LIGHTDB_SRC)/lightkv.h
	gcc $(FLAGS) -c $<

sqlite3.o: $(SQLITE_SRC)/sqlite3.c $(SQLITE_SRC)/sqlite3.h
	gcc $(FLAGS) -c $<

sqlite-objects.o: helpers/sqlite-objects.cc helpers/sqlite-objects.hh
	g++ $(FLAGS) -c $<

clean:
	rm -f *.o driver
