#ifndef ANT_HPP
#define ANT_HPP

#include "utils.hpp"
// #include "main.hpp"
#include "Node.hpp"
#include "CompleteGraph.hpp"


struct Ant {
	CompleteGraph* g;
	Node* start_town;
	VN path;
	VN J_;  // Nodes to visit. Copy into J every tick (J = J_)
	int path_len;
	int number;

	Ant(CompleteGraph*, int);

	double get_probability_from_to(Node*, Node*);
	void go();
	void compare_opt(VN&, int&);
};

#endif // ANT_HPP
