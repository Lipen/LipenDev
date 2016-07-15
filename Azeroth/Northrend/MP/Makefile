CC = g++
CV = -std=gnu++11
OPTLVL = -O2

EXE_MAIN = main
# Sources to compile:
SOURCES = main.cpp Server.cpp Sum.cpp Mult.cpp
EXES = $(SOURCES:.cpp=.exe)

# Compiler flags:
CFLAGS = $(CV) -Wall -Wextra $(OPTLVL)


all: compile run

compile: $(EXES)

run:
	@echo -n "Executing: "
	$(EXE_MAIN)

.SUFFIXES: .cpp .exe
.cpp.exe:
	@echo -n "Compiling: "
	$(CC) $(CFLAGS) $< -o $@

clean:
	@echo -n "Cleaning up: "
	rm -f $(EXES) $(EXE_MAIN).exe
