CXX = g++
CC = gcc

LIB = -lpthread -ldl
BIN = sqlite3.o test

all : $(BIN)

test : test.cpp sqlite3.o
	$(CXX) -o $@ $^ $(LIB)

sqlite3.o : sqlite3.c
	$(CC) -o $@ -c $^

clean :
	rm -f $(BIN)

.PHONY: all, clean
