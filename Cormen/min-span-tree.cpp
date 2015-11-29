/* Copyright (c) 2015, Lipen */
#include <iostream>     // cout, cin, endl
#include <sstream>      // stringstream
#include <set>          // set
#include <vector>       // vector
#include <algorithm>    // sort
#include <utility>      // pair

using std::cout;
using std::cin;
using std::endl;

struct Edge {
	int first, second, weight;
};

using VE = std::vector<Edge>;


int main() {
	std::stringstream cin;
	cin << "3 3\n0 1 3\n1 2 2\n0 2 10";  // ~>> 5
//	== ^ Remove if using console stdin ==
	size_t n, k;          // Number of vertices, edges
	cin >> n >> k;

	VE edges;             // Edges data
	VE mst;               // Minimum Spanning Tree
	std::vector<int> cc;  // Connected Components

	// Initialize unique cc numbers
	for (size_t i = 0; i < n; ++i) {
		cc.push_back(i);
	}

	// Read edges data
	for (size_t i = 0; i < k; ++i) {
		int v1, v2, w;
		cin >> v1 >> v2 >> w;

		edges.push_back(Edge{ v1, v2, w });
	}

	// Sort by weight
	std::sort( edges.begin(), edges.end(),
	  [] (const Edge &lhs, const Edge &rhs) {
		return lhs.weight < rhs.weight;
	  } );


	for (size_t i = 0; i < n; ++i) {
		Edge current = edges[i];
		size_t u = current.first;
		size_t v = current.second;

		if (cc[u] != cc[v]) {
			// Union connected components: change all 'x' ~> 'y'
			int x = cc[v];
			int y = cc[u];

			for (size_t i = 0; i < n; ++i) {
				if (cc[i] == x) {
					cc[i] = y;
				}
			}

			// Append mst with new edge
			mst.push_back(current);
		}
	}

	// Accumulate mst`s length
	size_t mst_length = 0;
	for (Edge& e : mst) {
		mst_length += e.weight;
	}
	// Output length of built MST
	cout << mst_length << endl;
}
