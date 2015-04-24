#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define FOR(i, n) for(int i = 0; i < (n); i++)

// Predeclare
class Matrix3;
class Matrix2;
class Vector;


class Matrix3 {
	int data[3][3][3];

public:
	Matrix3(int (*matrix)[3][3][3]) {
		FOR(i, 3)
			FOR(j, 3)
				FOR(k, 3)
					data[i][j][k] = (*matrix)[i][j][k];
	};

	int get(int i, int j, int k) {
		return data[i][j][k];
	}

	string toString() {
		string s;

		FOR(i, 3) {
			s += "Slice i = " + to_string(i+1) + ":\n";
			FOR(j, 3) {
				s += '\t';
				FOR(k, 3) {
					s += to_string(data[i][j][k]);
					if (k < 2) s += ' ';
				}
				if (j < 2 || i < 2) s += '\n';
			}
		}

		return s;
	}
};


class Matrix2 {
	int data[3][3];

public:
	Matrix2(int (*matrix)[3][3]) {
		FOR(i, 3)
			FOR(j, 3)
				data[i][j] = (*matrix)[i][j];
	}

	int get(int i, int j) {
		return data[i][j];
	}

	string toString() {
		string s;

		FOR(i, 3) {
			FOR(j, 3) {
				s += to_string(data[i][j]);
				if (j < 2) s += ' ';
			}
			if (i < 2) s += '\n';
		}

		return s;
	}
};


class Vector {
	int x, y, z;

public:
	Vector(int x, int y, int z): x(x), y(y), z(z) {};

	int getX() { return x; }
	int getY() { return y; }
	int getZ() { return z; }

	int get(int d) {
		switch (d) {
			case 0:
				return getX();
			case 1:
				return getY();
			case 2:
				return getZ();
		}
		return 0;
	}

	string toString() {
		return "{"+to_string(x)+", "+to_string(y)+", "+to_string(z)+"}";
	}
};


Matrix2 MultiplyMatrix3byVector(Matrix3 M, Vector V) {
	int A[3][3];

	FOR(j, 3)
		FOR(k, 3) {
			A[j][k] = 0;
			FOR(i, 3)
				A[j][k] += M.get(i, j, k)*V.get(i);
		}

	return Matrix2(&A);
}


int main()
{
	ifstream fi("input.txt");

	if (fi) {
		ofstream fo("output.txt");

		if (fo) {
			int matrix[3][3][3];
			int vec[3];

			FOR(i, 3)
				FOR(j, 3)
					FOR(k, 3)
						fi >> matrix[i][j][k];

			Matrix3 M(&matrix);
			cout << "Matrix:\n" << M.toString() << '\n';

			FOR(i, 3)
				fi >> vec[i];

			Vector V(vec[0], vec[1], vec[2]);
			cout << "Vector: " << V.toString() << '\n';

			Matrix2 result = MultiplyMatrix3byVector(M, V);
			string ans = result.toString();
			cout << "Answer:\n" << ans << '\n';

			fo << ans;
			fo.close();
		} else {
			cout << "Unable to open output file :C" << endl;
		}
		fi.close();
	} else {
		cout << "Unable to open input file :c" << endl;
	}
	return 0;
}
