#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <limits>
#include <cmath>
// TAGS: merge sort, mergesort

using std::cout;
using std::cin;
using std::endl;


// DEBUG
template<typename T, size_t N>
size_t size(T (&)[N]) { return N; }

template<typename T, size_t N>
void show(T (&A)[N]) {
	for (int i = 0; i < N; i++) {
		cout << A[i] << " ";
	}
	cout << endl;
}

template<typename T>
void show(std::vector<T> v) {
	cout << '\t';
	for (size_t i = 0; i < v.size(); ++i) {
		cout << v[i] << " ";
	}
	cout << endl;
}
// DEBUG END


template<typename T>
std::vector<T> mergesort(std::vector<T> A) {
	std::deque<std::vector<T>> Q;
	for (size_t i = 0; i < A.size(); ++i) {
		Q.push_back(std::vector<T>{A[i]});
	}


	while (Q.size() > 1) {
		// Merge:
		std::vector<T> v1 = Q.front(); Q.pop_front();
		std::vector<T> v2 = Q.front(); Q.pop_front();
		std::vector<T> v;
		size_t n1 = v1.size();
		size_t n2 = v2.size();
		size_t i = 0, j = 0;

		while (i < n1 && j < n2) {
			if (v1[i] <= v2[j]) {
				v.push_back(v1[i++]);
			}
			else {
				v.push_back(v2[j++]);
			}
		}

		while (i < n1) {
			v.push_back(v1[i++]);
		}
		while (j < n2) {
			v.push_back(v2[j++]);
		}

		Q.push_back(v);
	}

	return Q.front();
}


int main() {
	std::stringstream cin;
	cin << "5\n2 3 9 2 9";
//  ========
	size_t n;
	cin >> n;

	std::vector<int> A;
	for (size_t i = 0; i < n; ++i) {
		int tmp;
		cin >> tmp;
		A.push_back(tmp);
	}

	cout << "Initial:" << endl;
	show(A);


	auto As = mergesort(A);


	cout << "Sorted:" << endl;
	show(As);
}
