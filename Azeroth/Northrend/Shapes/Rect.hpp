#ifndef RECT_HPP
#define RECT_HPP

#include "Shape.hpp"


class Rect : public Shape {
	double a, b;

public:
	Rect(double, double);

	virtual double get_perimeter() const;
	virtual double get_area() const;
	virtual std::string get_type() const;
};

#endif
