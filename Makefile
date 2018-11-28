EXECNAME = zing
LIBNAME = zed

SRCS = main.cpp $(wildcard z/compiler/*.cpp)
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

INCLUDE = -I"../libzed" -I"../zing"
CCFLAGS = $(INCLUDE) -std=c++11 -W -Wall -Wextra -pedantic -fexceptions $(CCTARGET)

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
else
CCFLAGS += -g3 -O$(OLEVEL) -DDEBUG
endif

LFLAGS = -l$(LIBNAME) -ldl
ifeq ($(OS),Windows_NT)
LFLAGS += -L.
EXECUTABLE = $(EXECNAME).exe
RMOBJS = $(subst /,\,$(OBJS))
RM = del
else
EXECUTABLE = $(EXECNAME)
RMOBJS = $(OBJS)
RM = rm -f
endif

CC = g++
LN = g++

default: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(LN) -o $@ $^ $(LFLAGS)

main.o: main.cpp
	$(CC) $(CCFLAGS) -o $@ -c $^

%.o: %.cpp %.h
	$(CC) $(CCFLAGS) -o $@ -c $<

clean: clear
	$(RM) $(RMOBJS)

clear:
	$(RM) *.so $(EXECUTABLE)

rebuild: clean default

.PHONY: default clean clear rebuild lang
