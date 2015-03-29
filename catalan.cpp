#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>

#define pb push_back

using namespace std;

int main() {
	int n;
	cin >> n;
	clock_t timeStart = clock();
	vector<string> tree(1, "(");

	for (int i=0; i<2*n-1; ++i) {
		vector<string> newtree;
		//C++11:
		// for (string b : tree) {
		//C++03:
		for (vector<string>::iterator b = tree.begin(); b != tree.end(); ++b) {
			int left = count((*b).begin(), (*b).end(), '(');
			if (n - left) {
				newtree.pb(*b + '(');
			}
			if ((*b).size() - left < left) {
				newtree.pb(*b + ")");
			}
		}
		tree = newtree;
	}

	//C++11:
	// for (string b : tree) {
	//C++03:
	for (vector<string>::iterator b = tree.begin(); b != tree.end(); ++b) {
		cout << *b << endl;
	}
	cout << tree.size() << endl;
	cout << "Time: " << (double)(clock() - timeStart)/CLOCKS_PER_SEC << " seconds" << endl;
}
