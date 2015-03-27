#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include <cmath>

#define pb push_back

using namespace std;

long long convert(char * buf) {
	long long size = strlen(buf);
	long long x = 0;
	for (int i=0; i<size; ++i) {
		if (buf[size-i-1]=='1') {
			x += pow(2, i);
		}
	}
	return x;
}

int main()
{
	ifstream fi("LZW_Encoded.txt");

	if (fi.is_open()) {
		vector<string> flow;
		char * buf = new char[1000];
		long long k;
		string c;
		string stack;
		string newstack;
		map<string, int> d;
		for (int i=0; i<256; ++i) {
			d[string(1, i)] = i;
		}
		int last = 256;

		while (fi.read(buf, (long long)log2(last+1)+1)) {
			k = convert(buf);
			map<string, int>::iterator it = d.begin();
			for (; it != d.end(); ++it )
				if (it->second == k) {
					c = it->first;
					break;
				}
			stack += c;
			if (d.find(stack) != d.end()) {
				flow.pb(stack);
			} else {
				flow.pb(c);
				d[stack] = last++;
				stack = c;
			}
		}

		ofstream fo("LZW_Decoded.txt");
		if (fo.is_open()) {
			//C++11:
			// for (auto item : flow) {
			// 	fo << item;
			// }
			//C++98:
			vector<string>::iterator item = flow.begin();
			for (; item != flow.end(); ++item) {
				fo << *item;
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
