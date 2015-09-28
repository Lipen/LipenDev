#ifndef RIGHTTRIANGLE_HPP
#define RIGHTTRIANGLE_HPP

#include "Shape.hpp"


class RightTriangle : public Shape {
	double a, b;

public:
	RightTriangle(double, double);

	virtual double get_perimeter() const;
	virtual double get_area() const;
	virtual std::string get_type() const;
};

#endif
