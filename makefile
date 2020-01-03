CC=g++ -O0 -g --std=c++17 -Wall
GTK=`pkg-config gtk+-3.0 --cflags`
GTKMM=`pkg-config gtkmm-3.0 --cflags --libs`
WEBKITGTK=`pkg-config webkit2gtk-4.0 --cflags --libs`

SRC=main.cpp websdk.cpp backend.cpp
OBJ=$(SRC:.cpp=.o)
MUJS_DIR=mujs
MUJS=-Lmujs/build/release -lmujs

EXE=main

all: mujs $(EXE)

$(EXE) : $(OBJ)
	$(CC) $(OBJ) -o $@ $(WEBKITGTK) $(GTKMM) $(GTK) $(MUJS) -lstdc++fs

.cpp.o:
	$(CC) -c $< -o $@ $(WEBKITGTK) $(GTKMM) $(GTK)

.PHONY: mujs

mujs:
	$(MAKE) -C $(MUJS_DIR) static

clean:
	rm *.o $(EXE)
	$(MAKE) -C $(MUJS_DIR) clean
