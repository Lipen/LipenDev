#include <iostream>

using std::cout;
using std::cin;
using std::endl;


template<typename T>
class CircularList {
	struct Node {
		T     data;
		Node* next;
		Node* prev;

		Node(T v, Node* n = nullptr, Node* p = nullptr)
		: data(v), next(n), prev(p) {}

		// Printable interface
		friend std::ostream& operator<< (std::ostream& o, const Node& rhs) {
			o << "[Node: " << rhs.data << "]";
			return o;
		}
	};

	Node* head;
	Node* tail;
	size_t size_;

 public:
 	CircularList() : head(nullptr), tail(nullptr), size_(0) {}

	explicit CircularList(size_t s) : size_(s) {
		if (size_ > 0) {
			head = new Node(1);
			tail = head;

			for (size_t i = 1; i < size_; ++i) {
				Node* new_node = new Node(i+1, nullptr, tail);
				tail->next = new_node;
				tail = new_node;
			}

			tail->next = head;
			head->prev = tail;

			cout << "CircularList constructed." << endl;
		}
		else {
			throw "CircularList :: Size must be positive.";
		}
	}

	~CircularList() {
		Node* current = head;
		for (size_t i = 0; i < size_; ++i) {
			Node* tmp = current;
			current = current->next;
			delete tmp;
		}
	}


	void insert(T value) {
		if (size_ > 1) {
			// cout << "Inserting..." << endl
			Node* new_node = new Node(value, head, tail);
			tail->next = new_node;
			tail = new_node;
			head->prev = tail;
		}
		else if (size_ == 1) {
			// cout << "Inserting into 1-element list..." << endl
			tail = new Node(value, head, head);
			head->next = tail;
			head->prev = tail;
		}
		else {  // size_ == 0
			// cout << "Inserting into empty list..." << endl
			head = new Node(value);
			head->next = head;
			head->prev = head;
			tail = head;
		}

		++size_;
	}

	void remove(Node* node) {
		if (size_ > 1) {
			if (node == head) {
				// cout << "HEAD REMOVE" << endl;
				head->next->prev = tail;
				head = head->next;
				tail->next = head;
			}
			else if (node == tail) {
				// cout << "TAIL REMOVE" << endl;
				tail->prev->next = head;
				tail = tail->prev;
				head->prev = tail;
			}
			else {
				// cout << "NODE REMOVE" << endl;
				node->prev->next = node->next;
			}

			delete node;
		}
		else {
			delete node;
			head = nullptr;
			tail = nullptr;
		}

		--size_;
	}


	T exhaust_by(size_t k) {
		Node* current = head;

		while (size_ > 1) {
			Node* tmp = current;

			for (size_t i = 1; i < k; ++i) {
				tmp = tmp->next;
			}
			current = tmp->next;

			remove(tmp);
			// cout << "exhaust_by :: walk after remove:" << endl; walk();
		}

		return current->data;
	}


	void walk() {
		Node* current = head;

		for (size_t i = 0; i < size_; ++i) {
			cout << "walk :: " << *current << endl;
			current = current->next;
		}

		cout << "walk done (nextlast = " << *current << ")" << endl;
	}

	void walk_reverse() {
		Node* current = head;

		for (size_t i = 0; i < size_; ++i) {
			cout << "walk_reverse :: " << *current << endl;
			current = current->prev;
		}

		cout << "walk_reverse done (nextlast = " << *current << ")" << endl;
	}

	size_t size() const {
		return size_;
	}
};


int main_wrapped() {
	using T = int;

	size_t n = 5;
	size_t k = 3;


	CircularList<T> r(n);
	// r.walk(); r.walk_reverse();

	T ans_r = r.exhaust_by(k);
	cout << "Answer: " << ans_r << endl;


	CircularList<T> u;
	for (size_t i = 1; i <= n; ++i) {
		u.insert(i);
	}
	// u.walk(); u.walk_reverse();

	T ans_u = u.exhaust_by(k);
	cout << "Answer: " << ans_u << endl;


	return 0;
}


int main() {
	try {
		int e = main_wrapped();
		if (e == 0) {
			cout << "OK" << endl;
		}
		else {
			cout << "Main returned errcode = " << e << endl;
		}
	}
	catch (const char* msg) {
		cout << "Exception caught: " << msg << endl;
	}
	catch (...) {
		cout << "Exception caught." << endl;
	}
}

