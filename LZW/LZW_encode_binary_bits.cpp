#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <cmath>

#define pb push_back

using namespace std;

string convert2bits(int n, int size) {
	string s;
	for (int i=size-1; i>=0; --i) {
		int p = pow(2, i);
		s += (n / p)?"1":"0";
		n %= p;
	}
	return s;
}

unsigned char convert2byte(string s) {
	unsigned char b = 0;
	for (int i=0; i<8; ++i) {
		if (s[i]=='1') b += (int)pow(2, 7-i);
	}
	return b;
}

int main()
{
	ifstream fi("LZW_toEncode.txt");

	if (fi.is_open()) {
		string flow;
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
				flow += convert2bits(d[stack], (int)log2(last)+1);
				d[newstack] = last++;
				newstack = c;
			}
			stack = newstack;
		}
		flow += convert2bits(d[stack], (int)log2(last)+1);

		ofstream fo("LZW_Encoded.txt", ios::binary);

		if (fo.is_open()) {
			flow.resize(ceil(flow.length()/8.)*8, '0');
			for (int i=0; i<flow.length(); i+=8) {
				char* buf = new char[1];
				buf[0] = convert2byte(flow.substr(i, 8));
				fo.write(buf, 1);
				delete[] buf;
			}

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
