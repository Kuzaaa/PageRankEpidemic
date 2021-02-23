CC=gcc
CFLAGS=-Wall -lm
EXEC=main

all: $(EXEC)

main: main.o epidemic.o
	$(CC) -o main main.o epidemic.o $(CFLAGS)

main.o: main.c epidemic.h
	$(CC) -o main.o -c main.c $(CFLAGS)

epidemic.o: epidemic.c epidemic.h
	$(CC) -o epidemic.o -c epidemic.c $(CFLAGS)

clean:
	rm -rf *.o
