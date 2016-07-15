#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
// TAGS: count sort

using std::cout;
using std::cin;
using std::endl;


std::vector<int> count_sort(std::vector<int>& A) {
	size_t n = A.size();
	auto mm = std::minmax_element(A.begin(), A.end());
	int amin = *mm.first;
	std::vector<size_t> B(*mm.second - amin + 1);

	for (size_t i = 0; i < n; ++i) {
		++B[A[i]-amin];
	}

	std::partial_sum(B.begin(), B.end(), B.begin());

	std::vector<int> As(n);
	for (size_t i = n; i > 0; --i) {
		As[B[A[i-1]-amin]---1] = A[i-1];
	}

	return As;
}


int random(int xmin, int xmax) {
	return rand()%(xmax-xmin+1) + xmin;
}


int main() {
{
	std::stringstream cin;
	cin << "5\n2 3 9 2 9";
//
	size_t n;
	cin >> n;

	std::vector<int> A(n);
	for (size_t i = 0; i < n; ++i) {
		cin >> A[i];
	}

	std::vector<int> As = count_sort(A);

	for (size_t i = 0; i < n; ++i) {
		cout << As[i];
		if (i+1 < n)  cout << " ";
	}
	cout << endl;
}
{
	srand(time(0));
	const size_t n = 100;
	constexpr int xmin = 3;
	constexpr int xmax = 15;

	std::vector<int> A;
	for (size_t i = 0; i < n; ++i) {
		A.push_back(random(xmin, xmax));
	}

	std::vector<int> As = count_sort(A);

	cout << "As:" << endl;
	for (size_t i = 0; i < n; ++i) {
		cout << As[i];
		if (i+1 < n)  cout << " ";
	}
	cout << endl;
}
}
