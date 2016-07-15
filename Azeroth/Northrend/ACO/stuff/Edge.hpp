#ifndef EDGE_HPP
#define EDGE_HPP

// #include "utils.hpp"


struct Edge {
	int v1, v2;
	int weight;
	double eta;
	double phe;
	double phe_acc;

	Edge(int, int, int);

	friend std::ostream& operator<<(std::ostream&, Edge const&);

	void update();
};

#endif // EDGE_HPP
