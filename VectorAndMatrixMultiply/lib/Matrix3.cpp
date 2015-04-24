#include <string>

#include "Matrix3.hpp"

#define FOR(i, n) for(int i = 0; i < (n); i++)

Matrix3::Matrix3(int (*matrix)[3][3][3]) {
	FOR(i, 3)
		FOR(j, 3)
			FOR(k, 3)
				data[i][j][k] = (*matrix)[i][j][k];
};

int Matrix3::get(int i, int j, int k) {
	return data[i][j][k];
}

std::string Matrix3::toString() {
	std::string s;

	FOR(i, 3) {
		s += "Slice i = " + std::to_string(i+1) + ":\n";
		FOR(j, 3) {
			s += '\t';
			FOR(k, 3) {
				s += std::to_string(data[i][j][k]);
				if (k < 2) s += ' ';
			}
			if (j < 2 || i < 2) s += '\n';
		}
	}

	return s;
}
