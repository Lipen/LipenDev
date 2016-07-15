#include <string>

#include "Matrix2.hpp"

#define FOR(i, n) for(int i = 0; i < (n); i++)

Matrix2::Matrix2(int (*matrix)[3][3]) {
	FOR(i, 3)
		FOR(j, 3)
			data[i][j] = (*matrix)[i][j];
}

int Matrix2::get(int i, int j) {
	return data[i][j];
}

std::string Matrix2::toString() {
	std::string s;

	FOR(i, 3) {
		FOR(j, 3) {
			s += std::to_string(data[i][j]);
			if (j < 2) s += ' ';
		}
		if (i < 2) s += '\n';
	}

	return s;
}
