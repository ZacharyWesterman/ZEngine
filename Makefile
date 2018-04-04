CFLAGS= -I"../zLibraries" -std=c++11 -g -Wall -fexceptions
LFLAGS=

all: main.cpp
	g++ $(CFLAGS) -o zengine $(LFLAGS) main.cpp

clean:
	rm -f zengine
