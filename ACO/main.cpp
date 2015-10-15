/* Copyright (c) 2015, Lipen */
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cmath>
#include <ctime>
#include <random>
#include <algorithm>
#include <utility>

#define pb push_back

using std::cout;
using std::cin;
using std::endl;
using std::string;

std::default_random_engine RandomEngine;
std::uniform_real_distribution<double> random;  // 0 .. 1
std::uniform_real_distribution<double> uni_real_dist(5.0, 10.0);

struct Node;
struct Edge;
struct Ant;

typedef std::vector<int> VI;
typedef std::vector<double> VD;
typedef std::vector<Edge*> VE;
typedef std::vector<Node*> VN;
typedef std::vector<Ant*> VA;
typedef std::vector<std::pair<double, Node*>> VPDN;


template<typename T>
void print_vector(std::vector<T> v) {
	for (T x : v) cout << " " << x;
	cout << endl;
}

template<typename T>
std::ostream& operator<< (std::ostream &out, std::vector<T> v) {
	out << "[";

	for (size_t i = 0; i < v.size(); ++i) {
		out << v[i] << (( i < v.size()-1 ) ? ", " : "]");
	}

	return out;
}
template<typename T>
std::ostream& operator<< (std::ostream &out, std::vector<T*> v) {
	out << "[";

	for (size_t i = 0; i < v.size(); ++i) {
		out << *v[i] << (( i < v.size()-1 ) ? ", " : "]");
	}

	return out;
}

template<typename T>
T constrain(T x, T x_min, T x_max) {
	if (x < x_min) return x_min;
	if (x > x_max) return x_max;
	return x;
}


size_t t_max = 1000;
double alpha = 0.3;
double beta = 1. - alpha;
double Q = 10;
double rho = 0.1;
double phe_min = 1.;
double phe_max = 20.;

std::array<std::array<int, 15>, 15> W {{
//1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
{0,  29, 82, 46, 68, 52, 72, 42, 51, 55, 29, 74, 23, 72, 46},  // 1
{29,  0, 55, 46, 42, 43, 43, 23, 23, 31, 41, 51, 11, 52, 21},  // 2
{82, 55,  0, 68, 46, 55, 23, 43, 41, 29, 79, 21, 64, 31, 51},  // 3
{46, 46, 68,  0, 82, 15, 72, 31, 62, 42, 21, 51, 51, 43, 64},  // 4
{68, 42, 46, 82,  0, 74, 23, 52, 21, 46, 82, 58, 46, 65, 23},  // 5
{52, 43, 55, 15, 74,  0, 61, 23, 55, 31, 33, 37, 51, 29, 59},  // 6
{72, 43, 23, 72, 23, 61,  0, 42, 23, 31, 77, 37, 51, 46, 33},  // 7
{42, 23, 43, 31, 52, 23, 42,  0, 33, 15, 37, 33, 33, 31, 37},  // 8
{51, 23, 41, 62, 21, 55, 23, 33,  0, 29, 62, 46, 29, 51, 11},  // 9
{55, 31, 29, 42, 46, 31, 31, 15, 29,  0, 51, 21, 41, 23, 37},  // 10
{29, 41, 79, 21, 82, 33, 77, 37, 62, 51,  0, 65, 42, 59, 61},  // 11
{74, 51, 21, 51, 58, 37, 37, 33, 46, 21, 65,  0, 61, 11, 55},  // 12
{23, 11, 64, 51, 46, 51, 51, 33, 29, 41, 42, 61,  0, 62, 23},  // 13
{72, 52, 31, 43, 65, 29, 46, 31, 51, 23, 59, 11, 62,  0, 59},  // 14
{46, 21, 51, 64, 23, 59, 33, 37, 11, 37, 61, 55, 23, 59,  0}}};// 15
// std::array<std::array<int, 5>, 5> W {{{5, 8, 2, 4, 7},
// 									  {8, 3, 1, 6, 2},
// 									  {2, 1, 9, 5, 8},
// 									  {4, 6, 5, 3, 1},
// 									  {7, 2, 8, 1, 4}}};


struct Node {
	int number;

