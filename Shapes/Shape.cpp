#include <iostream>

#include "Shape.hpp"


std::ostream& operator<<(std::ostream& o, const Shape& s) {
	return o << s.get_type() << " P=" << s.get_perimeter() << " S=" << s.get_area();
}
