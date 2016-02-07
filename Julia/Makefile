all: cl run

cl:
	g++ -std=gnu++11 -Wall -Wextra -O3 -o julia julia.cpp -lOpenCL

gray:
	g++ -std=c++11 -Wall -Wextra -O3 -o julia julia_grayscale.cpp -lOpenCL

c:
	gcc -Wall -Wextra -O3 -o julia julia.c -lOpenCL

run:
	julia
