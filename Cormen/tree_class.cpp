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
struct TreeNode {
	size_t key;
	T data;
	TreeNode* parent = nullptr;
	TreeNode* left = nullptr;
	TreeNode* right = nullptr;

	TreeNode() = delete;
	explicit TreeNode(size_t k) : key(k) {}
	TreeNode(size_t k, T d) : key(k), data(d) {}

	friend std::ostream& operator<<(std::ostream& o, const TreeNode<T>& node) {
		return o << "[TreeNode: key="<<node.key<<", data="<<node.data<<"]";
	}
};


template<typename Node>
class Tree {
	Node* root = nullptr;

 public:
	Tree() { cout << "Tree initialized." << endl; }

	Node* get_root() {
		return root;
	}

	size_t get_size(Node* x) {
		if (x == nullptr) {
			return 0;
		}
		return get_size(x->left) + get_size(x->right) + 1;
	}
	size_t get_size() {
		return get_size(root);
	}

	void inorder_walk(Node* x) {
		if (x != nullptr) {
			inorder_walk(x->left);
			cout << "Visited: " << *x << endl;
			inorder_walk(x->right);
		}
	}
	void inorder_walk() {
		inorder_walk(root);
	}

	Node* search(Node* x, size_t k) {
		if (x == nullptr || k == x->key) {
			return x;
		}

		if (k < x->key) {
			return search(x->left, k);
		} else {
			return search(x->right, k);
		}
	}

	Node* search_iterative(Node* x, size_t k) {
		while (x != nullptr && k != x->key) {
			if (k < x->key) {
				x = x->left;
			} else {
				x = x->right;
			}
		}

		return x;
	}
	Node* search(size_t k) {
		return search_iterative(root, k);
	}

	Node* minimum(Node* x) {
		while (x->left != nullptr) {
			x = x->left;
		}

		return x;
	}
	Node* minimum() {
		return minimum(root);
	}

	Node* maximum(Node* x) {
		while (x->right != nullptr) {
			x = x->right;
		}

		return x;
	}
	Node* maximum() {
		return maximum(root);
	}

	Node* successor(Node* x) {
		if (x->right != nullptr) {
			return minimum(x->right);
		}

		Node* y = x->parent;

		while (y != nullptr && x == y->right) {
			x = y;
			y = y->parent;
		}

		return y;
	}

	Node* predecessor(Node* x) {
		if (x->left != nullptr) {
			return maximum(x->left);
		}

		Node* y = x->parent;

		while (y != nullptr && x == y->left) {
			x = y;
			y = y->parent;
		}

		return y;
	}

	void insert(Node* z) {
		Node* y = nullptr;
		Node* x = root;

		while (x != nullptr) {
			y = x;

			if (z->key < x->key) {
				x = x->left;
			} else {
				x = x->right;
			}
		}

		z->parent = y;

		if (y == nullptr) {
			root = z;
		} else {
			if (z->key < y->key) {
				y->left = z;
			} else {
				y->right = z;
			}
		}
	}

	Node* cut(Node* z) {
		Node* x, y;

		if (z->left == nullptr || z->right == nullptr) {
			y = z;
		} else {
			y = successor(z);
		}

		if (y->left != nullptr) {
			x = y->left;
		} else {
			x = y->right;
		}

		if (x != nullptr) {
			x->parent = y->parent;
		}

		if (y->parent == nullptr) {
			root = x;
		} else {
			if (y == y->parent->left) {
				y->parent->left = x;
			} else {
				y->parent->right = x;
			}
		}

		if (y != z) {
			z->key = y->key;
			// copy everything else from y to z
		}

		return y;
	}

	void remove(Node* z) {
		Node* y = cut(z);

		delete y;
	}
};


int main() {
	int A[] {32, 71, 85, 75, 28, 3, 29, 82, 55, 16, 57, 92, 79, 98, 1, 13, 11, 56, 9, 31, 15, 2, 64, 14, 5, 27, 89, 22, 40, 18};  // 30
	cout << "Initial array:\n\t"; show(A);

	Tree<TreeNode<int>> t;
	for (auto x : A) {
		TreeNode<int>* node = new TreeNode<int>(x, -x*x);
		t.insert(node);
	}
	cout << "Tree size = " << t.get_size() << endl;
	cout << "Tree min = " << *t.minimum() << ", max = " << *t.maximum() << endl;
	cout << "Inorder-walk:" << endl; t.inorder_walk();
}
