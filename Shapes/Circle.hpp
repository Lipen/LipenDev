#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "Shape.hpp"


class Circle : public Shape {
	double r;

public:
	explicit Circle(double);

	virtual double get_perimeter() const;
	virtual double get_area() const;
	virtual std::string get_type() const;
};

#endif
