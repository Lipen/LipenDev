/* Copyright (C) 2015, Lipen */
#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::endl;


template <typename T>
T* resize_expand(T* data, size_t &capacity) {
	T* tmp = new T[capacity * 2];

	for (size_t i = 0; i < capacity; ++i) {
		tmp[i] = data[i];
	}

	delete[] data;
	data = 0;
	capacity *= 2;
	// cout << "realloced(expand) to " << capacity << endl;

	return tmp;
}

template <typename T>
T* resize_reduce(T* data, size_t &capacity) {
	T* tmp = new T[capacity / 2];

	for (size_t i = 0; i < capacity/4; ++i) {
		tmp[i] = data[i];
	}

	delete[] data;
	data = 0;
	capacity /= 2;
	// cout << "realloced(reduce) to " << capacity << endl;

	return tmp;
}


template <typename T>
struct StackInterface {
	virtual void push(T c) = 0;
	virtual void pop() = 0;
	virtual bool empty() const = 0;
	virtual T    top() const = 0;
	virtual void makeEmpty() = 0;
};


template <typename T>
class StackArray : public StackInterface<T> {
	T*     data;
	size_t head;  // 0 is reserved!
	size_t capacity;

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
			data = resize_expand(data, capacity);
		}

		data[++head] = element;
	}

	void pop() {
		--head;
		if (head < capacity/4) {
			data = resize_reduce(data, capacity);
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
class StackList : public StackInterface<T> {
	struct Node {
		T     value;
		Node* next;

		Node(T v, Node* n) : value(v), next(n) {}
	};

	Node*  head;
	size_t size_;  // Just to be able to access size w/ O(1)

 public:
	StackList() : head(nullptr), size_(0) {}

	~StackList() {
		for (Node* next = head; next; ) {
			Node* old = next;
			next = old->next;
			delete old;
		}
		// head = 0;  // ???
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
		head = nullptr;  // need..
		size_ = 0;
	}
};


int main() {
	char stack_type;
	cin >> stack_type;

	StackInterface<char>* stack;
	if (stack_type == 'a') {
		stack = new StackArray<char>;
	} else if (stack_type == 'l') {
		stack = new StackList<char>;
	} else {
		throw "Invalid stack type.";
	}


	cin.ignore(1);  // ignore '\n' on the first line
	std::string line;
	std::getline(cin, line);

	for (char c : line) {
		if (c == '#') {
			stack->pop();
		} else if (c == '@') {
			stack->makeEmpty();
		} else {
			stack->push(c);
		}
	}


	StackInterface<char>* tmp;
	if (stack_type == 'a') {
		tmp = new StackArray<char>;
	} else if (stack_type == 'l') {
		tmp = new StackList<char>;
	}

	while (!stack->empty()) {
		tmp->push(stack->top());
		stack->pop();
	}

	while (!tmp->empty()) {
		cout << tmp->top();
		tmp->pop();
	}


	// try {
	// 	// StackList<char> stack_list;

	// 	// stack_list.push('a');
	// 	// stack_list.push('b');
	// 	// stack_list.push('c');
	// 	// stack_list.push('d');
	// 	// cout << "Stack size: " << stack_list.size() << endl;

	// 	// cout << stack_list.top() << endl; stack_list.pop();
	// 	// cout << stack_list.top() << endl; stack_list.pop();
	// 	// cout << stack_list.top() << endl; stack_list.pop();
	// 	// cout << "Stack size: " << stack_list.size() << endl;
	// 	// stack_list.push('X');
	// 	// cout << "Stack size: " << stack_list.size() << endl;
	// 	// cout << stack_list.top() << endl; stack_list.pop();
	// 	// cout << stack_list.top() << endl; stack_list.pop();

	// 	// stack_list.push('U');
	// 	// cout << "Stack size: " << stack_list.size() << endl;
	// 	// stack_list.makeEmpty();
	// 	// // cout << "Stack size: " << stack_list.size() << endl;
	// 	// stack_list.push('Q');
	// 	// stack_list.push('W');
	// 	// cout << "Stack size: " << stack_list.size() << endl;
	// 	// cout << stack_list.top() << endl; stack_list.pop();
	// 	// cout << stack_list.top() << endl; stack_list.pop();
	// 	// cout << "Stack size: " << stack_list.size() << endl;
	// 	// stack_list.pop();
	// 	// cout << "Stack size: " << stack_list.size() << endl;


	// 	// StackArray<char> stack_array;

	// 	// stack_array.push('a');
	// 	// stack_array.push('b');
	// 	// stack_array.push('c');
	// 	// stack_array.push('d');
	// 	// stack_array.push('F');

	// 	// cout << stack_array.pop() << endl;
	// 	// cout << stack_array.pop() << endl;
	// 	// cout << stack_array.pop() << endl;
	// 	// stack_array.push('X');
	// 	// // stack_array.print_();
	// 	// cout << stack_array.pop() << endl;
	// 	// cout << stack_array.pop() << endl;
	// 	// cout << stack_array.pop() << endl;
	// 	// stack_array.push('w');
	// 	// stack_array.push('r');
	// 	// stack_array.makeEmpty();
	// 	// stack_array.push('t');
	// 	// stack_array.push('y');
	// 	// cout << stack_array.pop() << endl;
	// 	// cout << stack_array.pop() << endl;
	// } catch (const char* msg) {
	// 	cout << "Exc: " << msg << endl;
	// } catch (...) {
	// 	cout << "Exc: uncaught!" << endl;
	// }
}
