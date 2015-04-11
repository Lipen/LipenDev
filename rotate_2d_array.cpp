#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <ctime>

using namespace std;

void getWidthHeight(char * data, long int &width, long int &height) {
	int i = 0;
	for (; i<4; ++i) {
		width += data[i] << 8*(3-i);
		// cout << "dbg width(after): " <<  width << endl;
	}
	for (; i<8; ++i) {
		height += data[i] << 8*(7-i);
	}
}

bool rotate_clockwise(string nameIn, string nameOut) {
	ifstream fi(nameIn, ios::binary);

	if (fi) {
		fi.seekg(0, fi.end);
		int fileSize = fi.tellg();
		fi.seekg(0, fi.beg);
		char * data = new char[fileSize];
		fi.read(data, fileSize);

		// Weird debug stuff
		if (fi)
			std::cout << "successfully" << endl;
		else
			std::cout <<"error: only "<<fi.gcount()<<" could be read"<< endl;

		fi.close();

		long int width = 0, height = 0;
		getWidthHeight(data, width, height);

		cout << "WIDTH: " << width << "   HEIGHT: " << height << endl;
		if (width*height < fileSize-8) {
			cout << "Broken file" << endl;
			return false;
		}
		cout << "DATA:" << endl;
		for (int i=8; i<fileSize; ++i) {
			cout << (int)(unsigned char)data[i] << " ";
			if ((i-7)%width == 0) {
				cout << endl;
			}
		}
		cout << "END OF DATA" << endl;

		char * meta = new char[fileSize+1];
		for (int i=0; i<4; ++i) {
			meta[i] = data[i+4];
			meta[i+4] = data[i];
		}

		int p = 8;
		for (int i=0; i<width; ++i) {
			for (int j=height-1; j>=0; --j) {
				cout << "p = " << p << endl;
				meta[p++] = data[8+j*width+i];
			}
		}

		cout << "META" << endl;
		for (int i=8; i<fileSize; ++i) {
			cout << (int)(unsigned char)meta[i] << " ";
			if ((i-7)%height == 0) {
				cout << endl;
			}
		}
		cout << "END OF META" << endl;

		ofstream fo(nameOut, ios::binary);

		if (fo) {
			fo.write(meta, fileSize);
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
	string nameIn("file.bin");
	string nameOut("file_rotated.bin");
	// TODO: Implement various angles (90/-90/180)
	// int angle = 1;

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

	rotate_clockwise(nameIn, nameOut);

	cout << "---------------\nDone." << endl;
	cout << "\nTime: " << (double)(clock() - timeStart)/CLOCKS_PER_SEC << " seconds" << endl;
}
