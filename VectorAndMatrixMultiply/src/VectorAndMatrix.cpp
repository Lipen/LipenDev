#include <iostream>
#include <fstream>
#include <string>

#include "Matrix3.hpp"
#include "Matrix2.hpp"
#include "Vector3.hpp"

using namespace std;

#define FOR(i, n) for(int i = 0; i < (n); i++)


Matrix2 MultiplyMatrix3byVector(Matrix3 M, Vector3 V) {
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
	ofstream fo("output.txt");

	if (fi && fo) {
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

		Vector3 V(vec[0], vec[1], vec[2]);
		cout << "Vector: " << V.toString() << '\n';

		Matrix2 result = MultiplyMatrix3byVector(M, V);
		string ans = result.toString();
		cout << "Answer:\n" << ans << '\n';

		fo << ans;

		fi.close();
		fo.close();
	} else {
		cout << "Unable to open input or output file :c" << endl;
	}
	return 0;
}
