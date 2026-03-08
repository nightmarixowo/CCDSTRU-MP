CC = gcc
CFLAGS = -Wall -g

chain: main.o game.o
	$(CC) $(CFLAGS) -o chain main.o game.o

main.o: main.c game.h
	$(CC) $(CFLAGS) -c main.c

game.o: game.c game.h
	$(CC) $(CFLAGS) -c game.c

clean:
	rm -f *.o chain
