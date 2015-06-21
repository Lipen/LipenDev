CC=g++
CV=-std=gnu++0x
# There goes list of all sources:
SOURCES=src/CardsDeck.cpp lib/Card.cpp lib/Deck.cpp lib/Player.cpp
EXECUTABLE=bin/CardsDeck

OBJECTS=$(SOURCES:.cpp=.o)
OPTLEVEL=-O2
# Compiler flags:
CFLAGS=$(CV) -c -Wall $(OPTLEVEL) -Ilib
# Linker flags:
LDFLAGS=

.SUFFIXES: .cpp .o

all: compile run

compile: $(EXECUTABLE)

run:
	@echo -n "Executing: "
	$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@echo -n "Linking: "
	$(CC) $(CV) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	@echo -n "Compiling: "
	$(CC) $(CV) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(EXECUTABLE).exe
