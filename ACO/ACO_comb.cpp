/* Copyright (c) 2015, Lipen */
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <iterator>
#include <vector>
#include <array>
#include <cmath>
#include <chrono>
#include <random>
#include <numeric>
#include <algorithm>

using std::cout;
using std::cin;
using std::endl;
using std::string;

using weight_t = long long signed int;


const size_t t_max = 100000;
const double alpha = 0.4;
const double beta = 1. - alpha; 	// Greedyness
const double Q = 5000;
const double q0 = 0.2; 			// Probabalisticness
const double rho = 0.03;
const bool phe_minmax = true;
const double phe_min = 1;
const double phe_max = 999;
volatile weight_t L_opt = 0;
const bool phe_show_map = true;
const string dataset_name = "dataset_xqf131";
const string output_filename = "opt";

/*	dataset_fri26
 *	http://people.sc.fsu.edu/~jburkardt/datasets/tsp/fri26.tsp
 *	Optimal path: 937
 *	Found: 937 at 1000+
 *	[9->10->14->15->12->13->11->16->19->20->18->17->21->22->26->23->24->25->1->2->3->4->6->5->7->8->9]
 *	Algorithm parameters:
 *	alpha = 0.2 	beta = 0.8
 *	Q = 1000 		q0 = 0.2
 *	rho = 0.02 		phe_minmax = (1, 999)
 */

/*	dataset_att48
 *	http://people.sc.fsu.edu/~jburkardt/datasets/tsp/att48.tsp
 *	Optimal path: 10628
 *	Found: 35987 so far
 *	[35->45->10->24->42->5->48->39->32->21->47->11->23->14->25->13->36->18->7->28->6->37->19->27->43->30->17->44->31->38->46->33->20->12->15->40->9->1->8->22->16->3->34->41->29->2->26->4->35]
 *	alpha = 0.2 	beta = 0.8
 *	Q = 30000 		q0 = 0.2
 *	rho = 0.02 		phe_minmax = (1, 9999)
 */

/*	dataset_dantzig42
 *	http://people.sc.fsu.edu/~jburkardt/datasets/tsp/dantzig42.tsp
 *	Optimal path: unknown
 *	Found: 700 at 1000+
 *	[2->1->3->4->5->6->7->8->9->10->11->12->13->14->15->16->17->18->19->20->21->22->23->24->25->26->27->28->29->30->31->32->33->34->35->36->37->38->39->40->41->42->2]
 *	Algorithm parameters:
 *	alpha = 0.9 	beta = 0.1
 *	Q = 1000 		q0 = 0.2
 *	rho = 0.02 		phe_minmax = (1, 999)
 */

/*	dataset_gr17
 *	http://people.sc.fsu.edu/~jburkardt/datasets/tsp/gr17.tsp
 *	Optimal path: 2085
 *	Found: 2085 at 1000-
 *	[10->11->3->15->14->17->6->8->7->13->4->1->16->12->9->5->2->10]
 *	Algorithm parameters:
 *	alpha = 0.5 	beta = 0.5
 *	Q = 1000 		q0 = 0.2
 *	rho = 0.01 		phe_minmax = (1, 999)
 */

/*	dataset_p01		(n=15)
 *	http://people.sc.fsu.edu/~jburkardt/datasets/tsp/p01.tsp
 *	Optimal path: 291
 *	Found: 291 at 1000-
 *	[1->13->2->15->9->5->7->3->12->14->10->8->6->4->11->1]
 *	Algorithm parameters:
 *	alpha = 0.5 	beta = 0.5
 *	Q = 1000 		q0 = 0.2
 *	rho = 0.01 		phe_minmax = (1, 999)
 */

/*	dataset_xqf131
 *	http://www.math.uwaterloo.ca/tsp/vlsi/xqf131.tsp
 *	Optimal path: 564
 *	Found:

 */

