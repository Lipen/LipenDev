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
		size_ = N;
		data = new T[size_];

		for (size_t i = 0; i < size_; ++i) {
			data[i] = a[i];
		}

		build_heap();
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


template<size_t K, size_t N>
int* merge_k_arrays(int (&arr)[K][N]) {
	size_t NK = N*K;
	int* result = new int[NK];
	size_t j = 0;
	int* ptrs = new int[K];
	for (size_t i = 0; i < K; ++i) {
		ptrs[i] = 0;
	}

	Heap<HeapNode> heap([](HeapNode& o1, HeapNode& o2) {
		return o1.get_data() < o2.get_data();
	});

	for (size_t i = 0; i < K; ++i) {
		heap.insert(HeapNode(arr[i][0], i));
	}

	while (j < NK) {
		HeapNode m = heap.extract_root();
		result[j++] = m.get_data();
		int listNo = m.get_listNo();
		++ptrs[listNo];
		if (static_cast<size_t>(ptrs[listNo]) < N) {
			heap.insert(HeapNode(arr[listNo][ptrs[listNo]], listNo));
		} else {
			cout << "Insterting +INF from " << listNo << endl;
			heap.insert(HeapNode(2147483647, listNo));
		}
	}

	// cout << "j = " << j << endl;
	// for (size_t i = 0; i < j; ++i) {
	// 	cout << result[i] << " ";
	// } cout << endl;

	return result;
}


int main() {
	int arr[][4] = {{2, 6, 12, 34},
					{1, 9, 20, 1000},
					{23, 34, 90, 2000}};

	int* res = merge_k_arrays(arr);

	for (size_t i = 0; i < 12; ++i) {
		cout << res[i] << " ";
	}
	cout << endl;
}
