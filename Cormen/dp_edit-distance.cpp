#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
// TAGS: levenshtein distance, edit distance, dp, dynamic programming
// O(mn) time and space
// see Hirschberg`s algorithm for O(min(n,m)) memory

using std::cout;
using std::cin;
using std::endl;
using std::string;

using VI = std::vector<int>;
using VVI = std::vector<VI>;


// DEBUG
template<typename T>
void show(std::vector<T> v) {
	cout << '\t';
	for (size_t i = 0; i < v.size(); ++i) {
		cout << v[i];
		if (i+1 < v.size()) cout << " ";
	}
	cout << endl;
}
template<typename T>
void show(std::vector<std::vector<T>> v) {
	cout << "vector<vector<T>> [" << v.size() << "] x [" << ((v.size()) ? v[0].size() : 0) << "] {" << endl;
	for (size_t i = 0; i < v.size(); ++i) {
		show(v[i]);
	}
	cout << "}" << endl;
}
// DEBUG END


// Top Down dp
int edit_distance_TD(const string &A, const string &B, VVI &D, int i, int j) {
	if (D[i][j] == std::numeric_limits<int>::max()) {
		if (i == 0) {
			D[i][j] = j;
		}
		else if (j == 0) {
			D[i][j] = i;
		}
		else {
			int ins = edit_distance_TD(A, B, D, i, j-1) + 1;
			int del = edit_distance_TD(A, B, D, i-1, j) + 1;
			int sub = edit_distance_TD(A, B, D, i-1, j-1) + (A[i-1] != B[j-1]);

			D[i][j] = std::min({ ins, del, sub });
		}
	}

	return D[i][j];
}


// Bottom Up dp
int edit_distance_BU(const string &A, const string &B) {
	size_t n = A.size();
	size_t m = B.size();
	VVI D(n+1, VI(m+1));

	for (size_t i = 0; i <= n; ++i) {
		D[i][0] = i;
	}
	for (size_t j = 0; j <= m; ++j) {
		D[0][j] = j;
	}

	for (size_t i = 1; i <= n; ++i) {
		for (size_t j = 1; j <= m; ++j) {
			int ins = D[i][j-1] + 1;
			int del = D[i-1][j] + 1;
			int sub = D[i-1][j-1] + (A[i-1] != B[j-1]);

			D[i][j] = std::min({ ins, del, sub });
		}
	}
	cout << "edit_distance_BU:" << endl;
	show(D);

	return D[n][m];
}


int main() {
	// edi-ting-
	// -distance
	// =  = = ==  ~>> 5
	string s1 = "distance";
	string s2 = "editing";
	size_t n = s1.size();
	size_t m = s2.size();


	VVI D(n+1, VI(m+1, std::numeric_limits<int>::max()));

	int x_td = edit_distance_TD(s1, s2, D, n, m);
	cout << "edit_distance_TD:" << endl;
	show(D);
	cout << x_td << endl;


	int x_bu = edit_distance_BU(s1, s2);
	cout << x_bu << endl;
}