/*	dataset_bays29
 *	http://www.iwr.uni-heidelberg.de/groups/comopt/software/TSPLIB95/tsp/ ...
 *	Optimal path: 2020 (http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/STSP.html)
 *	Found: 2020 at 150000+
 *	[1->28->6->12->9->5->26->29->3->2->20->10->4->15->18->17->14->22->11->19->25->7->23->27->8->24->16->13->21->1]
 *	Algorithm parameters:
 *	alpha = 0.4 	beta = 0.6
 *	Q = 4000 		q0 = 0.2
 *	rho = 0.02 		phe_minmax = (1, 999)
 *	t_max = 200000
 */

std::default_random_engine RandomEngine;
std::uniform_real_distribution<double> random;  // 0 .. 1
std::uniform_real_distribution<double> uni_real_dist(5, 10);


struct Node;
struct Edge;
struct CompleteGraph;
struct Ant;

typedef std::vector<Edge*> VE;
typedef std::vector<Node*> VN;
typedef std::vector<Ant*> VA;
typedef std::pair<double, Node*> PDN;
typedef std::vector<PDN> VPDN;
typedef std::vector<std::vector<weight_t>> VVW;


// Display T vector
template<typename T>
std::ostream& operator<< ( std::ostream &out, std::vector<T> v ) {
	out << '[';

	for (size_t i = 0; i < v.size(); ++i) {
		out << v[i];
		if ( i < v.size()-1 ) out << "->";
	}

	return out << ']';
}

// Display T* vector
template<typename T>
std::ostream& operator<< ( std::ostream &out, std::vector<T*> v ) {
	out << '[';

	for (size_t i = 0; i < v.size(); ++i) {
		out << *v[i];
		if ( i < v.size()-1 ) out << "->";
	}

	return out << ']';
}

// Persuade compiler, that Node can be displayed too (forward declaration):
std::ostream& operator<< ( std::ostream&, const Node& );
// Display vector of pairs
std::ostream& operator<< ( std::ostream &out, VPDN v ) {
	out << '[';

	for (size_t i = 0; i < v.size(); ++i) {
		out << "{" << v[i].first << ":" << *v[i].second << "}";
		if ( i < v.size()-1 ) out << ", ";
	}

	return out << ']';
}

// Constrain 'x' inside [x_min, x_max]
template<typename T>
void constrain( T* x, const T &x_min, const T &x_max ) {
	if (*x < x_min) *x = x_min;
	if (*x > x_max) *x = x_max;
}

// Sum pairs (for std::partial_sum)
constexpr PDN pair_sum( const PDN &lhs, const PDN &rhs ) {
	return PDN(lhs.first + rhs.first, rhs.second);
}

VVW read_dataset(string filename) {
	std::ifstream fi(filename);

	if (fi) {
		VVW dataset;

		for (string line; std::getline( fi, line ); ) {
			std::vector<weight_t> line_values;
			std::istringstream iss(line);
			std::copy(
				std::istream_iterator<weight_t>(iss),
				std::istream_iterator<weight_t>(),
				std::back_inserter(line_values)
			);
			dataset.push_back(line_values);
		}

		fi.close();
		return dataset;
	}
	else {
		throw "Couldn't open " + filename;
	}
}


struct Node {
	int number;

	explicit Node(int n) : number(n) {}

	// Printable interface
	friend std::ostream& operator<<( std::ostream &out, const Node &node ) {
		return out << node.number;
	}
};


struct Edge {
	int v1, v2;
	weight_t weight;
	double eta;
	double phe;
	double phe_acc;


	Edge( int x, int y, weight_t w )
	: v1(x), v2(y), weight(w), eta(std::abs(1./weight))
	, phe(uni_real_dist(RandomEngine)), phe_acc(0.0) {}


	void update() {
		phe = (1. - rho) * phe + phe_acc;
		if (phe_minmax) constrain( &phe, phe_min, phe_max );
		phe_acc = 0.0;
	}

	// Printable interface
	friend std::ostream& operator<< ( std::ostream &out, const Edge &edge ) {
		return out << "[Edge: " << edge.v1 << "->" << edge.v2 << " (" << edge.weight << ")]";
	}
};


struct CompleteGraph {
	VN nodes;
	VE edges;
	size_t n;
	std::vector<VE> edges_map;


