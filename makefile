CC=g++ -O0 -g --std=c++17 -Wall
GTK=`pkg-config gtk+-3.0 --cflags`
GTKMM=`pkg-config gtkmm-3.0 --cflags --libs`
WEBKITGTK=`pkg-config webkit2gtk-4.0 --cflags --libs`

SRC=main.cpp websdk.cpp
OBJ=$(SRC:.cpp=.o)

EXE=main

all: $(EXE)

$(EXE) : $(OBJ)
	$(CC) $(OBJ) -o $@ $(WEBKITGTK) $(GTKMM) $(GTK) -lstdc++fs

.cpp.o:
	$(CC) -c $< -o $@ $(WEBKITGTK) $(GTKMM) $(GTK)

clean:
	rm *.o $(EXE)