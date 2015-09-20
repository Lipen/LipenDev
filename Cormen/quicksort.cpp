#include <iostream>  	// cout
#include <utility>  	// swap
#include <random>  		// default_random_engine, uniform_int_distribution
#include <algorithm>  	// max, shuffle

#include <ctime>  		// clock

using std::cout;
using std::endl;

static std::default_random_engine RandomEngine;


template<typename T, size_t N>
size_t size(T (&)[N]) { return N; }

template<typename T, size_t N>
void show(T (&A)[N]) {
	for (int i = 0; i < static_cast<int>(size(A)); i++) {
		cout << A[i] << " ";
	}
	cout << endl;
}


template<typename T, size_t N>
size_t median_of_3(T (&A)[N], size_t p, size_t r) {
	if (r-p+1 < 3) {
		return p;
	}

	T a = A[p];
	T b = A[r];
	T c = A[(p+r)/2];

	if (a >= b && a >= c) {
		if (b >= c) {
			return r;  // b
		} else {
			return (p+r)/2;  // c
		}
	} else if (b >= a && b >= c) {
		if (a >= c) {
			return p;  // a
		} else {
			return (p+r)/2;  // c
		}
	} else {
		if (a >= b) {
			return p;  // a
		} else {
			return r;  // b
		}
	}
}

template<typename T, size_t N>
size_t median_of_3_randomized(T (&A)[N], size_t p, size_t r) {
	if (r-p+1 < 3) {
		return p;
	}

	std::uniform_int_distribution<size_t> generator(p, r);
	T a = A[generator(RandomEngine)];
	T b = A[generator(RandomEngine)];
	T c = A[generator(RandomEngine)];

	if (a >= b && a >= c) {
		if (b >= c) {
			return r;  // b
		} else {
			return (p+r)/2;  // c
		}
	} else if (b >= a && b >= c) {
		if (a >= c) {
			return p;  // a
		} else {
			return (p+r)/2;  // c
		}
	} else {
		if (a >= b) {
			return p;  // a
		} else {
			return r;  // b
		}
	}
}


template<typename T, size_t N>
size_t partition(T (&A)[N], size_t p = 0, size_t r = N-1) {
	T x = A[p];
	size_t i = p;
	size_t j = r;

	while (true) {
		while (A[j] > x) { --j; }
		while (A[i] < x) { ++i; }

		if (i < j) {
			std::swap(A[i], A[j]);
		} else {
			return j;
		}
	}
}

template<typename T, size_t N>
size_t partition_median_of_3(T (&A)[N], size_t p = 0, size_t r = N-1) {
	std::swap(A[p], A[median_of_3(A, p, r)]);
	return partition(A, p, r);
}

template<typename T, size_t N>
size_t partition_randomized(T (&A)[N], size_t p = 0, size_t r = N-1) {
	std::uniform_int_distribution<size_t> generator(p, r);
	std::swap(A[p], A[generator(RandomEngine)]);
	return partition(A, p, r);
}

template<typename T, size_t N>
size_t partition_randomized_median_of_3(T (&A)[N], size_t p = 0, size_t r = N-1) {
	std::swap(A[p], A[median_of_3_randomized(A, p, r)]);
	return partition(A, p, r);
}

template<typename T, size_t N>
size_t partition_lomuto(T (&A)[N], size_t p = 0, size_t r = N-1) {
	T x = A[r];
	size_t i = p - 1;

	for (size_t j = p; j <= r; ++j) {
		if (A[j] <= x) {
			std::swap(A[++i], A[j]);
		}
	}

	if (i < r) {
		return i;
	} else {
		return i - 1;
	}
}

template<typename T, size_t N>
size_t partition_lomuto_median_of_3(T (&A)[N], size_t p = 0, size_t r = N-1) {
	std::swap(A[p], A[median_of_3(A, p, r)]);
	return partition_lomuto(A, p, r);
}

template<typename T, size_t N>
size_t partition_lomuto_randomized(T (&A)[N], size_t p = 0, size_t r = N-1) {
	std::uniform_int_distribution<size_t> generator(p, r);
	std::swap(A[p], A[generator(RandomEngine)]);
	return partition_lomuto(A, p, r);
}

