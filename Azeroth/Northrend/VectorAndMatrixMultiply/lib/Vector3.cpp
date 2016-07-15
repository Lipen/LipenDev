#include <string>

#include "Vector3.hpp"

Vector3::Vector3(int x, int y, int z): x(x), y(y), z(z) {};

int Vector3::getX() { return x; }
int Vector3::getY() { return y; }
int Vector3::getZ() { return z; }

int Vector3::get(int d) {
	switch (d) {
		case 0:
			return getX();
		case 1:
			return getY();
		case 2:
			return getZ();
	}
	return 0;
}

std::string Vector3::toString() {
	return "{"+std::to_string(x)+", "+std::to_string(y)+", "+std::to_string(z)+"}";
}
