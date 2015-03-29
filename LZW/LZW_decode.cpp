#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

#define pb push_back

using namespace std;

int main()
{
	ifstream fi("LZW_Encoded.txt");

	if (fi) {
		map<int, string> d;
		for (int i=0; i<256; ++i) {
			d[i] = string(1, i);
		}
		int last = 256;
		int k;
		fi >> k;
		string stack(1, k); //char group builder
		string flow = stack; //result
		string c; //current char/chargroup

		while (fi >> k) {
			if (d.count(k)) {
				c = d[k];
			} else if (k==last) {
				c = stack + stack[0];
			} else {
				cout << "Bad encoded k = " << k << endl;
			}
			flow += c;
			d[last++] = stack+c[0];
			stack = c;
		}

		ofstream fo("LZW_Decoded.txt");

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
