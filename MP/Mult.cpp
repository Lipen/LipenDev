#include <iostream>
#include <windows.h>

using std::cout;
using std::cin;
using std::endl;


int main(int argc, char* argv[]) {
	cout << "Mult.exe wokrs..." << endl;

	long int mult = 1;

	for (int i = 1; i < argc; ++i) {
		mult *= atoi(argv[i]);
	}

	cout << "Mult: " << mult << endl;

	return 0;
}
