CC=gcc
CFLAGS=-Wall
EXEC=prog

all: $(EXEC)

prog: graph.o pageRank.o exploitResult.o main.o
	$(CC) -o prog graph.o pageRank.o exploitResult.o main.o

graph.o: graph.c graph.h pageRank.h
	$(CC) -o graph.o -c graph.c $(CFLAGS)

pageRank.o: pageRank.c graph.h pageRank.h
	$(CC) -o pageRank.o -c pageRank.c $(CFLAGS)
	
exploitResult.o: exploitResult.c graph.h exploitResult.h
	$(CC) -o exploitResult.o -c exploitResult.c $(CFLAGS)

main.o: main.c graph.h pageRank.h exploitResult.h
	$(CC) -o main.o -c main.c $(CFLAGS)

clean:
	rm -rf *.o

cleanAll: clean
	rm -rf $(EXEC)
