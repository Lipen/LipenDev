#include <iostream>
#include <sstream>
#include <vector>
// TAGS: largest increasing subsequence, dp, dynamic programming

using std::cout;
using std::cin;
using std::endl;


int main() {
	std::stringstream cin;
	// cin << "10\n1 2 3 4 5 6 7 8 9 10";  // ~>> 10
	cin << "10\n5 19 5 81 50 28 29 1 83 23";  // ~>> 5 ?
//
	int n;
	cin >> n;

	std::vector<int> A(n);
	for (int i = 0; i < n; ++i) {
		cin >> A[i];
	}


	std::vector<int> D(n, 1);

	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < i; ++j) {
			if (A[j] < A[i] && D[j]+1 > D[i]) {
				D[i] = D[j] + 1;
			}
		}
	}


	int ans = 0;
	for (int i = 0; i < n; ++i) {
		if (D[i] > ans) {
			ans = D[i];
		}
	}

	cout << ans << endl;
}
