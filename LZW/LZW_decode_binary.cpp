#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cmath>

#define pb push_back

using namespace std;

string convert2bits(unsigned char c) {
	string s;
	for (int i=0; i<8; ++i) {
		int p = pow(2, 7-i);
		s += (c/p)?"1":"0";
		c %= p;
	}
	return s;
}

int convert2int(string buf) {
	int size = buf.length();
	int x = 0;
	for (int i=0; i<size; ++i) {
		if (buf[size-i-1]=='1') {
			x += pow(2, i);
		}
	}
	return x;
}

int main()
{
	ifstream fi("LZW_Encoded.txt", ios::binary);

	if (fi) {
		fi.seekg(0, fi.end);
		int fileSize = fi.tellg() * 8; //bits
		fi.seekg(0, fi.beg);
		map<int, string> d;
		for (int i=0; i<256; ++i) {
			d[i] = string(1, i);
		}
		int last = 256;
		int len = 9; //(int)log2(last+1)+1

		char t;
		string data;
		data.reserve(fileSize);
		while (fi.get(t)) {
			data += convert2bits(t);
		}

		int k = convert2int(data.substr(0, len));
		string stack(1, k); //char group builder
		string flow = stack; //result
		string c; //current char/chargroup

		for (int i=len; i<fileSize; i+=len) {
			len = (int)log2(last+1)+1; //force(!) pre(!) ceil
			string z = data.substr(i, len);

			//Ignore trailing bits == last-byte-building-(zeros):
			if (z.length() < len) {
				cout << "Ignoring trailing " << z << endl;
				break;
			}
			k = convert2int(z);

			if (d.count(k)) {
				c = d[k];
			} else if (k==last) {
				c = stack + stack[0];
			} else {
				cout << "Bad encoded k = " << k << " at " << i << "!" <<endl;
			}
			flow += c;
			d[last++] = stack+c[0];
			stack = c;
		}

		ofstream fo("LZW_Decoded.txt", ios::binary);

		if (fo) {
			fo << flow;
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
