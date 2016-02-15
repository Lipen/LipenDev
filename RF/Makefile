CC = g++
CV = -std=gnu++11
OPTLVL = -O2

EXECUTABLE = main
SOURCES = main.cpp Robot.cpp Ball.cpp LTexture.cpp
INCLUDE =
LIBPATH =

CFLAGS = $(CV) -Wall -Wextra $(OPTLVL) $(INCLUDE)
LDFLAGS = $(LIBPATH) -lmingw32 -lSDL2main -lSDL2_image -lSDL2 -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid

all: compile run

compile:
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(SOURCES) $(LDFLAGS)

run:
	./$(EXECUTABLE)
