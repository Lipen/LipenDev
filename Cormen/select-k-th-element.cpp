#include <iostream>   // cout
#include <utility>    // swap
#include <cstdlib>    // rand
#include <ctime>      // time
#include <algorithm>  // max, shuffle

using std::cout;
using std::endl;


template<typename T, size_t N>
constexpr size_t size(T (&)[N]) { return N; }

template<typename T, size_t N>
void show(T (&A)[N]) {
	for (size_t i = 0; i < N; i++) {
		cout << A[i] << " ";
	}
	cout << endl;
}

template<typename T>
constexpr T random(T xmin, T xmax) {
	return rand() % (xmax - xmin + 1) + xmin;
}


// Cormen`s.
// template<typename T, size_t N>
// size_t partition(T (&A)[N], size_t p = 0, size_t r = N-1) {
// 	T x = A[p];
// 	size_t i = p;
// 	size_t j = r;

// 	while (true) {
// 		while (A[j] > x) { --j; }
// 		while (A[i] < x) { ++i; }

// 		if (i < j) {
// 			std::swap(A[i], A[j]);
// 		} else {
// 			return j;
// 		}
// 	}
// }

template<typename T, size_t N>
size_t partition(T (&A)[N], size_t l = 0, size_t r = N-1) {
	T x = A[l];
	size_t j = l;

	for (size_t i = l+1; i <= r; ++i) {
		if (A[i] < x) {
			++j;
			std::swap(A[j], A[i]);
		}
	}

	std::swap(A[l], A[j]);
	return j;
}

template<typename T, size_t N>
size_t partition_randomized(T (&A)[N], size_t p = 0, size_t r = N-1) {
	std::swap(A[p], A[random(p, r)]);
	return partition(A, p, r);
}


template<typename T, size_t N>
size_t select_simple(T (&A)[N], size_t k = 0) {  // k = 0..N-1
	for (size_t i = 0; i <= k; ++i) {
		size_t min_index = i;
		T min_value = A[i];

		for (size_t j = i+1; j < N; ++j) {
			if (A[j] < min_value) {
				min_index = j;
				min_value = A[j];
			}
		}

		std::swap(A[i], A[min_index]);
	}

	return A[k];
}

template<typename T, size_t N>
size_t select_randomized(T (&A)[N], size_t i = 1, size_t p = 0, size_t r = N-1) {  // Cormen`s
	if (p == r) {
		return A[p];
	}

	size_t q = partition_randomized(A, p, r);
	size_t k = q - p + 1;

	if (i <= k) {
		return select_randomized(A, i, p, q);
	} else {
		return select_randomized(A, i-k, q+1, r);
	}
}


int main_wrapped() {
 { cout << "Select-Randomized tests:" << endl;
	int A[] {32, 71, 85, 75, 28, 3, 29, 82, 55, 16, 57, 92, 79, 98, 1, 13, 11, 56, 9, 31, 15, 2, 64, 14, 5, 27, 89, 22, 40, 18};  // 30
	cout << "Initial:\n\t";  show(A);

	for (size_t i = 1; i <= size(A); ++i) {
		int s = select_randomized(A, i);
		cout << "Select-Randomized("<<i<<") >> " << s << endl;
	}

	cout << "Post:\n\t";  show(A);
 }

 { cout << "\nSelect-Simple tests:" << endl;
	int A[] {32, 71, 85, 75, 28, 3, 29, 82, 55, 16, 57, 92, 79, 98, 1, 13, 11, 56, 9, 31, 15, 2, 64, 14, 5, 27, 89, 22, 40, 18};  // 30
	cout << "Initial:\n\t";  show(A);

	for (size_t i = 0; i < size(A); ++i) {
		size_t s = select_simple(A, i);
		cout << "Select-Simple("<<i<<") >> " << s << endl;
	}

	cout << "Post:\n\t";  show(A);
 }
	return 0;
}

int main() {
	try {
		int e = main_wrapped();
		if (e == 0) {
			cout << "\nOK" << endl;
		}
		else {
			cout << "\nMain returned errcode = " << e << endl;
		}
	}
	catch (const char* msg) {
		cout << "\nException caught: " << msg << endl;
	}
	catch (...) {
		cout << "\nSome exception caught." << endl;
	}
}
