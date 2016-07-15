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


enum Color { BLACK, RED };

template<typename T>
struct TreeNode {
	size_t key;
	T data;
	Color color;
	TreeNode* parent = nullptr;
	TreeNode* left = nullptr;
	TreeNode* right = nullptr;

	TreeNode() = delete;
	explicit TreeNode(size_t k, Color c = BLACK) : key(k), color(c) {}
	TreeNode(size_t k, T d, Color c = BLACK) : key(k), data(d), color(c) {}

	friend std::ostream& operator<<(std::ostream& o, const TreeNode<T>& node) {
		// return o << "[TreeNode: key=" << node.key << ", data=" << node.data << ", color = " << (node.color?"Red":"Black") << "]";
		return o << "[TN(" << &node << "): k=" << node.key << ", c=" << (node.color?"R":"B") << ", l="<<node.left << ", r=" << node.right << ", p=" << node.parent << "]";
	}
};


template<typename Node>
class RedBlackTree {
	Node* root = nullptr;
	// Node* nil;

 public:
	RedBlackTree() {
		// nil = new Node(0, BLACK);
		// nil->left = nil->right = nil->parent = nil;
		cout << "Red-Black Tree initialized." << endl;
	}

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

	void rotate_left(Node* x) {
		if (x->right == nullptr) {
			cout << "Exception: x->right == nullptr" << endl;
			return;
		}

		Node* y = x->right;
		x->right = y->left;

		if (y->left != nullptr) {
			y->left->parent = x;
		}

		y->parent = x->parent;

		if (x->parent == nullptr) {
			root = y;
		} else {
			if (x == x->parent->left) {
				x->parent->left = y;
			} else {
				x->parent->right = y;
			}
		}

		y->left = x;
		x->parent = y;
	}

	void rotate_right(Node* y) {
		if (y->left == nullptr) {
			cout << "Exception: y->left == nullptr" << endl;
			return;
		}

		Node* x = y->left;
		y->left = x->right;

		if (x->right != nullptr) {
			x->right->parent = y;
		}

		x->parent = y->parent;

		if (y->parent == nullptr) {
			root = x;
		} else {
			if (y == y->parent->left) {
				y->parent->left = x;
			} else {
				y->parent->right = x;
			}
		}

		x->right = y;
		y->parent = x;
	}