	explicit Node(int n) : number(n) {
		cout << "Node " << n << " created" << endl;
	}

	friend std::ostream& operator<<(std::ostream &out, const Node &node) {
		out << "[Node: " << node.number << "]";
		return out;
	}
};
template<>
std::ostream& operator<< (std::ostream &out, VPDN v) {
	out << "[";

	for (size_t i = 0; i < v.size(); ++i) {
		out << "{" << v[i].first << ":" << *v[i].second << "}" << (( i < v.size()-1 ) ? ", " : "]");
	}

	return out;
}


struct Edge {
	int v1, v2;
	int weight;
	double eta;
	double phe;
	double phe_acc;

	Edge(int x, int y, int w)
		: v1(x), v2(y), weight(w), eta(1./weight), phe(uni_real_dist(RandomEngine)), phe_acc(0.0) {
			cout << "Edge between " << x << " and " << y << " created" << endl;
		}

	friend std::ostream& operator<<(std::ostream &out, const Edge &edge) {
		out << "[Edge: " << edge.v1 << "->" << edge.v2 << " (" << edge.weight << ")" << "]";
		return out;
	}

	void update() {
		phe = constrain( (1. - rho) * phe + phe_acc, phe_min, phe_max );
		phe_acc = 0.0;
	}
};


struct CompleteGraph {
	VN nodes;
	VE edges;
	size_t n;

	template<size_t N>
	explicit CompleteGraph(std::array<std::array<int, N>, N> W) : n(N) {
		for (size_t i = 0; i < n; ++i) {
			nodes.push_back(new Node(i+1));
		}
		for (size_t i = 0; i < n-1; ++i) {
			for (size_t j = i + 1; j < n; ++j) {
				edges.push_back(new Edge(i+1, j+1, W[i][j]));
			}
		}
	}

	~CompleteGraph() {}

	Node* get_node(int i) {
		// for (Node* node : nodes) {
		// 	if (node->number == i) {
		// 		return node;
		// 	}
		// }
		// return nullptr;
		return nodes[i - 1];
	}

	Edge* get_edge(int i, int j) {
		for (Edge* edge : edges) {
			if ((edge->v1 == i && edge->v2 == j) ||
				(edge->v1 == j && edge->v2 == i)) {
				return edge;
			}
		}
		return nullptr;
	}

	Edge* get_edge(Node* x, Node* y) {
		for (Edge* edge : edges) {
			if ((edge->v1 == x->number && edge->v2 == y->number) ||
				(edge->v1 == y->number && edge->v2 == x->number)) {
				return edge;
			}
		}
		return nullptr;
	}

	void update() {
		for (Edge* edge : edges) {
			edge->update();
		}
	}
};


struct Ant {
	CompleteGraph* g;
	Node* start_town;
	VN path;
	VN J_;  // Nodes to visit. Copy into J every tick (J = J_)
	int path_len = 0;
	int number;

	// TODO: Fabric Pattern
	Ant(CompleteGraph* graph, int i) : g(graph), start_town(g->get_node(i)), number(i) {
		cout << "Ant at " << i << " created" << endl;

		path.reserve(g->n);

		for (Node* node : g->nodes) {
			if (node != start_town) {
				J_.push_back(node);
			}
		}
	}

	double get_probability_from_to(Node* from, Node* to) {
		Edge* e = g->get_edge(from->number, to->number);
		return pow(e->phe, alpha) * pow(e->eta, beta);
	}

	void go() {
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

	void compare_opt(VN &T_opt, int &L_opt) {
		if (path_len < L_opt || T_opt.size() == 0) {
			cout << "Overwriting L_opt=" << L_opt << " with " << path_len << endl;
			T_opt = path;
			L_opt = path_len;
		}
	}
};


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
	for (size_t t = 0; t < t_max; ++t) {
		if ( t % 10 == 0 ) cout << "t = " << t << "..." << endl;

		for (Ant* ant : ants) {
			ant->go();
		}

		for (Ant* ant : ants) {
			ant->compare_opt(T_opt, L_opt);
		}

		g->update();
	}

	cout << "Optimal path of length " << L_opt << ":" << endl;
	cout << "\t" << T_opt << endl;
}
