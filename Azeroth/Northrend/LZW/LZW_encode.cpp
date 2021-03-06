#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <ctime>

using namespace std;

int main()
{
	ifstream fi("LZW_toEncode.txt");

	if (fi) {
		ofstream fo("LZW_Encoded.txt");

		if (fo) {
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
			fo.close();
		} else {
			cout << "Unable to open output file :C" << endl;
		}
		fi.close();
	} else {
		cout << "Unable to open input file :c" << endl;
	}
	return 0;
}
