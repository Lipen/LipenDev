#include <iostream>
#include <vector>
#include <iomanip>
#include <string>


using std::cout;
using std::cin;
using std::endl;

using C = char;
using I = int;
using VI = std::vector<I>;
using VVI = std::vector<VI>;
template<typename T> using V = std::vector<T>;
template<typename T> using VV = std::vector<V<T>>;


template<typename T>
void printVV( VV<T> &v, size_t start=0, size_t width=5 ) {
	size_t n = v.size();
	size_t m = v[0].size();

	for (size_t i = start; i < n; ++i) {
		for (size_t j = start; j < m; ++j) {
			cout << std::setw(width) << v[i][j];
		}
		cout << endl;
	}
}


size_t LCS_length( VI &X, VI &Y, VVI &c, VV<C> &b ) {
	size_t m = X.size();
	size_t n = Y.size();

	for (size_t i = 1; i <= m; ++i) {
		for (size_t j = 1; j <= n; ++j) {
			if ( X[i-1] == Y[j-1] ) {
				c[i][j] = c[i-1][j-1] + 1;
				b[i][j] = '\\';
			}
			else if (c[i-1][j] >= c[i][j-1]) {
				c[i][j] = c[i-1][j];
				b[i][j] = '^';
			}
			else {
				c[i][j] = c[i][j-1];
				b[i][j] = '<';
			}
		}
	}

	return c[m][n];
}


void print_LCS( VV<C> &b, VI &X, size_t i, size_t j ) {
	if ( i == 0 || j == 0 )
		return;

	if ( b[i][j] == '\\' ) {
		print_LCS(b, X, i-1, j-1);
		cout << X[i-1] << " ";
	}
	else if ( b[i][j] == '^' ) {
		print_LCS(b, X, i-1, j);
	}
	else {
		print_LCS(b, X, i, j-1);
	}
}


int main() {
	// VI X { 1, 2, 3, 2, 4, 1, 2 };
	// VI Y { 2, 4, 3, 1, 2, 1 };
	VI X {1,0,0,1,0,1,0,1};
	VI Y {0,1,0,1,1,0,1,1,0};

	size_t m = X.size();
	size_t n = Y.size();

	VVI   c(m+1, VI(n+1));         // [0..m, 0..n]
	VV<C> b(m+1, V<C>(n+1, '-'));  // [1..m, 1..n]

	size_t len = LCS_length(X, Y, c, b);

	cout << "Matrix C:" << endl;
	printVV(c, 0, 3);
	cout << "Matrix B:" << endl;
	printVV(b, 0, 3);
	cout << "LCS length = " << len << endl;
	cout << "LCS: "; print_LCS(b, X, m, n); cout << endl;
}
