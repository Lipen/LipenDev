#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <ctime>

using namespace std;

int main()
{
	clock_t timeStart = clock();
	ifstream fi("LZW_toEncode.txt");

	if (fi.is_open()) {
		ofstream fo("LZW_Encoded.txt");

		if (fo.is_open()) {
			char c;
			string stack;
			string newstack;
			map<string, int> d;
			for (int i=0; i<256; ++i) {
				d[string(1, i)] = i;
			}
			int last = 256;

			while (fi.get(c)) {
				newstack = stack + c;
				if (d.find(newstack) == d.end()) {
					fo << d[stack] << " ";
					d[newstack] = last++;
					newstack = c;
				}
				stack = newstack;
			}
			fo << d[stack]; // BEWARE! :O
		} else {
			cout << "Unable to open output file :C" << endl;
		}
		fi.close();
	} else {
		cout << "Unable to open input file :c" << endl;
	}
	cout << "Done.\nTime: " << (double)(clock() - timeStart)/CLOCKS_PER_SEC*1000. << " ms" << endl;
	return 0;
}
