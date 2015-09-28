#include <iostream>
#include <string>
#include <cmath>

#include "RightTriangle.hpp"


RightTriangle::RightTriangle(double a, double b) : a(a), b(b) {}

double RightTriangle::get_perimeter() const {
	return a + b + sqrt(a*a + b*b);
}

double RightTriangle::get_area() const {
	return a * b / 2.0;
}

std::string RightTriangle::get_type() const {
	return "TRIANGLE";
}