template<typename T, size_t N>
size_t partition_lomuto_randomized_median_of_3(T (&A)[N], size_t p = 0, size_t r = N-1) {
	std::swap(A[p], A[median_of_3_randomized(A, p, r)]);
	return partition_lomuto(A, p, r);
}


template<typename T, size_t N>
void quicksort(T (&A)[N], size_t p = 0, size_t r = N-1) {
	if (p < r) {
		size_t q = partition(A, p, r);
		quicksort(A, p, q);
		quicksort(A, q+1, r);
	}
}

template<typename T, size_t N>
void quicksort_median_of_3(T (&A)[N], size_t p = 0, size_t r = N-1) {
	if (p < r) {
		size_t q = partition_median_of_3(A, p, r);
		quicksort_median_of_3(A, p, q);
		quicksort_median_of_3(A, q+1, r);
	}
}

template<typename T, size_t N>
void quicksort_randomized(T (&A)[N], size_t p = 0, size_t r = N-1) {
	if (p < r) {
		size_t q = partition_randomized(A, p, r);
		quicksort_randomized(A, p, q);
		quicksort_randomized(A, q+1, r);
	}
}

template<typename T, size_t N>
void quicksort_randomized_median_of_3(T (&A)[N], size_t p = 0, size_t r = N-1) {
	if (p < r) {
		size_t q = partition_randomized_median_of_3(A, p, r);
		quicksort_randomized_median_of_3(A, p, q);
		quicksort_randomized_median_of_3(A, q+1, r);
	}
}

template<typename T, size_t N>
void quicksort_lomuto(T (&A)[N], size_t p = 0, size_t r = N-1) {
	if (p < r) {
		size_t q = partition_lomuto(A, p, r);
		quicksort_lomuto(A, p, q);
		quicksort_lomuto(A, q+1, r);
	}
}

template<typename T, size_t N>
void quicksort_lomuto_median_of_3(T (&A)[N], size_t p = 0, size_t r = N-1) {
	if (p < r) {
		size_t q = partition_lomuto_median_of_3(A, p, r);
		quicksort_lomuto_median_of_3(A, p, q);
		quicksort_lomuto_median_of_3(A, q+1, r);
	}
}

template<typename T, size_t N>
void quicksort_lomuto_randomized(T (&A)[N], size_t p = 0, size_t r = N-1) {
	if (p < r) {
		size_t q = partition_lomuto_randomized(A, p, r);
		quicksort_lomuto_randomized(A, p, q);
		quicksort_lomuto_randomized(A, q+1, r);
	}
}

template<typename T, size_t N>
void quicksort_lomuto_randomized_median_of_3(T (&A)[N], size_t p = 0, size_t r = N-1) {
	if (p < r) {
		size_t q = partition_lomuto_randomized_median_of_3(A, p, r);
		quicksort_lomuto_randomized_median_of_3(A, p, q);
		quicksort_lomuto_randomized_median_of_3(A, q+1, r);
	}
}


