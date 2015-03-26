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

	if (fi.is_open()) {
		vector<string> flow;
		long k;
		string c;
		string stack;
		string newstack;
		map<string, int> d;
		for (int i=0; i<256; ++i) {
			d[string(1, i)] = i;
		}
		int last = 256;

		while (fi >> k) {
			//C++11:
			// for (auto it : d)
			// 	if (it.second == k) {
			// 		c = it.first;
			// 		break;
			// 	}
			//C++98:
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
