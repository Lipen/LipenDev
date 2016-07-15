/* Copyright (c) 2015, Lipen */
#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
#include <algorithm>
// TAGS: matrix chain order, matrix multiply order, dp, dynamic programming

using std::cout;
using std::cin;
using std::endl;

using I = int;
using VI = std::vector<I>;
using VVI = std::vector<VI>;


template<typename T>
void show(const std::vector<std::vector<T>> &v, size_t start = 0, size_t width = 5) {
	size_t n = v.size();

	for (size_t i = start; i < n; ++i) {
		cout << '\t';
		for (size_t j = start; j < n; ++j) {
			cout << std::setw(width) << v[i][j];
		}
		cout << endl;
	}
}


void print_optimal_parens(const VVI &s, size_t i, size_t j) {
	if (i == j) {
		cout << "A";
		if (i == 0)
			cout << "₀";
		else if (i == 1)
			cout << "₁";
		else if (i == 2)
			cout << "₂";
		else if (i == 3)
			cout << "₃";
		else if (i == 4)
			cout << "₄";
		else if (i == 5)
			cout << "₅";
		else if (i == 6)
			cout << "₆";
		else if (i == 7)
			cout << "₇";
		else if (i == 8)
			cout << "₈";
		else if (i == 9)
			cout << "₉";
		else
			cout << "?";
		cout << "";
	}
	else {
		cout << "(";
		print_optimal_parens(s, i, s[i][j]);
		print_optimal_parens(s, s[i][j]+1, j);
		cout << ")";
	}
}


// Cormen`s
void matrix_chain_order(const VI &p, VVI &m, VVI &s) {
	size_t n = p.size() - 1;
	cout << "n = " << n << endl;

	for (size_t l = 2; l <= n; ++l) {
		for (size_t i = 1; i <= n-l+1; ++i) {
			size_t j = i + l - 1;
			m[i][j] = std::numeric_limits<I>::max();

			for (size_t k = i; k <= j-1; ++k) {
				I q = m[i][k] + m[k+1][j] + p[i-1]*p[k]*p[j];

				if (q < m[i][j]) {
					m[i][j] = q;
					s[i][j] = k;
				}
			}
		}
	}
}


// Bottom Up dp
int matrix_mult_BU(const VI &m) {  // m_0 .. m_n    (n+1 total)
	size_t n = m.size() - 1;

	VVI D(n, VI(n, std::numeric_limits<int>::max()));

	for (size_t i = 0; i < n; ++i) {
		D[i][i] = 0;
	}

	for (size_t s = 1; s <= n-1; ++s) {
		for (size_t i = 1; i <= n-s; ++i) {
			size_t j = i + s;

			for (size_t k = i; k <= j-1; ++k) {
				D[i-1][j-1] = std::min( D[i-1][j-1], D[i-1][k-1] + D[k+1-1][j-1] + m[i-1]*m[k]*m[j] );
			}
		}
	}
	cout << "D:" << endl;
	show(D, 0, 10);

	return D[0][n-1];
}


int main() {
	VI p { 5, 10, 3, 12, 5, 50, 6 };
	// VI p { 30, 35, 15, 5, 10, 20, 25 };
	// VI p {40, 20, 30, 10, 30};
	size_t n = p.size();
	VVI m(n, VI(n));
	VVI s(n, VI(n));

	cout << "matrix_chain_order() .." << endl;
	matrix_chain_order(p, m, s);
	cout << "Done." << endl;

	cout << "Matrix m:" << endl;
	show(m, 1, 6);
	cout << "Matrix s:" << endl;
	show(s, 1, 3);

	cout << "Optimal multiplications:" << endl;
	cout << '\t' << m[1][n-1] << endl;

	cout << "Optimal parens:" << endl;
	print_optimal_parens(s, 1, n-1); cout << endl;


	int x = matrix_mult_BU(p);
	cout << "dp: x = " << x << endl;
}
