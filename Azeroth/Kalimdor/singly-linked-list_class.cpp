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
class SinglyLinkedList {
	struct Node {
		T data;
		Node* next;

		explicit Node(T x, Node* n = nullptr)
		: data(x), next(n) {}
	};

	Node* head;

 public:
	explicit SinglyLinkedList(Node* h = nullptr) : head(h) {}

	size_t get_size() {
		size_t n = 0;
		Node* cur = head;

		while (cur) {
			++n;
			cur = cur->next;
		}

		return n;
	}

	void insert(Node* loc, T x) {
		if (loc == nullptr) {
			return;
		}

		Node* node = new Node(x);

		if (!head) {
			head = node;
			return;
		}

		if (loc == head) {
			push_front(x);
			return;
		}


		Node* cur = head;

		while (cur->next) {
			if (cur->next == loc) {
				node->next = cur->next;
				cur->next = node;
				return;
			}

			cur = cur->next;
		}
	}

	void push_back(T x) {
		Node* node = new Node(x);

		if (!head) {
			head = node;
			return;
		}

		Node* cur = head;

		while (cur) {
			if (!cur->next) {
				cur->next = node;
				return;
			}
			cur = cur->next;
		}
	}

	void push_front(T x) {
		Node* node = new Node(x);

		if(!head) {
			head = node;
			return;
		}

		node->next = head;
		head = node;
	}

	T pop_back() {
		Node* cur = head;

		while (cur) {
			if (!cur->next) {  // only could be when cur==head...
				T data = cur->data;
				delete cur;
				head = nullptr;
				return data;
			} else if (!cur->next->next) {
				T data = cur->next->data;
				cur->next = nullptr;
				delete cur->next;
				return data;
			}

			cur = cur->next;
		}

		throw "No element to pop_back.";
	}

	T pop_front() {
		if (!head) {
			throw "No element to pop_front.";
		}

		Node* tmp = head;
		T data = head->data;

		if (head->next) {
			head = head->next;
			delete tmp;
			return data;
		}

		delete tmp;
		head = nullptr;
		return data;
	}

	void erase(Node* loc) {
		if (loc == head) {
			Node* tmp = head->next;
			head = head->next;
			delete tmp;
			return;
		}

		Node* cur = head;

		while (cur) {
			if (cur->next == loc) {
				cur->next = loc->next;  // == cur->next = cur->next->next;
				delete loc;
				return;  // drop explicitly! (not neccessary, check)
			}

			cur = cur->next;
		}
	}

	Node* search(T x) {
		if (!head) {
			return nullptr;
		}

		Node* cur = head;

		while (cur) {
			if (cur->data == x) {
				return cur;
			}

			cur = cur->next;
		}

		return nullptr;
	}

	void show() {
		if (!head) return;

		Node *cur = head;

		while (cur) {
			cout << cur->data << " ";
			cur = cur->next;
		}
		cout << endl;
	}
};


int main() {
	int A[] {32, 71, 85, 75, 28, 3, 29, 82, 55, 16, 57, 92, 79, 98, 1, 13, 11, 56, 9, 31, 15, 2, 64, 14, 5, 27, 89, 22, 40, 18};  // 30
	cout << "Initial array:\n\t"; show(A);

	SinglyLinkedList<int> list;
	for (auto x : A) {
		list.push_back(x);
	}
	cout<<"SinglyLinkedList (size = "<<list.get_size()<<"):\n\t"; list.show();


	list = SinglyLinkedList<int>();
	cout << "\nNew list created.\n\n";

	cout << "push_back(20, 30 40, 50):\n\t";
	list.push_back(20);
	list.push_back(30);
	list.push_back(40);
	list.push_back(50);
	list.show();

	cout << "push_front(10):\n\t";
	list.push_front(10);
	list.show();

	cout << "erase 30:\n\t";
	list.erase(list.search(30));
	list.show();

	cout << "insert 32 before 40:\n\t";
	list.insert(list.search(40), 32);
	list.show();

	cout << "pop_back(): " << list.pop_back() << " just back popped\n\t";
	list.show();

	cout << "pop_front(): " << list.pop_front() << " just front popped\n\t";
	list.show();
}
