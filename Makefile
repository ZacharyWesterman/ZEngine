NAME = zing
VERSION = 0
MAJOR   = 1
MINOR   = 0

ASSEMBLER = za

# SRCS = $(wildcard engine/*.cpp) $(wildcard engine/instructions/*.cpp)
SRCS = $(wildcard z/memory/*.cpp) $(wildcard z/compiler/*.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

ARCH = $(shell g++ -dumpmachine)

ifeq ($(findstring x86_64,$(ARCH)),x86_64)
CCTARGET = -m64
else
ifeq ($(findstring i686,$(ARCH)),i686)
CCTARGET = -m32
else
CCTARGET =
endif
endif

# opt defaults to -O3
ifndef OPT
OLEVEL = 3
endif

#if opt flag is true
ifneq (,$(findstring $(OPT),S size Size SIZE))
OLEVEL = s
endif

INCLUDE = -I"../libzed"
CCFLAGS = $(INCLUDE) -std=c++11 -W -Wall -Wextra -pedantic -fexceptions $(CCTARGET)
LFLAGS = -lzed

# if debug flag is false
ifeq (,$(findstring $(DEBUG),1 true True TRUE))
CCFLAGS += -O$(OLEVEL) -g0
LFLAGS += -s
else
CCFLAGS += -g3 -O$(OLEVEL) -DDEBUG
endif

CC = g++
LN = g++

default: $(NAME)

$(NAME): $(OBJS) main.o
	$(LN) $(LFLAGS) -o $@ $^

za: $(OBJS) za.o
	$(LN) $(LFLAGS) -o $@ $^

main.o: main.cpp
	$(CC) $(CCFLAGS) -o $@ -c $^

za.o: za.cpp
	$(CC) $(CCFLAGS) -o $@ -c $^

%.o: %.cpp %.hpp
	$(CC) $(CCFLAGS) -o $@ -c $<

clean: clean-examples
	rm -rf $(OBJS) $(NAME) *.o

examples: $(OBJS) examples/object examples/binary examples-bin

examples-bin: examples/binary/heap examples/binary/scanner

examples/binary:
	mkdir -p $@

examples/object:
	mkdir -p $@

examples/object/%.o: examples/source/%.cpp
	g++ -I. -I"../libzed" -std=c++11 -fPIC -o $@ -c $^

examples/binary/%: examples/object/%.o $(OBJS)
	g++ -lzed -o $@ $^

clean-examples:
	rm -rf examples/binary examples/object

.PHONY: clean default examples clean-examples examples-bin
