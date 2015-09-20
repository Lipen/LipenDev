/* Copyright (C) 2015, Lipen */
#include <iostream>
#include <cstdint>

using std::cout;
using std::endl;


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
void swap(T (&A)[N], size_t i, size_t j) {
	T t = A[i];
	A[i] = A[j];
	A[j] = t;
}


template<typename T, size_t N>
void heapify(T (&A)[N], size_t i, size_t heap_size) {
	// cout<<"Heapify(i="<<i<<",heap_size="<<heap_size<<"):\n\t"; show(A);
	size_t l = 2*i + 1;  // 2(i+1)   - 1 = 2i+2-1 = 2i+1
	size_t r = 2*i + 2;  // 2(i+1)+1 - 1          = 2i+2
	size_t largest;

	if (l < heap_size && A[l] > A[i]) {
		largest = l;
	} else {
		largest = i;
	}

	if (r < heap_size && A[r] > A[largest]) {
		largest = r;
	}

	if (largest != i) {
		// cout << "Before swap(" << i << ", " << largest << "):\n\t"; show(A);
		swap(A, i, largest);
		// cout << "After swap:\n\t"; show(A);
		heapify(A, largest, heap_size);
	}
}

template<typename T, size_t N>
void build_heap(T (&A)[N]) {
	// size_t heap_size = size(A);  // == N, so we already got it.
	for (int i = N/2-1; i >= 0; i--) {
		heapify(A, i, N);
	}
}

template<typename T, size_t N>
void heapsort(T (&A)[N]) {
	build_heap(A);

	for (int i = N; i >= 1; i--) {
		// cout << "heapsort::i = " << i << "\n\t"; show(A);
		swap(A, 0, i-1);
		heapify(A, 0, i-1);  // heap_sort--, but we knew that it was N..
	}
}


int main() {
	// int A[] { 16, 4, 10, 14, 7, 9, 3, 2, 8, 1 };
	int A[] { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7 };

	show(A);

	heapsort(A);

	show(A);
}
