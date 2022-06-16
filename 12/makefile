.PHONY: all clean

CC=g++
CXXFLAGS=-fdiagnostics-color=always
SRCDIR=./src
BIN=./bin
BUILD=build
PROG=day12.exe

all: $(PROG)

src = $(wildcard $(addsuffix /*.cpp,$(SRCDIR)))

obj = $(src:.c=.o)

$(PROG): $(OBJECTS)
	$(CC) $(CXXFLAGS) -o $(BIN)/$(PROG) $(obj)

clean: 
	del $(BIN)/$(PROG)
