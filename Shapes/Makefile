CC = g++
CV = -std=gnu++1y
OPTLVL = -O2
# DEBUG = -g
# INCLUDE = -Ilib
# RUNDEPS = input.txt output.txt

EXECUTABLE = Shapes_bin
# Sources to compile:
SOURCES = main.cpp Shape.cpp Rect.cpp RightTriangle.cpp Circle.cpp
OBJS = $(SOURCES:.cpp=.o)

# Compiler flags:
CFLAGS = $(CV) -c -Wall $(OPTLVL) $(DEBUG) $(INCLUDE)
# Linker flags:
LFLAGS = -Wall $(DEBUG)

all: compile run

compile: $(EXECUTABLE)

run: $(RUNDEPS)
	@echo -n "Executing: "
	$(EXECUTABLE)

run_clean: clean compile run

$(EXECUTABLE): $(OBJS)
	@echo -n "Linking: "
	$(CC) $(LFLAGS) $(OBJS) -o $@

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
