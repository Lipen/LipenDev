/* Copyright (C) 2015, Lipen */
#include <iostream>  	// cout
#include <utility>  	// swap
#include <functional>   // function

using std::cout;
using std::endl;


template<typename T>
class Heap {
using Compare = std::function<bool(T& o1, T& o2)>;

	T* data;
	size_t size_;
	size_t heap_size;
	Compare comp_;

 public:
	Heap(Compare comp) {
		comp_ = comp;
		size_ = 0;
		data = nullptr;

		build_heap();  // or just explicitly heap_size=0
	}

	template<size_t N>
	Heap(T (&a)[N], Compare comp) {
		comp_ = comp;
		// size_ = N;
		// data = new T[size_];

		// for (size_t i = 0; i < size_; ++i) {
		// 	data[i] = a[i];
		// }

		// build_heap();

		// build_heap'
		data = new T[1];
		data[0] = a[0];
		size_ = 1;
		heap_size = 1;
		for (size_t i = 1; i < N; ++i) {
			insert(a[i]);
		}
	}

	Heap(T* a, size_t N, Compare comp) {
		comp_ = comp;
		size_ = N;
		data = a;  // or copy?

		build_heap();
	}

	void heapify(size_t i) {
		size_t l = 2*i + 1;  // 2(i+1)   - 1 = 2i+2-1 = 2i+1
		size_t r = 2*i + 2;  // 2(i+1)+1 - 1          = 2i+2
		size_t largest;

		if (l < heap_size && comp_(data[l], data[i])) {
			largest = l;
		} else {
			largest = i;
		}

		if (r < heap_size && comp_(data[r], data[largest])) {
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

	T get_root() {
		return data[0];
	}

	T extract_root() {
		if (heap_size < 1) {
			cout << "Exception: Queue is empty. heap_size = " << heap_size << endl;
			throw "Queue is empty.";
		}

		T m = data[0];
		data[0] = data[heap_size-1];
		--heap_size;
		--size_;
		heapify(0);

		return m;
	}

	void insert(T key) {
		++heap_size;
		++size_;
		size_t i = heap_size - 1;

		// T* tmp = new T[size_];
		T* tmp = (T*)::operator new[] (sizeof(T)*size_);
		std::copy(data, data+size_-1, tmp);
		tmp[size_-1] = T();
		delete [] data;
		data = tmp;

		while (i > 0 && comp_(key, data[(i-1)/2])) {
			data[i] = data[(i-1)/2];
			i = (i-1)/2;
		}

		data[i] = key;
	}

	void increase_key(size_t i, T k) {
		if (comp_(k, data[i])) {
			while (i > 0 && comp_(k, data[(i-1)/2])) {
				data[i] = data[(i-1)/2];
				i = (i-1)/2;
			}
			data[i] = k;
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


template<typename T>
class MaxHeap: public Heap<T> {
 public:
	template<size_t N>
	MaxHeap(T (&a)[N]) : Heap<T>(a, [](const T& o1, const T& o2) {
		return o1 > o2;
	}) {
		cout << "MaxHeap constructor.\n";
	}

	T get_max() {
		return this->get_root();
	}

	T extract_max() {
		return this->extract_root();
	}
};

template<typename T>
class MinHeap: public Heap<T> {
 public:
	template<size_t N>
	MinHeap(T (&a)[N]) : Heap<T>(a, [](const T& o1, const T& o2) {
		return o1 < o2;
	}) {
		cout << "MinHeap constructor.\n";
	}

	T get_min() {
		return this->get_root();
	}

	T extract_min() {
		return this->extract_root();
	}
};


class HeapNode {
	int data;
	int listNo;

 public:
 	HeapNode() : data(0), listNo(-1) {};
	HeapNode(int data, int listNo) : data(data), listNo(listNo) {}

	int get_data() {
		return data;
	}

	int get_listNo() {
		return listNo;
	}
};


int main() {
	int a[] {4, 1, 3, 2, 16, 9, 10, 14, 8, 7};

	MaxHeap<int> H(a);
	cout << "Initial heap:\n\t"; H.show();

	H.insert(15); cout << "15 inserted:\n\t"; H.show();

	int m = H.extract_root();
	cout << "root(min/max) extracted from heap:\n\t" << m << endl;

	H.insert(30); cout << "30 inserted:\n\t"; H.show();

	H.increase_key(7, 11); cout << "#7 increased up to 11:\n\t"; H.show();

	H.remove(0); cout << "#0 removed:\n\t"; H.show();

	H.heapsort(); cout << "Sorted:\n\t"; H.show();
}