	void insert_tree(Node* z) {  // Tree::insert
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

	void insert(Node* x) {
		insert_tree(x);
		x->color = RED;

		while (x != root && x->parent->color == RED) {
			if (x->parent == x->parent->parent->left) {
				Node* y = x->parent->parent->right;

				if (y != nullptr && y->color == RED) {  // case #1
					x->parent->color = BLACK;
					y->color = BLACK;
					x->parent->parent->color = RED;
					x = x->parent->parent;
				} else {
					if (x == x->parent->right) {  // case #2
						x = x->parent;
						rotate_left(x);
					}
					// case #3
					x->parent->color = BLACK;
					x->parent->parent->color = RED;
					rotate_right(x->parent->parent);
				}
			} else {
				Node* y = x->parent->parent->left;

				if (y != nullptr && y->color == RED) {  // case #4?
					x->parent->color = BLACK;
					y->color = BLACK;
					x->parent->parent->color = RED;
					x = x->parent->parent;
				} else {
					if (x == x->parent->left) {  // case #5?
						x = x->parent;
						rotate_right(x);
					}
					// case #6?
					x->parent->color = BLACK;
					x->parent->parent->color = RED;
					rotate_left(x->parent->parent);
				}
			}
		}

		root->color = BLACK;
	}

	void cut_fixup(Node* x) {
		while (x != root && x->color == BLACK) {
			if (x == x->parent->left) {
				Node* w = x->parent->right;

				if (w->color == RED) {
					w->color = BLACK;
					x->parent->color = BLACK;
					rotate_left(x->parent);
					w = x->parent->right;
				}

				if (w->left->color == BLACK && w->right->color == BLACK) {
					w->color = RED;
					x = x->parent;
				} else {
					if (w->right->color == BLACK) {
						w->left->color = BLACK;
						w->color = RED;
						rotate_right(w);
						w = x->parent->right;
					}

					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->right->color = BLACK;
					rotate_left(x->parent);
					x = root;
				}
			} else {
				Node* w = x->parent->right;

				if (w->color == RED) {
					w->color = BLACK;
					x->parent->color = BLACK;
					rotate_left(x->parent);
					w = x->parent->right;
				}

				if (w->left->color == BLACK && w->right->color == BLACK) {
					w->color = RED;
					x = x->parent;
				} else {
					if (w->right->color == BLACK) {
						w->left->color = BLACK;
						w->color = RED;
						rotate_right(w);
						w = x->parent->right;
					}

					w->color = x->parent->color;
					x->parent->color = BLACK;
					w->right->color = BLACK;
					rotate_left(x->parent);
					x = root;
				}
			}
		}

		x->color = BLACK;
	}

	Node* cut(Node* z) {
		Node* x = nullptr;
		Node* y = nullptr;

		if (z->left == nullptr || z->right == nullptr) {
			y = z;
		} else {
			y = successor(z);
		}
		// cout << "361: " << y << endl;

		if (/*y != nullptr && */y->left != nullptr) {
			x = y->left;
		} else {
			x = y->right;
		}
		// cout << "367: x = " << x << ", y = " << y << endl;

		/////
		if (x == nullptr) {
			x = new Node(0, BLACK);  // nil!
			x->parent = y;
			y->left = x;
		}
		/////

		x->parent = y->parent;
		// cout << "378: x = " << x << ", y = " << y << endl;

		if (/*y != nullptr && */y->parent == nullptr) {
			root = x;
			// root = nullptr;
			cout << "382" << endl;
		} else {
			// cout << "388" << endl;
			if (y == y->parent->left) {
				y->parent->left = x;
			} else {
				y->parent->right = x;
			}
			// cout << "390" << endl;
		}

		if (y != z) {
			z->key = y->key;
			z->data = y->data;  // + copy other data if necessary
		}
		// cout << "397" << endl;

		if (y->color == BLACK) {
			// cout << "400" << endl;
			inorder_walk();
			// cout << "402" << endl;
			cut_fixup(x);
			// cout << "404" << endl;
		}
		// cout << "406" << endl;

		/////// Remove sentinel...
		if (x != nullptr) {
			if (x->parent != nullptr) {
				if (x == x->parent->left) {
					x->parent->left = nullptr;
				} else {
					x->parent->right = nullptr;
				}
			}

			delete x;
		}
		x = nullptr;
		///////

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

	RedBlackTree<TreeNode<double>> t;
	cout << "RedBlackTree created." << endl;

	for (auto x : A) {
		TreeNode<double>* node = new TreeNode<double>(x, x*3.14159);
		cout << "t.insert(" << x << ")..." << endl;
		t.insert(node);
	}

	cout << "\tdone!" << endl;
	cout << "Tree inorder-walk:" << endl; t.inorder_walk();
	cout << "Tree size = " << t.get_size() << endl;
	cout << "Tree min = " << *t.minimum() << "\nTree max = " << *t.maximum() << endl;

	cout << "Removing root..." << endl;
	cout << "Root = " << *t.get_root() << endl;
	t.remove(t.get_root());

	cout << "\tdone!" << endl;
	cout << "Tree inorder-walk:" << endl; t.inorder_walk();
	cout << "Tree size = " << t.get_size() << endl;
	cout << "Tree min = " << *t.minimum() << "\nTree max = " << *t.maximum() << endl;

	while (t.get_size()) {
		cout << "Again removing root..." << endl;
		// cout << "Root = " << *t.get_root() << endl;
		t.remove(t.get_root());
	}

	cout << "\tdone!" << endl;
	cout << "Tree inorder-walk:" << endl; t.inorder_walk();
	cout << "Tree size = " << t.get_size() << endl;
	cout << "Tree min = " << *t.minimum() << "\nTree max = " << *t.maximum() << endl;

	cout << "\n\tSuccessfully ended!\n" << endl;
}
