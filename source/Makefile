SDK=../SaleaeAnalyzerSdk-1.1.9
DYLIB=libISO7816Analyzer.dylib

SRCS=iso7816Analyzer.cpp iso7816AnalyzerResults.cpp iso7816AnalyzerSettings.cpp iso7816SimulationDataGenerator.cpp
GDB=-g -ggdb

CFLAGS=-I"$(SDK)/include" -I. -O3 -w -c -fpic -Wall $(GDB) -m32
LDFLAGS=-L/Applications/Logic.app/Contents/MacOS  -lAnalyzer -dynamiclib  $(GDB) -m32
OBJECTS=$(SRCS:.cpp=.o)
CC=g++

all: clean $(OBJECTS) $(DYLIB) Makefile

clean:
	rm -f $(OBJECTS) $(DYLIB)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

$(DYLIB): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(DYLIB) $(OBJECTS)