	explicit CompleteGraph(const VVW &W) : n(W.size()), edges_map( n, VE(n) ) {
		for (size_t i = 0; i < n; ++i) {
			nodes.push_back(new Node(i+1));

			for (size_t j = i + 1; j < n; ++j) {
				Edge* e = new Edge( i+1, j+1, W[i][j] );
				edges.push_back(e);
				edges_map[i][j] = e;
				edges_map[j][i] = e;
			}
		}
	}


	Node* get_node(int i) const {
		return nodes[i - 1];
	}

	Edge* get_edge(int i, int j) const {
		return edges_map[i-1][j-1];
	}

	Edge* get_edge(Node* x, Node* y) const {
		return edges_map[x->number-1][y->number-1];
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
	VN J_;  // Nodes to visit. Copy into J every tick (J = J_)
	// TODO: Replace "copy J_ into J" with something faster
	VN path;
	weight_t path_len;


	Ant( CompleteGraph* graph, int i ) : g(graph), start_town(g->get_node(i)), path_len(0) {
		J_.reserve(g->n);
		path.reserve(g->n);

		for (Node* node : g->nodes) {
			if (node != start_town) {
				J_.push_back(node);
			}
		}
	}


	double get_probability_from_to( Node* from, Node* to ) {
		Edge* e = g->get_edge( from, to );
		return pow( e->phe, alpha ) * pow( e->eta, beta );
	}


	void go() {
		Node* cur_town = start_town;
		VN J = J_;

		path.clear();
		path.push_back(start_town);
		path_len = 0;

		while ( !J.empty() ) {
			Node* choice = nullptr;
			double q = random(RandomEngine);

			if (q < q0) {
				// Calc probabilities:
				VPDN probabilities;
				probabilities.reserve(J.size());
				for (Node* node : J) {
					double p = get_probability_from_to( cur_town, node );
					probabilities.push_back(PDN( p, node ));
				}
				std::partial_sum(probabilities.begin(), probabilities.end(), probabilities.begin(), pair_sum);
				probabilities.back().first = 1.000001;  // round up last..

				// Choose random way to go:
				double r = random(RandomEngine);

				for (auto probs_pair : probabilities) {
					double p = probs_pair.first;
					Node* node = probs_pair.second;

					if (r <= p) {
						choice = node;
						break;
					}
				}
			}
			else {
				double eta_max = -1;  // Assume that eta > 0

				for (Node* node : J) {
					double eta_tmp = g->get_edge( cur_town, node )->eta;
					if (eta_tmp > eta_max) {
						eta_max = eta_tmp;
						choice = node;
					}
				}
			}

			// Move to <choice>
			Edge* e = g->get_edge( cur_town, choice );
			path.push_back(choice);
			path_len += e->weight;
			cur_town = choice;
			J.erase( std::remove(J.begin(), J.end(), choice), J.end() );
		}

		Edge* e = g->get_edge( cur_town, start_town );
		path.push_back(start_town);
		path_len += e->weight;
	}


	void compare_opt(VN &T_opt) {
		if (path_len < L_opt || L_opt == 0) {
			cout << "Overwriting L_opt = " << L_opt << " with " << path_len << ":" << endl;
			cout << '\t' << path << endl;

			T_opt = path;
			L_opt = path_len;
		}
	}

	// Compare ants (for std::min_element)
	static struct {
		constexpr bool operator() ( Ant* const &lhs, Ant* const &rhs ) const {
			return lhs->path_len < rhs->path_len;
		}
	} cmp;

	// Printable interface
	friend std::ostream& operator<< ( std::ostream &out, const Ant &ant ) {
		return out << "[Ant: " << *ant.start_town << "]";
	}
};


int main() {
	VA ants;
	VN T_opt;
	VVW dataset = read_dataset(dataset_name + ".txt");
		cout << "Dataset parsed" << endl;
	CompleteGraph g(dataset);
		cout << "Graph created" << endl;


	// Initially placing ants:
	for ( size_t i = 1; i <= g.n; ++i ) {
		ants.push_back(new Ant( &g, i ));
	}
	cout << "Ants created" << endl;


	// Print algorithm parameters:
	cout << "\talpha = " << alpha << endl;
	cout << "\tbeta  = " << beta << endl;
	cout << "\tQ     = " << Q << endl;
	cout << "\tq0    = " << q0 << endl;
	cout << "\trho   = " << rho << endl;
	if (phe_minmax)
		cout << "\tphe_minmax = (" << phe_min << ", " << phe_max << ")" <<endl;
	cout << "\tt_max = " << t_max << endl;


	// Main loop:
	cout << "Starting main loop..." << endl;
	const auto time_start = std::chrono::steady_clock::now();
	auto time_ = time_start;

	for ( size_t t = 1; t <= t_max; ++t ) {
		// Build ants' solutions
		for ( Ant* ant : ants ) {
			ant->go();
		}

		// Find local-best ("lb") ant
		Ant* lb_ant = *std::min_element(ants.begin(), ants.end(), Ant::cmp);
		weight_t lb_path_len = lb_ant->path_len;

		// Compare local-best ("lb") solution with global-best
		lb_ant->compare_opt(T_opt);

		// Update pheromone only on local-best ("lb") path
		auto lb_path = lb_ant->path;
		for (size_t i = 0; i < lb_path.size()-1; ++i) {  // exclude last
			Edge* e = g.get_edge( lb_path[i], lb_path[i+1] );
			e->phe += std::abs(Q / lb_path_len);
		}

		// Update edges
		g.update();

		// Some debug inforamation
		if ( t % (t_max / 20) == 0 ) {
			const auto time_tmp = std::chrono::steady_clock::now();;
			cout << "t = " << t << " done... \ttook " << std::chrono::duration_cast<std::chrono::milliseconds>(time_tmp - time_).count()/1000. << " s" << endl;
			time_ = time_tmp;
		}
	}

	const auto time_end = std::chrono::steady_clock::now();
	cout << "Main loop done within " << std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start).count()/1000. << " s" << endl;


