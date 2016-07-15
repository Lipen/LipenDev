/* Copyright (C) 2015, Lipen */
#include <iostream>  	// cout
#include <utility>  	// swap
#include <functional>   // less, greater

using std::cout;
using std::endl;


template<typename T, typename Compare = std::less<T>>
class Heap {
	T* data;
	size_t size_;
	size_t heap_size;
	Compare comp;

 public:
	explicit Heap(const Compare& comp_ = Compare())
	: data(nullptr), size_(0), heap_size(0), comp(std::move(comp_)) {}

	template<size_t N>
	explicit Heap( T (&a)[N], const Compare& comp_ = Compare() )
	: data(new T[1]), size_(1), heap_size(1), comp(std::move(comp_)) {
		// build_heap':
		data[0] = a[0];
		for (size_t i = 1; i < N; ++i) {
			push(a[i]);
		}
	}

	Heap( T* a, size_t N, const Compare &comp_ = Compare() )
	: data(a), size_(N), heap_size(0), comp(comp_) {
		// Copy data (a) if necessary
		build_heap();
	}

	explicit Heap(const Heap &other)
	: size_(other.size_), heap_size(other.heap_size), comp(other.comp) {
		data = new T[size_];
		std::copy(other.data, other.data+size_, data);
	}

	void heapify(size_t i) {
		size_t l = 2*i + 1;  // 2*i   ~>> 2(i+1)   - 1 = 2i+1
		size_t r = 2*i + 2;  // 2*i+1 ~>> 2(i+1)+1 - 1 = 2i+2
		size_t largest;

		if (l < heap_size && comp(data[i], data[l])) {
			largest = l;
		} else {
			largest = i;
		}

		if (r < heap_size && comp(data[largest], data[r])) {
			largest = r;
		}

		if (largest != i) {
			std::swap(data[i], data[largest]);
			heapify(largest);
		}
	}

	void build_heap() {
		heap_size = size_;

		for (int i = size_/2-1; i >= 0; --i) {
			heapify(i);
		}
	}

	void heapsort() {
		build_heap();

		for (int i = size_; i >= 1; --i) {
			std::swap(data[0], data[i-1]);
			--heap_size;
			heapify(0);
		}
	}

	T top() {
		return data[0];
	}

	T pop() {
		if (heap_size < 1) {
			throw "Heap is empty";
		}

		T m = data[0];
		data[0] = data[heap_size-1];
		--heap_size;
		--size_;
		heapify(0);

		return m;
	}

	void push(const T &key) {
		++heap_size;
		++size_;
		size_t i = heap_size - 1;

		T* tmp = new T[size_] ();
		std::copy(data, data+size_-1, tmp);
		tmp[size_-1] = T();
		delete [] data;
		data = tmp;

		while (i > 0 && comp(data[(i-1)/2], key)) {
			data[i] = data[(i-1)/2];
			i = (i-1)/2;
		}

		data[i] = key;
	}

	void increase_key(size_t i, T k) {
		if (k < data[i]) {
			cout << "Cannot increase key" << endl;
			return;
		}

		data[i] = k;
		while (i > 0 && comp(data[(i+1)/2 - 1], data[i])) {
			std::swap(data[i], data[(i+1)/2 - 1]);
			i = (i+1)/2 - 1;
		}
	}

	void remove(size_t i) {
		std::swap(data[i], data[size_-1]);
		--heap_size;
		--size_;
		heapify(i);
	}

	void show() {
		for (size_t i = 0; i < size_; ++i) {
			cout << data[i] << " ";
		}
		cout << endl;
	}
};


int main_wrapped() {
	int a[] {4, 1, 3, 2, 16, 9, 10, 14, 8, 7};
cout << "MaxHeap tests:" << endl;
{
	Heap<int> H(a);  // MaxHeap by default
	cout << "Initial heap:\n\t";  H.show();

	H.push(15); cout << "15 inserted:\n\t"; H.show();

	int m = H.pop();
	cout << "pop from heap:\n\t" << m << endl;

	H.push(30);  cout << "30 inserted:\n\t";  H.show();

	H.increase_key(7, 11); cout << "#7 (0-based) increased up to 11:\n\t";  H.show();

	H.remove(2); cout << "#2 removed:\n\t"; H.show();

	H.heapsort(); cout << "Sorted:\n\t"; H.show();
}
cout << "\nMinHeap tests:" << endl;
{
	Heap<int, std::greater<int>> H(a);
	cout << "Initial min-heap:\n\t";  H.show();

	H.push(15); cout << "15 inserted:\n\t"; H.show();

	int m = H.pop();
	cout << "pop from heap:\n\t" << m << endl;

	H.push(30);  cout << "30 inserted:\n\t";  H.show();

	H.increase_key(7, 11); cout << "#7 (0-based) increased up to 11:\n\t";  H.show();

	H.remove(2); cout << "#2 removed:\n\t"; H.show();

	H.heapsort(); cout << "Sorted:\n\t"; H.show();
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
