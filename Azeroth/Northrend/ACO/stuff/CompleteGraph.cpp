#include <vector>
#include <array>

#include "CompleteGraph.hpp"
// #include "main.hpp"
#include "Edge.hpp"
#include "Node.hpp"


template<size_t N>
CompleteGraph::CompleteGraph(std::array<std::array<int, N>, N> W) : n(N) {
	for (size_t i = 0; i < n; ++i) {
		nodes.push_back(new Node(i+1));
	}

	for (size_t i = 0; i < n-1; ++i) {
		for (size_t j = i + 1; j < n; ++j) {
			edges.push_back(new Edge(i+1, j+1, W[i][j]));
		}
	}
}


Node* CompleteGraph::get_node(int i) {
	return nodes[i - 1];
}

Edge* CompleteGraph::get_edge(int i, int j) {
	for (Edge* edge : edges) {
		if ((edge->v1 == i && edge->v2 == j) ||
			(edge->v1 == j && edge->v2 == i)) {
			return edge;
		}
	}
	return nullptr;
}

Edge* CompleteGraph::get_edge(Node* x, Node* y) {
	for (Edge* edge : edges) {
		if ((edge->v1 == x->number && edge->v2 == y->number) ||
			(edge->v1 == y->number && edge->v2 == x->number)) {
			return edge;
		}
	}
	return nullptr;
}


void CompleteGraph::update() {
	for (Edge* edge : edges) {
		edge->update();
	}
}
