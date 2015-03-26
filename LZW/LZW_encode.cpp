#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <string>

#define VI vector<int>
#define VVI vector<VI>
#define FOR(i, n) for(int i = 0; i < (n); i++)
#define FORS(i, si, n) for(int i = (si); i < (n); i++)
#define FORE(i, n) for(int i = 0; i <= (n); i++)
#define FORES(i, si, n) for(int i = (si); i <= (n); i++)
#define pb push_back

using namespace std;

int n,m,x,a,b,c,d;

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
			// cout << "Stack: " << stack << endl;
		}
		flow.pb(stack); // BEWARE! :O

		ofstream fo("LZW_Encoded.txt");
		if (fo.is_open()) {
			//
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
