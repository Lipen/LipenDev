CC = g++
CV = -std=gnu++0x
OPTLVL = -O2
# DEBUG = -g
# INCLUDE = -Ilib
RUNDEPS = input.txt output.txt

EXECUTABLE = TFM
# Sources to compile:
SOURCES = TFM.cpp
OBJS = $(SOURCES:.cpp=.o)

# Compiler flags:
CFLAGS = $(CV) -c -Wall $(OPTLVL) $(DEBUG) $(INCLUDE)
# Linker flags:
LDFLAGS =

all: compile run

compile: $(EXECUTABLE)

run: $(RUNDEPS)
	@echo -n "Executing: "
	$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	@echo -n "Linking: "
	$(CC) $(LDFLAGS) $(OBJS) -o $@

.SUFFIXES: .cpp .o

.cpp.o:
	@echo -n "Compiling: "
	$(CC) $(CFLAGS) $< -o $@

clean:
	@echo -n "Cleaning up: "
	rm -f $(OBJS) $(EXECUTABLE) $(EXECUTABLE).exe

%.txt:
	@echo -n "Creating non-existing empty txt file: "
	touch $@
