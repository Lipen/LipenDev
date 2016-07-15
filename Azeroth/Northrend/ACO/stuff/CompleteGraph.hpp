#ifndef COMPLETE_GRAPH_HPP
#define COMPLETE_GRAPH_HPP

#include "utils.hpp"
// #include "main.hpp"
// #include "Node.hpp"
// #include "Edge.hpp"


struct CompleteGraph {
	VN nodes;
	VE edges;
	size_t n;

	template<size_t N>
	explicit CompleteGraph(std::array<std::array<int, N>, N>);

	Node* get_node(int);
	Edge* get_edge(int, int);
	Edge* get_edge(Node*, Node*);

	void update();
};

#endif // COMPLETE_GRAPH_HPP
