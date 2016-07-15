/* Copyright (C) 2015, Lipen */
#include <iostream>  	// cout

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


template<typename T>
class Queue {
	T* data;
	size_t size_;
	size_t count;
	size_t head;
	size_t tail;

 public:
 	Queue() : size_(0), count(0), head(0), tail(0) {
 		cout << "Queue::warning: empty queue initiaized." << endl;
 	}

	Queue(size_t N) : size_(N), count(0), head(0), tail(0) {
		data = (T*)::operator new [](size_*sizeof(T));
	}

	size_t get_size() const {
		return size_;
	}

	size_t get_count() const {
		return count;
	}

	void enqueue(T x) {
		if (count < size_) {
			data[tail] = x;
			++count;

			if (tail == size_-1) {
				tail = 0;
			} else {
				++tail;
			}
		} else {
			cout << "Queue::exception: count>=size_ -> overflow!" << endl;
			// throw "Queue overflow.";
		}
	}

	T dequeue() {
		if (count > 0) {
			T x = data[head];
			--count;

			if (head == size_-1) {
				head = 0;
			} else {
				++head;
			}

			return x;
		} else {
			cout << "Queue::exception: count==0 -> underflow!" << endl;
			throw "Queue underflow.";
		}
	}
};


int main() {
	int A[] {32, 71, 85, 75, 28, 3, 29, 82, 55, 16, 57, 92, 79, 98, 1, 13, 11, 56, 9, 31, 15, 2, 64, 14, 5, 27, 89, 22, 40, 18};  // 30
	cout << "Initial:\n\t"; show(A);

	Queue<int> q(size(A));
	for (auto x : A) {
		q.enqueue(x);
	}
	cout << "size = " << q.get_size() << ", count = " << q.get_count() << endl;
	// q.enqueue(999);
	// q.enqueue(992);

	cout << "Dequeue`ed all:\n\t";
	while (q.get_count()) {
		cout << q.dequeue() << " ";
	}
	cout << endl;
}
