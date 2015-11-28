#include <iostream>
// TAGS: BINARY SEARCH, BINSEARCH

using std::cout;
using std::cin;
using std::endl;


template<typename T>
int binary_search(T* A, size_t n, T k) {
// Returns 1-based index
	size_t l = 1;
	size_t r = n;

	while (l <= r) {
		size_t m = l + (r-l)/2;  // (l + r) / 2;

		if (A[m - 1] == k)
			return m;
		else if (A[m - 1] > k)
			r = m - 1;
		else
			l = m + 1;
	}

	return -1;
}


int main() {
	size_t n = 11;
	int* data = new int[n] {1,2,3,5,6,8,4,9,10,11,16};

	for (int x = 1; x <= 20; ++x) {
		cout << "Index of " << x << ": " << binary_search(data, n, x) << endl;
	}
}
