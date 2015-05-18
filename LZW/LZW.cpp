#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cmath>
#include <stdlib.h>

#define pb push_back

using namespace std;

string convertNumberToBits(int n, int size) {
	string s;
	for (int i=size-1; i>=0; --i) {
		int p = pow(2, i);
		s += (n / p)?"1":"0";
		n %= p;
	}
	return s;
}

unsigned char convertBitsToByte(string s) {
	unsigned char b = 0;
	for (int i=0; i<8; ++i) {
		if (s[i]=='1') b += (int)pow(2, 7-i);
	}
	return b;
}

string convertByteToBits(unsigned char c) {
	string s;
	for (int i=0; i<8; ++i) {
		int p = pow(2, 7-i);
		s += (c/p)?"1":"0";
		c %= p;
	}
	return s;
}

int convertBitsToNumber(string buf) {
	int size = buf.length();
	int x = 0;
	for (int i=0; i<size; ++i) {
		if (buf[size-i-1]=='1') {
			x += pow(2, i);
		}
	}
	return x;
}

bool compress(string nameIn, string nameOut) {
	ifstream fi(nameIn, ios::binary);

	if (fi) {
		string flow;
		char c;
		string stack;
		string newstack;
		map<string, int> d;
		for (int i=0; i<256; ++i) {
			d[string(1, i)] = i;
		}
		int last = 256;
		int len = 9;

		while (fi.get(c)) {
			newstack = stack + c;
			if (d.find(newstack) == d.end()) {
				flow += convertNumberToBits(d[stack], len);
				d[newstack] = last++;
				newstack = c;
			}
			stack = newstack;
			len = (int)log2(last)+1;
		}
		flow += convertNumberToBits(d[stack], len);

		fi.close();

		ofstream fo(nameOut, ios::binary);

		if (fo) {
			flow.resize(ceil(flow.length()/8.)*8, '0');
			for (int i=0; i<(int)flow.length(); i+=8) {
				fo.put(convertBitsToByte(flow.substr(i, 8)));
			}
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

bool decompress(string nameIn, string nameOut) {
	ifstream fi(nameIn, ios::binary);

	if (fi) {
		fi.seekg(0, fi.end);
		int fileSize = fi.tellg() * 8; //bits!
		fi.seekg(0, fi.beg);
		map<int, string> d;
		for (int i=0; i<256; ++i) {
			d[i] = string(1, i);
		}
		int last = 256;
		int len = 9; //(int)log2(last+1)+1

		char t;
		string data;
		while (fi.get(t)) {
			data += convertByteToBits(t);
		}

		int k = convertBitsToNumber(data.substr(0, len));
		string stack(1, k); //char group builder
		string flow = stack; //result
		string c; //current char/chargroup

		for (int i=len; i<fileSize; i+=len) {
			len = (int)log2(last+1)+1; //force(!) pre(!) ceil
			string z = data.substr(i, len);
			//Ignore trailing bits == last-byte-building-(zeros):
			if ((int)z.length() < len) break;
			k = convertBitsToNumber(z);

			if (d.count(k)) {
				c = d[k];
			} else if (k==last) {
				c = stack + stack[0];
			} else {
				cout << "Bad encoded k = " << k << " at " << i << "!" <<endl;
				return false;
			}
			flow += c;
			d[last++] = stack+c[0];
			stack = c;
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
	int amount = 1;
	bool isCompress = true;

	for (int i=1; i<argc; ++i) {
		string arg(argv[i]);
		if (arg=="encode" || arg=="compress") {
			isCompress = true;
		} else if (arg=="decode" || arg=="decompress") {
			isCompress = false;
		} else if (arg=="-in") {
			nameIn = string(argv[i+++1]);
		} else if (arg=="-out") {
			nameOut = string(argv[i+++1]);
		} else if (arg=="-n") {
			amount = atoi(argv[i+++1]);
		} else {
			cout << "Weird argument \"" << arg << "\"" << endl;
		}
	}

	cout<<(isCompress?"C":"Dec")<<"ompression mode. At most "<<amount<<" times"<<endl<<nameIn<<"  -->  "<<nameOut<<endl;

	int n = 0;
	if (isCompress) {
		if (compress(nameIn, nameOut))
			while (++n<amount && compress(nameOut, nameOut));
	} else {
		if (decompress(nameIn, nameOut))
			while (++n<amount && decompress(nameOut, nameOut));
	}

	cout << "Done. Total " << (isCompress?"c":"dec") << "ompressed " << n << " times." << endl;

	return 0;
}
