CC = g++
CV = -std=c++14
OPTLVL = -O3
INCLUDE = -IC:/Users/Lipen/Desktop/SFML-2.3.2/install/include
LIBPATH = -LC:/Users/Lipen/Desktop/SFML-2.3.2/install/lib

# Compiler flags:
CFLAGS = $(CV) -c -Wall -Wextra $(OPTLVL) $(INCLUDE)
# Linker flags:
LDFLAGS = $(LIBPATH) -lsfml-network -lsfml-system

all: compile

compile: master slave

master: master.o
	@echo -n "Linking: "
	$(CC) master.o -o $@ $(LDFLAGS)

slave: slave.o
	@echo -n "Linking: "
	$(CC) slave.o -o $@ $(LDFLAGS)

.SUFFIXES: .cpp .o

.cpp.o:
	@echo -n "Compiling: "
	$(CC) $< -o $@ $(CFLAGS)

clean:
	@echo -n "Cleaning up: "
	rm -f master.o slave.o master slave master.exe slave.exe
