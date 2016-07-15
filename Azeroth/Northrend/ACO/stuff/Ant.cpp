#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <utility>

#include "Ant.hpp"
#include "Edge.hpp"
#include "Node.hpp"
#include "CompleteGraph.hpp"
#include "utils.hpp"
#include "main.hpp"

using std::cout;
using std::endl;


// TODO: Fabric Pattern
Ant::Ant(CompleteGraph* graph, int i)
: g(graph), start_town(g->get_node(i)), path_len(0), number(i) {
	// cout << "Ant at " << i << " created" << endl;

	path.reserve(g->n);

	for (Node* node : g->nodes) {
		if (node != start_town) {
			J_.push_back(node);
		}
	}
}


double Ant::get_probability_from_to(Node* from, Node* to) {
	Edge* e = g->get_edge(from->number, to->number);
	return pow(e->phe, alpha) * pow(e->eta, beta);
}


void Ant::go() {
	// cout << "Ant(" << number << ")::go..." << endl;
	Node* cur_town = start_town;
	VN J = J_;

	path.clear();
	path.push_back(start_town);
	// cout << "Ant::go: path cleared" << endl;

	while ( !J.empty() ) {
		// cout << "DBG J = " << J << endl;
		VPDN probs;          // Only (phe*eta) products
		VPDN probabilities;  // Actual probabilities (accumulated)
		double probs_sum = 0.0;
		double probabilities_acc = 0.0;

		// Calc probs:
		for (Node* node : g->nodes) {
			// if (node in J)
			if ( std::find(J.begin(), J.end(), node) != J.end() ) {
				double p = get_probability_from_to(cur_town, node);
				probs.push_back(std::make_pair(p, node));
				probs_sum += p;
			}
		}
		// cout << "Ant::go: probs calculated" << endl;
		// cout << probs << endl;

		// Calc probabilities:
		for (std::pair<double, Node*> probs_pair : probs) {
			double probs_pair_p = probs_pair.first;
			Node* probs_pair_node = probs_pair.second;
			probabilities_acc += probs_pair_p / probs_sum;
			probabilities.push_back(std::make_pair(probabilities_acc, probs_pair_node));
		}
		// cout << "Ant::go: probabilities calculated" << endl;
		probabilities.back().first = 1.000001;
		// cout << probabilities << endl;

		// Choose random way to go:
		double r = random(RandomEngine);
		Node* choice;

		for (std::pair<double, Node*> probs_pair : probabilities) {
			double p = probs_pair.first;
			Node* node = probs_pair.second;

			if (p >= r) {
				choice = node;
				break;
			}
		}
		// cout << "Ant::go: random(" << r << ") chosen: " << *choice << endl;
		// ASSERT choice != nullptr

		// Move to <choice>
		// cout << "Ant::go: Moving from: " << *cur_town << endl;
		Edge* e = g->get_edge(cur_town, choice);
		path.push_back(choice);
		path_len += e->weight;
		e->phe_acc += Q / path_len;
		cur_town = choice;
		// J.resize( std::remove(J.begin(), J.end(), choice) - J.begin() );
		J.erase( std::remove(J.begin(), J.end(), choice), J.end() );
		// cout << "Ant::go: moved" << endl;
	}

	// cout << "Ant::go: while loop ended" << endl;
	Edge* e = g->get_edge(cur_town, start_town);
	path.push_back(start_town);
	path_len += e->weight;
	e->phe_acc += Q / path_len;
	// cout << "Ant::go: end" << endl;
}


void Ant::compare_opt(VN &T_opt, int &L_opt) {
	if (path_len < L_opt || T_opt.size() == 0) {
		cout << "Overwriting L_opt = " << L_opt << " with " << path_len << endl;
		T_opt = path;
		L_opt = path_len;
	}
}
