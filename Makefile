CC = g++
CFLAGS = -I"../zLibraries" -std=c++11 -g -Wall -fexceptions
LFLAGS =

SRCS = main.cpp $(wildcard z/*.cpp) $(wildcard z/engine/*.cpp) $(wildcard z/engine/instructions/*.cpp)

OBJS = $(patsubst %.cpp,%.o,$(SRCS))

.PHONY: all
all: zengine

zengine: $(OBJS)
	g++ $(LFLAGS) -o zengine $^

$(OBJS): %.o: %.cpp
	g++ $(CFLAGS) -o $@ -c $^

.PHONY: objdir
objdir: | obj

obj:
	mkdir -p obj

.PHONY: clean
clean:
	rm -f $(OBJS) zengine

.PHONY: clear
clear:
	rm -f $(OBJS)

.PHONY: rebuild
rebuild: clean all
