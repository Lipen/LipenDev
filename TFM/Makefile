CC=g++
CV=-std=gnu++0x
# There goes list of all sources:
SOURCES=TFM.cpp
EXECUTABLE=TFM

OBJECTS=$(SOURCES:.cpp=.o)
# Compiler flags:
CFLAGS=$(CV) -c -Wall -O2
# Linker flags:
LDFLAGS=

.SUFFIXES: .cpp .o

all: compile run

compile: $(EXECUTABLE)

run:
	$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CV) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CV) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(EXECUTABLE).exe
