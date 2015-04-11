#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cmath>
#include <stdlib.h>

#define pb push_back

using namespace std;

bool rotate_clockwise(string nameIn, string nameOut) {
	ifstream fi(nameIn, ios::binary);

	if (fi) {
		fi.seekg(0, fi.end);
		int fileSize = fi.tellg();
		fi.seekg(0, fi.beg);
		while (fi.get(t)) {
			data += convertByteToBits(t);
		}

		fi.close();

		ofstream fo(nameOut, ios::binary);

		if (fo) {
			fo << flow;
			fo.close();
		} else {
			cout << "Unable to open output file :C" << endl;
			return false;
		}
	} else {
		cout << "Unable to open input file :c" << endl;
		return false;
	}
	return true;
}

int main(int argc, char * argv[]) {

	string nameIn("LZW_toEncode.txt");
	string nameOut("LZW_Encoded.txt");
	// TODO: Implement various angles (90/-90/180)
	// int angle = 1;
	bool isCompress = true;

	for (int i=1; i<argc; ++i) {
		string arg(argv[i]);
		if (arg=="-in") {
			nameIn = string(argv[i+++1]);
		} else if (arg=="-out") {
			nameOut = string(argv[i+++1]);
		// } else if (arg=="-angle") {
		// 	a = atoi(argv[i+++1]);
		// 	if (a == 90 || a == -90 || a == 180 || a == -180) {
		// 		angle = a;
		// 	} else {
		// 		cout << "Weird angle \"" << a << "\"" << endl;
		// 	}
		} else {
			cout << "Weird argument \"" << arg << "\"" << endl;
		}
	}

	clock_t timeStart = clock();

	//

	cout << "Time: " << (double)(clock() - timeStart)/CLOCKS_PER_SEC << " seconds" << endl;
}
