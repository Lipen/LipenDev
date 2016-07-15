#include <string>

#ifndef MATRIX2_HPP
#define MATRIX2_HPP

class Matrix2 {
	int data[3][3];

	Matrix2() {}

public:
	Matrix2(int (*matrix)[3][3]);

	int get(int i, int j);

	std::string toString();
};

#endif
