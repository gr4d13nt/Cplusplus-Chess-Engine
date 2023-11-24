CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -std=c++11

all: chess_engine

chess_engine: moves.o representation.o
	$(CC) $(CFLAGS) -o chess_engine moves.o representation.o

moves.o: moves.cpp representation.h
	$(CC) $(CFLAGS) -c moves.cpp

representation.o: representation.cpp representation.h
	$(CC) $(CFLAGS) -c representation.cpp

clean:
	rm -rf *.o chess_engine
