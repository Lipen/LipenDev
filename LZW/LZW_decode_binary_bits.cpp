#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cstring>
#include <cmath>

#define pb push_back

using namespace std;

string convert2bits(unsigned char c) {
	string s;
	cout << "convert2bits("<<c;
	for (int i=0; i<8; ++i) {
		int p = pow(2, 7-i);
		s += (c / p)?"1":"0";
		c %= p;
	}
	cout << "): Returning " << s << endl;
	return s;
}

long long convert2int(string buf) {
	long long size = buf.length();
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
	ifstream fi("LZW_Encoded.txt", ios::binary|ios::in);

	if (fi) {
		vector<string> flow;
		fi.seekg(0, fi.end);
		int fileSize = fi.tellg();
		fi.seekg(0, fi.beg);
		cout << "fileSize = " << fileSize << endl;
		long long k;
		string c;
		string stack;
		string newstack;
		map<string, int> d;
		for (int i=0; i<256; ++i) {
			d[string(1, i)] = i;
		}
		int last = 256;

		char t;
		string data;
		data.reserve(8*fileSize);
		while (fi.get(t)) {
			data += convert2bits(t);
		}
		//cout << data << endl;

		for (int i=0; i<data.length(); i+=(int)log2(last+1)+1) {
			//(int)log2(last+1)+1
			// cout << "substr = " << buf.substr(i, (int)log2(last+1)+1) << endl;
			// k = convert(string(buf+i, (int)log2(last+1)+1));
			k = convert2int(data.substr(i, (int)log2(last+1)+1));
			// cout << "substr = "<<data.substr(i, (int)log2(last+1)+1)<<"   k = " << k << " last="<<last<< endl;
			map<string, int>::iterator it = d.begin();
			for (; it != d.end(); ++it) {
				if (it->second == k) {
					c = it->first;
					break;
				}
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

		if (fo) {
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
