/* Copyright (C) 2015, Lipen */
#include <iostream>
#include <string>
#include <sstream>

using std::cout;
using std::cin;
using std::endl;


template<typename T>
struct IStack {
	virtual ~IStack() {}
	virtual void push(T c)     = 0;
	virtual void pop()         = 0;
	virtual bool empty() const = 0;
	virtual T    top()   const = 0;
	virtual void makeEmpty()   = 0;
};


template <typename T>
class StackArray : public IStack<T> {
	T*     data;
	size_t head;  // 0 is a Sentinel
	size_t capacity;

	void reduce() {
		T* tmp = new T[capacity / 2];

		for (size_t i = 0; i < capacity/4; ++i) {
			tmp[i] = data[i];
		}

		delete[] data;
		data = tmp;
		capacity /= 2;
	}

	void expand() {
		T* tmp = new T[capacity * 2];

		for (size_t i = 0; i < capacity; ++i) {
			tmp[i] = data[i];
		}

		delete[] data;
		data = tmp;
		capacity *= 2;
	}

 public:
	explicit StackArray(size_t capa = 1) : head(0), capacity(capa) {
		if (capa == 0) throw "Stack: capacity must be positive!";

		data = new T[capacity];
	}

	~StackArray() {
		delete[] data;
	}

	void push(T element) {
		if (head+1 >= capacity) {
			expand();
		}
		data[++head] = element;
	}

	void pop() {
		--head;
		if (head < capacity/4) {
			reduce();
		}
	}

	bool empty() const {
		return head == 0;
	}

	T top() const {
		if (empty()) throw "Stack::top: stack is empty!";

		return data[head];
	}

	void makeEmpty() {
		data = new T[capacity];
		head = 0;
	}
};


template <typename T>
class StackList : public IStack<T> {
	struct Node {
		T     value;
		Node* next;

		Node(T v, Node* n) : value(v), next(n) {}
	};

	Node*  head;
	size_t size_;

 public:
	StackList() : head(nullptr), size_(0) {}

	~StackList() {
		for (Node* next = head; next; ) {
			Node* old = next;
			next = old->next;
			delete old;
		}
	}

	void push(T element) {
		head = new Node(element, head);
		++size_;
	}

	void pop() {
		if (!empty()) {
			Node* old = head;
			head = head->next;
			delete old;
			--size_;
		}
	}

	bool empty() const {
		return size_ == 0;
	}

	T top() const {
		if (empty()) throw "Stack::top: stack is empty!";

		return head->value;
	}

	size_t size() const {
		return size_;
	}

	void makeEmpty() {
		for (Node* next = head; next; ) {
			Node* old = next;
			next = old->next;
			delete old;
		}
		head = nullptr;
		size_ = 0;
	}
};

#include "C:/Data/OneDrive/Programming/dev/Stuff/TValidator.ipp"

int main() {
	// typedef char T;
	typedef TValidator<char> T;

	char stack_type;
	cin >> stack_type;

	// Determine which stack to use:
	IStack<T>* stack;
	if (stack_type == 'a') {
		stack = new StackArray<T>;
	} else if (stack_type == 'l') {
		stack = new StackList<T>;
	} else {
		throw "Invalid stack type.";
	}

	// Read second line:
	cin.ignore(1);  // ignore remaining '\n' on the first line
	std::string line;
	cin >> line;

	for (char c : line) {
		if (c == '#') {
			stack->pop();
		} else if (c == '@') {
			stack->makeEmpty();
		} else {
			stack->push(T(c));
		}
	}

	// Reverse result using another stack:
	IStack<T>* tmp;
	if (stack_type == 'a') {
		tmp = new StackArray<T>;
	} else if (stack_type == 'l') {
		tmp = new StackList<T>;
	}

	while (!stack->empty()) {
		tmp->push(stack->top());
		stack->pop();
	}

	// Print answer:
	std::stringstream ss;
	while (!tmp->empty()) {
		ss << tmp->top();
		tmp->pop();
	}
	cout << ss.str() << endl;


	cout << "END." << endl;
}
