#ifndef MATRIX3_HPP
#define MATRIX3_HPP

#include <string>

class Matrix3 {
	int data[3][3][3];

	Matrix3() {}

public:
	Matrix3(int (*matrix)[3][3][3]);

	int get(int i, int j, int k);

	std::string toString();
};

#endif
