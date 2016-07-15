/* Copyright (C) 2016, Lipen */
#include <iostream>  // cout, endl
#include <string>    // string
#include <vector>    // vector
#include <tuple>     // tie
#include <utility>   // pair

using std::cout;
using std::endl;
using std::string;

const int ALPHABET_SIZE = 26;


inline int char_to_index(char c) {
	return c - 'a';
}
inline char index_to_char(int i) {
	return 'a' + i;
}


class Trie {
	struct Node {
		Node** children = new Node*[ALPHABET_SIZE]();
		bool is_end;

		explicit Node(bool is_end = false): is_end(is_end) {}

		~Node() {
			for (int i = 0; i < ALPHABET_SIZE; ++i) {
				delete children[i];
			}
			delete [] children;
			children = nullptr;
		}

		void print() {
			print("", "", true);
		}

		void print(string name, string prefix, bool is_tail) {
			if (name.size() > 0) {
				cout << prefix << (is_tail ? "└── " : "├── ") << name << endl;
			}
			else {
				cout << "@root" << endl;
			}

			std::vector<std::pair<Node*, int>> nodes_to_print;

			for (int i = 0; i < ALPHABET_SIZE; ++i) {
				if (children[i] != nullptr) {
					nodes_to_print.emplace_back(children[i], i);
				}
			}

			if (nodes_to_print.size() > 0) {
				for (size_t i = 0; i < nodes_to_print.size()-1; ++i) {
					Node* node;
					int index;
					std::tie(node, index) = nodes_to_print[i];

					// cout << node << "->print(..., false)" << endl;
					node->print(name + index_to_char(index), prefix + (is_tail ? "    " : "│   "), false);
				}

				Node* node;
				int index;
				std::tie(node, index) = nodes_to_print[nodes_to_print.size()-1];

				// cout << node << "->print(..., true)" << endl;
				node->print(name + index_to_char(index), prefix + (is_tail ? "    " : "│   "), true);
			}
		}
	};

	Node* root;

 public:
	Trie(): root(new Node) {}

	~Trie() {
		delete root;
	}


	void insert(string word) {
		Node* current = root;

		for (size_t level = 0; level < word.size(); ++level) {
			int index = char_to_index(word[level]);

			if (current->children[index] == nullptr) {
				current->children[index] = new Node;
			}

			current = current->children[index];
		}

		current->is_end = true;
	}

	bool search(string word) {
		Node* current = root;

		for (size_t level = 0; level < word.size(); ++level) {
			int index = char_to_index(word[level]);

			if (current->children[index] == nullptr) {
				return false;
			}

			current = current->children[index];
		}

		return current != nullptr && current->is_end;
	}

	void remove(string word) {
		Node* current = root;

		for (size_t level = 0; level < word.size(); ++level) {
			int index = char_to_index(word[level]);

			if (current->children[index] == nullptr) {
				cout << "No such word in trie: '" << word << "'" << endl;
				return;
			}

			current = current->children[index];
		}

		current->is_end = false;
	}

	void print() {
		root->print();
	}
};


int main() {
	std::vector<string> text {"the", "a", "there", "answer", "any", "by", "bye", "their"};

	// Init
	Trie trie;

	// Insert
	cout << ">\tInserting..." << endl;
	for (auto&& word : text) {
		cout << " -\tInserting '" << word << "'" << endl;
		trie.insert(word);
		cout << "\tWord '" << word << "' inserted" << endl;
	}

	// Search
	cout << "\n>\tSearching..." << endl;
	cout << "\t'the' in trie: " << trie.search("the") << endl;
	cout << "\t'answer' in trie: " << trie.search("answer") << endl;
	cout << "\t'hello' in trie: " << trie.search("hello") << endl;
	cout << "\t'any' in trie: " << trie.search("any") << endl;

	// Print
	cout << "\n>\tPrinting..." << endl;
	trie.print();

	// Delete
	cout << "\n>\tRemoving 'any'..." << endl;
	trie.remove("any");
	cout << "\t'any' in trie: " << trie.search("any") << endl;

	cout << ">\tRemoving 'by'..." << endl;
	trie.remove("by");
	cout << "\t'by' in trie: " << trie.search("by") << endl;
	cout << "\t'bye' in trie: " << trie.search("bye") << endl;
}
