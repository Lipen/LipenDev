CC = g++
CV = -std=c++14
OPTLVL = -O3
# DEBUG = -DDEBUG
INCLUDE = -IC:/Users/Lipen/Desktop/SFML-2.3.2/install/include
LIBPATH = -LC:/Users/Lipen/Desktop/SFML-2.3.2/install/lib
# RUNDEPS = input.txt output.txt

EXECUTABLE = main
# Sources to compile:
SOURCES = main.cpp
OBJS = $(SOURCES:.cpp=.o)

# Compiler flags:
CFLAGS = $(CV) -c -Wall -Wextra $(OPTLVL) $(DEBUG) $(INCLUDE)
# Linker flags:
LDFLAGS = $(LIBPATH) -lsfml-graphics -lsfml-window -lsfml-system

all: compile run

compile: $(EXECUTABLE)

run: $(RUNDEPS)
	@echo -n "Executing: "
	$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	@echo -n "Linking: "
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

.SUFFIXES: .cpp .o

.cpp.o:
	@echo -n "Compiling: "
	$(CC) $< -o $@ $(CFLAGS)

clean:
	@echo -n "Cleaning up: "
	rm -f $(OBJS) $(EXECUTABLE) $(EXECUTABLE).exe

%.txt:
	@echo -n "Creating non-existing empty txt file: "
	touch $@
