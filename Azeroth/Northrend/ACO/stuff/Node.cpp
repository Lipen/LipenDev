#include <iostream>

// #include "utils.hpp"
// #include "main.hpp"
#include "Node.hpp"


Node::Node(int n) : number(n) {
	// cout << "Node " << n << " created" << endl;
}

std::ostream& operator<<(std::ostream &out, const Node &node) {
	out << "[Node: " << node.number << "]";
	return out;
}
