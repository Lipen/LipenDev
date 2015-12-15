#include <iostream>
#include <windows.h>

using std::cout;
using std::cin;
using std::endl;


int main(int argc, char* argv[]) {
	cout << "Sum.exe wokrs..." << endl;

	long int sum = 0;

	for (int i = 1; i < argc; ++i) {
		sum += atoi(argv[i]);
	}

	cout << "Sum: " << sum << endl;

	return 0;
}
