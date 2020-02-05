CXX = g++
CC = gcc

LIB = -lpthread -ldl
BIN = sqlite3.o

all : $(BIN)

sqlite3.o : sqlite3.c
	$(CC) -o $@ -c $^

clean :
	rm -f $(BIN)

.PHONY: all, clean
