#include <iostream>
#include <sstream>
#include <set>
#include <vector>
#include <algorithm>
#include <utility>

#define DEBUG

using std::cout;
using std::cin;
using std::endl;

using Edge = std::pair<size_t, size_t>;
using VE = std::vector<Edge>;
using VI = std::vector<int>;
using VVI = std::vector<VI>;


#ifdef DEBUG
const VVI* weight_;

template<typename T, size_t N>
size_t size(T (&)[N]) { return N; }

template<typename T, size_t N>
void show(T (&A)[N]) {
	for (int i = 0; i < N; i++) {
		cout << A[i] << " ";
	}
	cout << endl;
}

template<typename T>
void show(std::set<T> s) {
	cout << '\t';
	for (auto& x : s) {
		cout << x << ' ';
	}
	cout << endl;
}

template<typename T>
void show(std::vector<T> v) {
	cout << '\t';
	for (size_t i = 0; i < v.size(); ++i) {
		cout << v[i] << " ";
	}
	cout << endl;
}
template<>
void show(std::vector<Edge> v) {
	for (size_t i = 0; i < v.size(); ++i) {
		cout << "\t" << v[i].first << " <-> " << v[i].second << "  (" << (*weight_)[v[i].first][v[i].second] << ")\n";
	}
}
template<typename T>
void show(std::vector<std::set<T>> v) {
	for (size_t i = 0; i < v.size(); ++i) {
		cout << "\t" << i << ": ";
		show(v[i]);
	}
}
#endif


int main() {
	std::stringstream cin;
	cin << "3 3\n0 1 3\n1 2 2\n0 2 10";
	// cin << "4 5\n0 1 5\n3 2 5\n1 2 1\n3 1 2\n0 3 1";
//	===========
	size_t n, k;
	cin >> n >> k;

	VVI weight(n, VI(n, -1));  // Let -1 be the Sentinel weight
	VE edges;
	VE mst;  // minimum spanning tree
	std::set<size_t> mst_set;
	std::vector<std::set<size_t>> AVL(n);  // Accessible vertices list for each vertex

#ifdef DEBUG
	weight_ = &weight;
#endif

	for (size_t i = 0; i < k; ++i) {
		size_t v1, v2, w;
		cin >> v1 >> v2 >> w;

		weight[v1][v2] = w;
		weight[v2][v1] = w;

		edges.push_back(Edge(v1, v2));
	}

	// Sort in non-increasing order  (yes, inverse!)
	std::sort(edges.begin(), edges.end(),
	  [&weight] (const Edge& lhs, const Edge& rhs) {
		return weight[lhs.first][lhs.second] > weight[rhs.first][rhs.second];
	  });
#ifdef DEBUG
	show(edges);
#endif


	while (!edges.empty()) {
		Edge e = edges.back();
		edges.pop_back();

		size_t u = e.first;
		size_t v = e.second;
		bool u_nin = mst_set.find(u) == mst_set.end();
		bool v_nin = mst_set.find(v) == mst_set.end();

		if (u_nin || v_nin || AVL[u].find(v) == AVL[u].end()) {
			mst.push_back(e);
			/*if (u_nin)  */mst_set.insert(u);
			/*if (v_nin)  */mst_set.insert(v);

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
#ifdef DEBUG
			cout << "dbg: AVL now:" << endl; show(AVL);
#endif
		}
	}

#ifdef DEBUG
	cout << "MST:" << endl;
	show(mst);
	cout << "MST SET:" << endl;
	show(mst_set);
	cout << "AVL:" << endl;
	show(AVL);
#endif

	size_t mst_length = 0;
	for (auto& it : mst) {
		mst_length += weight[it.first][it.second];
	}
	cout << mst_length << endl;
}
