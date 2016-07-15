/* Copyright (c) 2015, Lipen */
#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <ctime>
#include <random>
#include <algorithm>
#include <utility>

#include "main.hpp"
#include "Ant.hpp"
#include "Edge.hpp"
#include "Node.hpp"
#include "CompleteGraph.hpp"
#include "utils.hpp"

using std::cout;
using std::endl;


int main() {
	CompleteGraph* g = new CompleteGraph(W);
	cout << "Graph created" << endl;
	VA ants;
	VN T_opt;
	int L_opt;

	// Initially placing ants:
	for (size_t i = 0; i < g->n; ++i) {
		Ant* a = new Ant(g, i+1);
		ants.push_back(a);
	}
	cout << "Ants created" << endl;

	// Main loop:
	cout << "Starting main loop..." << endl;
	for (size_t t = 1; t <= t_max; ++t) {
		for (Ant* ant : ants) {
			ant->go();
		}

		for (Ant* ant : ants) {
			ant->compare_opt(T_opt, L_opt);
		}

		g->update();

		if ( t % (t_max / 8) == 0 ) cout << "t = " << t << " done.." << endl;
	}
	cout << "Main loop done" << endl;

	cout << "Optimal path of length " << L_opt << ":" << endl;
	cout << "\t" << T_opt << endl;
}
