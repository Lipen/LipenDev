#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <utility>
// TAGS: huffman codes encode

using std::cout;
using std::cin;
using std::endl;
using std::string;

// TODO: include "priority-queue"

template<typename T, typename Compare = std::less<T>>
class PriorityQueue {
	T* data;
	size_t size_;
	size_t heap_size;
	Compare comp;

 public:
	explicit PriorityQueue(const Compare& comp_ = Compare())
	: data(nullptr), size_(0), heap_size(0), comp(std::move(comp_)) {}

	template<size_t N>
	explicit PriorityQueue( T (&a)[N], const Compare& comp_ = Compare() )
	: data(new T[1]), size_(1), heap_size(1), comp(std::move(comp_)) {
		// build_heap':
		data[0] = a[0];
		for (size_t i = 1; i < N; ++i) {
			push(a[i]);
		}
	}

	PriorityQueue( T* a, size_t N, const Compare &comp_ = Compare() )
	: data(a), size_(N), heap_size(0), comp(comp_) {
		// copy data?
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


struct MyPair {
	char character;
	int frequency;

	MyPair() : character('\0'), frequency(0) {}

	MyPair(char c, int f)
	: character(c), frequency(f) {}
};


struct Node {
	MyPair* value;
	Node* left = nullptr;
	Node* right = nullptr;

	Node(MyPair* v) : value(v) {}
	Node(MyPair* v, Node* l, Node* r) : value(v), left(l), right(r) {}

	struct cmp {
		bool operator() ( Node* const &lhs, Node* const &rhs ) const {
			if (lhs->value->frequency == rhs->value->frequency) {
				return false;  // keep order
			}
			return lhs->value->frequency > rhs->value->frequency;
		}
	};

	// Printable interface
	friend std::ostream& operator<< ( std::ostream &o, const Node &rhs ) {
		if (rhs.left == nullptr && rhs.right == nullptr)
			return o << "[" << rhs.value->character << ": " << rhs.value->frequency << "]";
		else {
			return o << "(" << rhs.value->frequency << "){ " << *rhs.left << " :: " << *rhs.right << " }";
		}
	}
};


void build_codes(std::map<char, string> &codes, Node* root, string s) {
	if (root->left == nullptr && root->right == nullptr) {
		codes[root->value->character] = s;
	}
	else {
		build_codes(codes, root->left, s+"0");
		build_codes(codes, root->right, s+"1");
	}
}


int main() {
	std::stringstream cin;
	cin << "abacabad";
//
	string str;
	cout << "Enter string to encode: ";
	cin >> str;
	const char* ptr = str.c_str();

	int frequencies[256] {};  // should be initialized with all zeros
	while (*ptr != '\0') {
		++frequencies[*ptr++];  // [-Wchar-subscripts] !
	}


	PriorityQueue<Node*, Node::cmp> pq;

	for (size_t i = 0; i < 256; ++i) {
		if (frequencies[i] > 0) {
			pq.push(new Node(new MyPair( static_cast<char>(i), frequencies[i] )));
		}
	}

	pq.show_deref();
	cout << "Ordered ";
	pq.show_ordered_deref();


	while (pq.size() > 1) {
		Node* first = pq.pop();
		Node* second = pq.pop();

		pq.push(new Node( new MyPair( '\0', first->value->frequency+second->value->frequency ), first, second ));
	}

	pq.show_deref();


	std::map<char, string> codes;

	// If there is only one unique character - its code is "0"
	if (str.size() > 1)
		build_codes(codes, pq.top(), "");
	else
		codes[str[0]] = "0";

	cout << "Huffman codes:" << endl;
	for (auto entry : codes) {
		cout << "\t" << entry.first << ": " << entry.second << endl;
	}


	std::stringstream answer;
	for (auto c : str) {
		answer << codes[c];
	}
	cout << "Encoded string: " << answer.str() << endl;
}
