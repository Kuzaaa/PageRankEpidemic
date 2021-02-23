CC=gcc
CFLAGS=-Wall -lm
EXEC=main

all: $(EXEC)

main: main.o pageRank.o
	$(CC) -o main main.o pageRank.o $(CFLAGS)

main.o: main.c pageRank.h
	$(CC) -o main.o -c main.c $(CFLAGS)

pageRank.o: pageRank.c pageRank.h
	$(CC) -o pageRank.o -c pageRank.c $(CFLAGS)

clean:
	rm -rf *.o