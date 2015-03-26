#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

#define pb push_back

using namespace std;

int main()
{
	ifstream fi("LZW_toEncode.txt");

	if (fi.is_open()) {
		vector<string> flow;
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
				flow.pb(stack);
				d[newstack] = last++;
				newstack = c;
			}
			stack = newstack;
		}
		flow.pb(stack); // BEWARE! :O

		ofstream fo("LZW_Encoded.txt");
		if (fo.is_open()) {
			for (auto item : flow) {
				fo << d[item] << endl;
			}
		} else {
			cout << "Unable to open output file :C" << endl;
		}
		fi.close();
	} else {
		cout << "Unable to open input file :c" << endl;
	}
	return 0;
}
