#include <string>

#ifndef VECTOR3_HPP
#define VECTOR3_HPP

class Vector3 {
	int x, y, z;

public:
	Vector3(int x, int y, int z);

	int getX();
	int getY();
	int getZ();

	int get(int d);

	std::string toString();
};

#endif
