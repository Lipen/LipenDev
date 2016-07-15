#include <iostream>

using std::cout;
using std::endl;


int main(int argc, char* argv[]) {
	int sum = 0;

	#pragma omp parallel for reduction(+:sum) num_threads(4)
	{
		for (int i = 0; i < 1e6; i++) {
			sum++;
		}
	}

	cout << "Sum = " << sum << endl;

	return 0;
}
