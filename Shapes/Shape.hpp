#ifndef SHAPE_HPP
#define SHAPE_HPP


class Shape {

public:
	virtual double get_perimeter() const = 0;
	virtual double get_area() const = 0;
	virtual std::string get_type() const = 0;
	friend std::ostream& operator<<(std::ostream&, const Shape&);
};

#endif
