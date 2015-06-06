/* Copyright (c) 2015 Lipen */
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <cmath>
#include <cstdlib>

#define pb push_back

using namespace std;

string convertNumberToBits(int n, int size) {
	stringstream ss;
	for (int i = size-1; i >= 0; --i) {
		int p = pow(2, i);
		ss << ((n / p) ? "1" : "0");
		n %= p;
	}
	return ss.str();
}

unsigned char convertBitsToByte(string s) {
	unsigned char b = 0;
	for (int i = 0; i < 8; ++i) {
		if (s[i] == '1') b += (int)pow(2, 7-i);
	}
	return b;
}

string convertByteToBits(unsigned char c) {
	stringstream ss;
	for (int i = 0; i < 8; ++i) {
		int p = pow(2, 7-i);
		ss << ((c / p) ? "1" : "0");
		c %= p;
	}
	return ss.str();
}

int convertBitsToNumber(string buf) {
	int size = buf.length();
	int x = 0;
	for (int i = 0; i < size; ++i) {
		if (buf[size-i-1] == '1') {
			x += pow(2, i);
		}
	}
	return x;
}


bool compress(string nameIn, string nameOut) {
	ifstream fi(nameIn, ios::binary);

	if (fi) {
		stringstream flow;
		char c;
		string stack;
		string newstack;
		int last = 256;
		int len = 9;  // (int)log2(last+1)+1

		map<string, int> d;
		for (int i = 0; i < 256; ++i) {
			d[string(1, i)] = i;
		}

		while (fi.get(c)) {
			newstack = stack + c;

			if (d.find(newstack) == d.end()) {
				flow << convertNumberToBits(d[stack], len);
				d[newstack] = last++;
				newstack = c;
			}

			stack = newstack;
			len = (int)log2(last) + 1;
		}
		flow << convertNumberToBits(d[stack], len);

		fi.close();

		ofstream fo(nameOut, ios::binary);

		if (fo) {
			string flowString = flow.str();
			flowString.resize(ceil(flowString.length()/8.)*8, '0');

			for (int i = 0; i < (int)flowString.length(); i += 8) {
				fo.put(convertBitsToByte(flowString.substr(i, 8)));
			}

			fo.close();
		} else {
			cout << "Unable to open output file :C\n";
			return false;
		}
	} else {
		cout << "Unable to open input file :c\n";
		return false;
	}
	return true;
}


bool decompress(string nameIn, string nameOut) {
	ifstream fi(nameIn, ios::binary);

	if (fi) {
		fi.seekg(0, fi.end);
		int fileSize = fi.tellg() * 8;  // bits!
		fi.seekg(0, fi.beg);
		map<int, string> d;
		for (int i = 0; i < 256; ++i) {
			d[i] = string(1, i);
		}
		int last = 256;
		int len = 9;  // (int)log2(last+1)+1

		char t;
		string data;
		while (fi.get(t)) {
			data += convertByteToBits(t);
		}

		int k = convertBitsToNumber(data.substr(0, len));
		string stack(1, k);  // char group builder
		stringstream flow;   // result
		flow << stack;
		string c;            // current char/chargroup

		for (int i = len; i < fileSize; i += len) {
			len = (int)log2(last+1) + 1;  // force(!) pre(!) ceil
			string z = data.substr(i, len);
			// Ignore trailing bits == last-byte-building-(zeros):
			if ((int)z.length() < len) break;
			k = convertBitsToNumber(z);

			if (d.count(k)) {
				c = d[k];
			} else if (k == last) {
				c = stack + stack[0];
			} else {
				cout << "Bad encoded k = " << k << " at " << i << "!\n";
				return false;
			}
			flow << c;
			d[last++] = stack+c[0];
			stack = c;
		}

		fi.close();

		ofstream fo(nameOut, ios::binary);

		if (fo) {
			fo << (flow.str());  // parentheses!
			fo.close();
		} else {
			cout << "Unable to open output file :C\n";
			return false;
		}
	} else {
		cout << "Unable to open input file :c\n";
		return false;
	}
	return true;
}


bool compareStrings(string s, string t, bool ignoreCase=false) {
	if (s.size() != t.size())
		return false;

	for (int i = 0; i < (int)s.size(); ++i) {
		if (ignoreCase) {
			if (std::tolower(s[i]) != std::tolower(t[i]))
				return false;
		} else {
			if (s[i] != t[i])
				return false;
		}
	}

	return true;
}

template<size_t N>
bool match(string toMatch, const string (&matchWith)[N]) {
	for (int i = 0; i < (int)N; ++i)
		if (compareStrings(toMatch, matchWith[i], true))
			return true;
	return false;
}

template<typename T>
using matcherT = T;
using matcher = matcherT<string[]>;


int main(int argc, char * argv[]) {
	string nameIn("LZW_toEncode.txt");
	string nameOut("LZW_Encoded.txt");
	int amount = 1;
	bool isCompress = true;

	for (int i = 1; i < argc; ++i) {
		string arg(argv[i]);
		if (match(arg, matcher{"encode", "-encode", "compress", "-compress"})) {
			isCompress = true;
		} else if (match(arg, matcher{"decode", "decompress", "-decode", "-decompress"})) {
			isCompress = false;
		} else if (match(arg, matcher{"in", "-in", "input", "-input"})) {
			nameIn = string(argv[i+++1]);
		} else if (match(arg, matcher{"out", "-out", "output", "-output"})) {
			nameOut = string(argv[i+++1]);
		} else if (match(arg, matcher{"n", "-n", "number", "-number", "amount", "-amount"})) {
			amount = atoi(argv[i+++1]);
		} else {
			cout << "Weird argument \"" << arg << "\". Ignoring.\n";
		}
	}

	cout << (isCompress?"C":"Dec") << "ompression mode. At most " << amount << " times\n" << nameIn << "  -->  " << nameOut << '\n';

	int n = 0;
	if (isCompress) {
		if (compress(nameIn, nameOut))
			while (++n<amount && compress(nameOut, nameOut)) {}
	} else {
		if (decompress(nameIn, nameOut))
			while (++n<amount && decompress(nameOut, nameOut)) {}
	}

	cout << "Done. Total " << (isCompress ? "c" : "dec") << "ompressed " << n << " times.\n";

	return 0;
}
