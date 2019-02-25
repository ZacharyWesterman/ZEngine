NAME = zing
VERSION = 0
MAJOR   = 1
MINOR   = 0

SRCS = main.cpp $(wildcard engine/*.cpp) $(wildcard engine/instructions/*.cpp)
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

# if debug flag is false
ifeq (,$(findstring $(DEBUG),1 true True TRUE))
CCFLAGS += -O$(OLEVEL) -g0
LFLAGS += -s
else
CCFLAGS += -g3 -O$(OLEVEL) -DDEBUG
endif

INCLUDE = -I"../libzed"
CCFLAGS = $(INCLUDE) -std=c++11 -W -Wall -Wextra -pedantic -fexceptions $(CCTARGET)

LFLAGS = -lzed

CC = g++
LN = g++

default: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(LFLAGS) -o $(NAME) $^

main.o: main.cpp
	$(CC) $(CCFLAGS) -o $@ -c $^

%.o: %.cpp %.hpp
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -f $(OBJS) $(NAME)

.PHONY: clean default
