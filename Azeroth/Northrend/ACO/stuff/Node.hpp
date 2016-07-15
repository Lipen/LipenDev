#ifndef NODE_HPP
#define NODE_HPP

// #include "utils.hpp"


struct Node {
	int number;

	explicit Node(int);

	friend std::ostream& operator<<(std::ostream&, Node const&);
};

#endif // NODE_HPP
