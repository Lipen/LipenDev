/* Copyright (C) 2015, Lipen */
#include <iostream>    // cin, cout, endl
#include <sstream>     // stringstream
#include <ostream>     // ostream, flush
#include <string>      // string
#include <vector>      // vector
#include <utility>     // swap
#include <functional>  // less, greater
// TAGS: heap, priority queue

using std::cout;
using std::cin;
using std::endl;
using std::string;


template<typename T, typename Compare = std::less<T>>
class PriorityQueue {
	T* data;
	size_t size_;
	size_t heap_size;
	Compare comp;

 public:
	explicit PriorityQueue(const Compare &comp_ = Compare())
	: data(nullptr), size_(0), heap_size(0), comp(std::move(comp_)) {}

	template<size_t N>
	explicit PriorityQueue( T (&a)[N], const Compare &comp_ = Compare() )
	: data(new T[1]), size_(1), heap_size(1), comp(std::move(comp_)) {
		// build_heap':
		data[0] = a[0];
		for (size_t i = 1; i < N; ++i) {
			push(a[i]);
		}
	}

	PriorityQueue( T* a, size_t N, const Compare &comp_ = Compare() )
	: data(a), size_(N), heap_size(0), comp(comp_) {
		// Copy data (a) if necessary
		build_heap();
	}

	explicit PriorityQueue(const PriorityQueue &other)
	: size_(other.size_), heap_size(other.heap_size), comp(other.comp) {
		data = new T[size_];
		std::copy(other.data, other.data+size_, data);
	}


	~PriorityQueue() {
		delete [] data;
	}


	bool empty() {
		return size_ == 0;
	}

	size_t size() {
		return size_;
	}

	T top() {
		return data[0];
	}

	T pop() {
		if (heap_size < 1) {
			throw "Queue is empty.";
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

	void heapsort() {
		build_heap();

		for (int i = size_; i >= 1; --i) {
			std::swap(data[0], data[i-1]);
			--heap_size;
			heapify(0);
		}
	}


	void show() {
		cout << "PriorityQueue: [ ";
		for (size_t i = 0; i < size_; ++i) {
			cout << data[i];
			if (i+1 < size_) cout << ", ";
		}
		cout << " ]" << endl;
	}
	void show_deref() {
		cout << "PriorityQueue: [ ";
		for (size_t i = 0; i < size_; ++i) {
			cout << *data[i];
			if (i+1 < size_) cout << ", ";
		}
		cout << " ]" << endl;
	}

	void show_ordered() {
		PriorityQueue tmp(*this);
		tmp.heapsort();
		tmp.show();
	}
	void show_ordered_deref() {
		PriorityQueue tmp(*this);
		tmp.heapsort();
		tmp.show_deref();
	}


 private:
	void heapify(size_t i) {
		size_t l = 2*i + 1;  // 2(i+1)   - 1 = 2i+2-1 = 2i+1
		size_t r = 2*i + 2;  // 2(i+1)+1 - 1          = 2i+2
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
};


struct BellNode {
	size_t number;
	size_t interval;
	size_t next_time;

	BellNode(size_t num, size_t in, size_t nt)
	: number(num), interval(in), next_time(nt) {}

	struct cmp {
		// Min heap  ~>  std::greater
		bool operator() ( BellNode* const &lhs, BellNode* const &rhs ) const {
			if (lhs->next_time == rhs->next_time) {
				return lhs->number > rhs->number;
			}
			return lhs->next_time > rhs->next_time;
		}
	};

	// Printable interface
	friend std::ostream& operator<< ( std::ostream &o, const BellNode &rhs ) {
		return o << "[BellNode: " << rhs.number << " / " << rhs.interval << " / " << rhs.next_time << "]";
	}
};


int main_wrapped() {
{ /* Test default 'less' comparator */
	int a[] {4, 1, 3, 2, 16, 9, 10, 14, 7};

	PriorityQueue<int> pq(a);
	cout << "Default std::less ~> max-heap:" << endl;
	pq.show();
	cout << "std::less ~> ascending order:" << endl;
	pq.show_ordered();

	while (!pq.empty()) {
		cout << "\tPop ~> " << pq.pop() << endl;
	}
	pq.show();
}
{ /* Test 'greater' comparator */
	int a[] {14, 29, 42, 30, 44, 24, 47, 2, 17, 25, 23, 9};

	PriorityQueue<int, std::greater<int>> pq(a);
	cout << "std::greater ~> min-heap:" << endl;
	pq.show();
	cout << "std::greater ~> descending order:" << endl;
	cout << "Heapsorted "; pq.show_ordered();

	while (!pq.empty()) {
		cout << "\tPop ~> " << pq.pop() << endl;
	}
	pq.show();
}
{ /* Test empty */
	PriorityQueue<int> pq;
	cout << "Some empty:" << endl;
	pq.show();
}
{ /* Lab5 */
	std::stringstream cin;
	cin << "2 6\n3\n2";  // ~>> 2
	// cin << "2 3\n7\n3";  // ~>> 1
//
	size_t n, k;
	cout << "Enter <n>, <k> and n <intervals>:" << endl;
	cin >> n >> k;

	std::vector<BellNode*> bells;
	PriorityQueue<BellNode*, BellNode::cmp> q;

	for (size_t i = 0; i < n; ++i) {
		size_t ti;
		cin >> ti;
		BellNode* bell = new BellNode(i+1, ti, ti);

		bells.push_back(bell);
		q.push(bell);
	}


	BellNode* ans;

	// t for time; j for bell counter
	for (size_t t = 0, j = 0; ; ) {
		BellNode* bell = q.pop();

		if (bell->next_time == t) {
			cout << ">bell at " << t << ": " << *bell << endl;
			bell->next_time += bell->interval;  // Modify
			if (++j >= k) {  // k-th bell ~> found answer
				ans = bell;
				break;
			}
		}
		q.push(bell);  // Push it back

		// If top and popped bell had same next_time, repeat actions
		// Else: ++t  ~>  proceed in time
		if (q.top()->next_time > t) {
			++t;
		}
	}

	cout << "Total answer: " << ans->number << endl;
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
