#include <iostream>

#include "Edge.hpp"
#include "utils.hpp"
#include "main.hpp"


Edge::Edge(int x, int y, int w)
: v1(x), v2(y), weight(w), eta(1./weight)
, phe(uni_real_dist(RandomEngine)), phe_acc(0.0) {
	// cout << "Edge between " << x << " and " << y << " created" << endl;
}

std::ostream& operator<<(std::ostream &out, const Edge &edge) {
	out << "[Edge: " << edge.v1 << "->" << edge.v2 << " (" << edge.weight << ")" << "]";
	return out;
}

void Edge::update() {
	phe = (1. - rho) * phe + phe_acc;
	if (phe_minmax) constrain( phe, phe_min, phe_max );
	phe_acc = 0.0;
}