	// Print pheromon map if neccessary
	if (phe_show_map) {
		cout << endl << "Pheromon map:" << endl;
		cout << "***";
		for (size_t i = 1; i <= g.n; ++i) {
			cout << "| " << std::setw(2) << i;
		}
		cout << endl;

		for (size_t i = 0; i < g.n; ++i) {
			cout << std::setw(3) << (i+1) << "|";

			for (size_t j = 0; j < g.n; ++j) {
				double p = 0.0;
				if (i != j)
					p = g.get_edge(i+1, j+1)->phe;

				cout << std::setw(3) << std::fixed << std::setprecision(0) << p;
				if (j < g.n-1)
					cout << "|";
				else
					cout << endl;
			}
		}
	}
	else {
		cout << endl << "Pheromon map omitted" << endl;
	}

	// Print algorithm parameters again:
	cout << endl << " *\tAlgorithm parameters:" << endl;
	cout << " *\talpha = " << std::fixed << std::setprecision(1) << alpha << " \tbeta = " << beta << endl << " *\tQ = " << std::setprecision(0) << Q << " \t\tq0 = " << std::setprecision(1) << q0 << endl << " *\trho = " << std::setprecision(2) << rho;
	if (phe_minmax)
		cout << " \t\tphe_minmax = (" << std::setprecision(0) << phe_min << ", " << phe_max << ")";
	cout << endl << " *\tt_max = " << t_max << endl;

	cout << "\nOptimal path of length " << L_opt << ":\n *\t" << T_opt << endl;

	// Write L_opt and T_opt to file
	std::ofstream fo(output_filename);
	if (fo) {
		cout << "\nWriting to file... " << std::flush;
		fo << dataset_name << endl;
		fo << L_opt << endl;
		for (size_t i = 0; i <= g.n; ++i) {
			fo << T_opt[i]->number;
			if (i < g.n) fo << " ";
		}
		fo << endl;

		fo.close();
		cout << "done." << endl;
	}
	else {
		cout << "\nCouldn`t open '" << output_filename << "' to write" << endl;
	}
}
