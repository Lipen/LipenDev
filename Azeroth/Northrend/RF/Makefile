CC = g++
CV = -std=gnu++11
OPTLVL = -O2

EXECUTABLE = main
SOURCES = main.cpp Robot.cpp Ball.cpp LTexture.cpp
INCLUDE =
LIBPATH =

OBJS = $(SOURCES:.cpp=.o)

CFLAGS = $(CV) -c -Wall -Wextra $(OPTLVL) $(INCLUDE)
LDFLAGS = $(LIBPATH) -lmingw32 -lSDL2main -lSDL2_image -lSDL2 -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid

all: compile run

compile: $(EXECUTABLE)

run:
	@echo -n "Executing: "
	./$(EXECUTABLE)

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
