#include <iostream>
#include <string>

#include "Rect.hpp"


Rect::Rect(double a, double b) : a(a), b(b) {}

double Rect::get_perimeter() const {
	return 2 * (a + b);
}

double Rect::get_area() const {
	return a * b;
}

std::string Rect::get_type() const {
	return "RECT";
}
