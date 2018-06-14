OUT = zengine
CC = g++
ODIR = obj
CFLAGS = -I"../zLibraries" -std=c++11 -g -Wall -fexceptions
LFLAGS =

_OBJS = main.o driver.o program.o instruction.o
OBJS = $(patsubst %,$(ODIR)/%,$(_OBJS))


all: objdir $(OBJS)
	g++ -o $(OUT) $(OBJS)

$(ODIR)/main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o $(ODIR)/main.o

$(ODIR)/driver.o: z/engine/driver.cpp
	$(CC) $(CFLAGS) -c z/engine/driver.cpp -o $(ODIR)/driver.o

$(ODIR)/program.o: z/engine/program.cpp
	$(CC) $(CFLAGS) -c z/engine/program.cpp -o $(ODIR)/program.o

$(ODIR)/instruction.o: z/engine/instruction.cpp
	$(CC) $(CFLAGS) -c z/engine/instruction.cpp -o $(ODIR)/instruction.o

objdir:
	mkdir -p $(ODIR)

clean:
	rm -f $(ODIR)/*.o $(OUT)

rebuild: clean all