int main() {
	int A[] {3082, 5020, 6240, 9254, 8651, 5644, 9893, 7619, 1168, 3696, 6874, 3602, 7720, 4383, 3980, 3721, 6910, 3650, 5103, 8481, 1172, 8930, 4266, 3059, 1593, 2666, 9017, 2496, 7514, 7974, 2937, 9845, 4433, 6805, 1604, 9988, 4960, 9740, 7571, 5305, 5405, 6173, 6795, 2105, 8892, 1724, 7312, 2993, 6951, 5169, 7378, 3514, 7178, 7550, 9462, 9655, 8417, 6487, 3074, 9168, 5328, 3392, 8360, 8173, 7240, 2506, 5768, 7278, 6133, 3752, 7270, 4930, 9234, 9447, 3419, 1117, 2889, 8608, 3945, 5519, 4569, 5666, 7489, 4787, 1161, 4281, 1573, 1439, 5407, 1717, 3951, 9008, 9078, 8000, 4588, 4109, 3883, 2192, 1622, 1966, 2855, 2739, 2837, 4121, 3574, 9986, 3829, 1783, 5004, 2215, 6193, 8582, 4880, 3913, 9379, 8567, 3367, 2508, 6783, 3302, 9383, 8335, 9003, 2717, 8627, 6138, 1620, 7816, 5326, 3932, 6635, 2906, 6102, 4681, 5886, 8158, 5011, 6426, 5607, 3185, 7533, 7092, 2990, 2316, 3575, 9703, 9497, 3706, 7932, 8125, 8393, 8362, 2311, 9744, 9803, 1514, 6916, 7067, 2543, 7662, 3044, 2952, 4497, 3070, 9092, 8426, 2769, 6114, 1575, 9666, 9705, 7655, 8643, 4517, 3127, 2011, 9459, 2075, 1700, 7461, 8653, 2207, 3584, 1328, 9701, 9212, 5253, 5088, 4715, 5398, 7171, 5670, 6477, 7076, 2625, 1231, 8705, 6777, 9200, 1245};

	// int B[] {4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
	// cout << "Initial:\n\t"; show(B);

	// quicksort(B);
	// cout << "Sorted:\n\t"; show(B);


	int N_stress = 50000;
	cout << "Stress test with N=" << N_stress << " for size=" << size(A) << " started..." << endl;
	double time_ = clock();  // strict: clock_t type

	for (int i = 0; i < N_stress; ++i) {
		std::shuffle(std::begin(A), std::end(A), RandomEngine);
		std::sort(std::begin(A), std::end(A));
	}
	cout << "Default: " << (time_=clock() - time_)/CLOCKS_PER_SEC << " seconds" << endl;

	for (int i = 0; i < N_stress; ++i) {
		std::shuffle(std::begin(A), std::end(A), RandomEngine);
		quicksort(A);
	}
	cout << "Quicksort: " << (time_=clock() - time_)/CLOCKS_PER_SEC << " seconds" << endl;

	for (int i = 0; i < N_stress; ++i) {
		std::shuffle(std::begin(A), std::end(A), RandomEngine);
		quicksort_lomuto(A);
	}
	cout << "Quicksort-Lomuto: " << (time_=clock() - time_)/CLOCKS_PER_SEC << " seconds" << endl;

	for (int i = 0; i < N_stress; ++i) {
		std::shuffle(std::begin(A), std::end(A), RandomEngine);
		quicksort_randomized(A);
	}
	cout << "Quicksort-Randomized: " << (time_=clock() - time_)/CLOCKS_PER_SEC << " seconds" << endl;

	for (int i = 0; i < N_stress; ++i) {
		std::shuffle(std::begin(A), std::end(A), RandomEngine);
		quicksort_lomuto_randomized(A);
	}
	cout << "Quicksort-Lomuto-Randomized: " << (time_=clock() - time_)/CLOCKS_PER_SEC << " seconds" << endl;

	for (int i = 0; i < N_stress; ++i) {
		std::shuffle(std::begin(A), std::end(A), RandomEngine);
		quicksort_median_of_3(A);
	}
	cout << "Quicksort-Median-of-3: " << (time_=clock() - time_)/CLOCKS_PER_SEC << " seconds" << endl;

	for (int i = 0; i < N_stress; ++i) {
		std::shuffle(std::begin(A), std::end(A), RandomEngine);
		quicksort_lomuto_median_of_3(A);
	}
	cout << "Quicksort-Lomuto-Median-of-3: " << (time_=clock() - time_)/CLOCKS_PER_SEC << " seconds" << endl;

	for (int i = 0; i < N_stress; ++i) {
		std::shuffle(std::begin(A), std::end(A), RandomEngine);
		quicksort_randomized_median_of_3(A);
	}
	cout << "Quicksort-Randomized-Median-of-3: " << (time_=clock() - time_)/CLOCKS_PER_SEC << " seconds" << endl;

	for (int i = 0; i < N_stress; ++i) {
		std::shuffle(std::begin(A), std::end(A), RandomEngine);
		quicksort_lomuto_randomized_median_of_3(A);
	}
	cout << "Quicksort-Lomuto-Randomized-Median-of-3: " << (time_=clock() - time_)/CLOCKS_PER_SEC << " seconds" << endl;
}
