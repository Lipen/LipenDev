#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

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
	cin >> n;

	std::vector<Shape*> v;


	for (int i = 0; i < n; ++i) {
		Shape* sh;
		char c;
		cin >> c;

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
	}


	int size = v.size();
	Shape** shapes = new Shape*[size];
	std::copy(v.begin(), v.end(), shapes);

	cout << std::setprecision(2) << std::fixed << *min_perimeter(shapes, size) << endl;
}
