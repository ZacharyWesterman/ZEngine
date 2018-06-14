CC = g++
CFLAGS = -I"../zLibraries" -std=c++11 -g -Wall -fexceptions
LFLAGS =

SRCS = main.cpp $(wildcard z/*.cpp) $(wildcard z/engine/*.cpp) $(wildcard z/engine/instructions/*.cpp)

OUT_ = $(subst /,.,$(SRCS))
OUTS = $(patsubst %.cpp,obj/%.o,$(OUT_))

all: objdir zengine

zengine: $(OUTS)
	g++ $(LFLAGS) -o zengine $<

$(OUTS): $(SRCS)
	g++ $(CFLAGS) -c $< -o $@

.PHONY: objdir
objdir:
	mkdir -p obj

.PHONY: clean
clean:
	rm -f obj/*.o zengine

.PHONY: rebuild
rebuild: clean all
