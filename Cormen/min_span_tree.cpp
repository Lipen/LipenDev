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

using Edge = std::pair<size_t, size_t>;
using VE = std::vector<Edge>;
using VI = std::vector<int>;
using VVI = std::vector<VI>;


int main() {
	std::stringstream cin;
	cin << "3 3\n0 1 3\n1 2 2\n0 2 10";
//	== ^ Remove if using console stdin ==
	size_t n, k;
	cin >> n >> k;

	VVI weight(n, VI(n, -1));  // n x n  weight matrix
	VE edges;
	VE mst;  // minimum spanning tree
	std::set<size_t> mst_set;  // vertices in mst
	std::vector<std::set<size_t>> AVL(n);  // "Accessible vertices list for each vertex"

	// Read data and fill weight matrix
	for (size_t i = 0; i < k; ++i) {
		size_t v1, v2, w;
		cin >> v1 >> v2 >> w;

		weight[v1][v2] = w;
		weight[v2][v1] = w;

		edges.push_back(Edge(v1, v2));
	}

	// Sort in non-increasing order
	std::sort(edges.begin(), edges.end(),
	  [&weight] (const Edge& lhs, const Edge& rhs) {
		return weight[lhs.first][lhs.second] > weight[rhs.first][rhs.second];
	  });


	while (!edges.empty()) {
		Edge e = edges.back();
		edges.pop_back();

		size_t u = e.first;
		size_t v = e.second;
		bool u_nin = mst_set.find(u) == mst_set.end();  // u not in mst
		bool v_nin = mst_set.find(v) == mst_set.end();

		if (u_nin || v_nin || AVL[u].find(v) == AVL[u].end()) {
			mst.push_back(e);
			if (u_nin)  mst_set.insert(u);
			if (v_nin)  mst_set.insert(v);

			// update u`s component
			AVL[u].insert(v);
			for (auto& y : AVL[v]) {
				AVL[u].insert(y);
			}
			for (auto& x : AVL[u]) {
				for (auto& y : AVL[v]) {
					AVL[x].insert(y);
				}
			}

			// update v`s component
			AVL[v].insert(u);
			for (auto& y : AVL[u]) {
				AVL[v].insert(y);
			}
			for (auto& x : AVL[v]) {
				for (auto& y : AVL[u]) {
					AVL[x].insert(y);
				}
			}
		}
	}


	size_t mst_length = 0;
	for (auto& e : mst) {
		mst_length += weight[e.first][e.second];
	}
	// Output length of built MST
	cout << mst_length << endl;
}
