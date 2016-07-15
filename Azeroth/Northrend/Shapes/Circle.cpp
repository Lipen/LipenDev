#include <iostream>
#include <string>

#include "Circle.hpp"


Circle::Circle(double r) : r(r) {}

double Circle::get_perimeter() const {
	return 2 * 3.14159 * r;
}

double Circle::get_area() const {
	return 3.14159 * r * r;
}

std::string Circle::get_type() const {
	return "CIRCLE";
}
