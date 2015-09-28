#include <iostream>
#include <string>
#include <vector>

#include "Shape.hpp"
#include "Rect.hpp"
#include "RightTriangle.hpp"
#include "Circle.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::string;


Shape* min_perimeter(Shape** shapes, int size) {
	int j = 0;
	double m = shapes[0]->get_perimeter();

	for (int i = 1; i < size; ++i) {
		double t = shapes[i]->get_perimeter();

		if (t < m) {
			m = t;
			j = i;
		}
	}

	return shapes[j];
}


int main() {
	int n;
	cin >> n; cin.ignore(2147483647, '\n');

	std::vector<Shape*> v;

	for (int i = 0; i < n; ++i) {
		Shape* sh;
		char c;
		cin.get(c);

		if (c == 'R') {
			double a, b;
			cin >> a >> b;
			sh = new Rect(a, b);
		} else if (c == 'T') {
			double a, b;
			cin >> a >> b;
			sh = new RightTriangle(a, b);
		} else if (c == 'C') {
			double r;
			cin >> r;
			sh = new Circle(r);
		}

		if (sh) {
			v.push_back(sh);
		}

		cin.ignore(2147483647, '\n');
	}

	// Print all shapes:
	// for (auto* s : shapes) {
	// 	cout << *s << endl;
	// }

	int size = v.size();
	Shape** shapes = new Shape*[size];
	std::copy(v.begin(), v.end(), shapes);

	cout << *min_perimeter(shapes, size) << endl;

	cout << "End." << endl;

	// ### TEST ###

	// Rect r(2, 5); 			cout << r << endl;
	// RightTriangle t(3, 4);	cout << t << endl;
	// Circle c(6);			cout << c << endl;

	// Shape* r = new Rect(2, 5);
	// cout << *r << endl;

	// Shape* t = new RightTriangle(3, 4);
	// cout << *t << endl;

	// Shape* c = new Circle(6);
	// cout << *c << endl;
}
