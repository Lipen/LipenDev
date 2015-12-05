#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
// TAGS: knapsack problem, dp, dynamic programming
// O(nW) ~ O(n*2^(logW))

using std::cout;
using std::cin;
using std::endl;

using VI = std::vector<int>;
using VVI = std::vector<VI>;

// DEBUG
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
void show(std::vector<T> v) {
	cout << '\t';
	for (size_t i = 0; i < v.size(); ++i) {
		cout << v[i] << " ";
	}
	cout << endl;
}
template<typename T>
void show(std::vector<std::vector<T>> v) {
	cout << "vector<vector<T>> [" << v.size() << "] x [" << ((v.size()) ? v[0].size() : 0) << "] {" << endl;
	for (size_t i = 0; i < v.size(); ++i) {
		show(v[i]);
	}
	cout << "}" << endl;
}
// DEBUG END


int knapsack_with_repetitions(int W, const VI &weight, const VI &cost) {
	size_t n = weight.size();  // number of items
	VI D(W+1, 0);

	for (int w = 1; w <= W; ++w) {
		for (size_t i = 0; i < n; ++i) {
			if (weight[i] <= w) {
				D[w] = std::max( D[w], D[w - weight[i]] + cost[i] );
			}
		}
	}

	cout << "knapsack_with_repetitions :: D:" << endl;
	show(D);

	return D[W];
}


int knapsack_without_repetitions(int W, const VI &weight, const VI &cost) {
	size_t n = weight.size();  // number of items
	VVI D(W+1, VI(n+1));  // Write ...VI(n+1, 0)... to eliminate next couple zero-initializing loops. But they are more explicit

	for (int w = 0; w <= W; ++w) {
		D[w][0] = 0;
	}
	for (size_t i = 0; i <= n; ++i) {
		D[0][i] = 0;
	}

	for (size_t i = 1; i <= n; ++i) {
		for (int w = 0; w <= W; ++w) {
			D[w][i] = D[w][i-1];

			if (weight[i-1] <= w) {
				D[w][i] = std::max( D[w][i], D[w-weight[i-1]][i-1]+cost[i-1] );  // weight and cost are indexed from 0, but i is 1..n;  the ..D[w-weight[i-1]][i-1].. is correct (second index is for i-1 items in knapsack)
			}
		}
	}

	cout << "knapsack_without_repetitions :: D:" << endl;
	show(D);

	return D[W][n];
}


// // Top Down dp (recursive)
// int knapsack_TD(w) {
// 	// TODO: write test case for this
// 	int v = 0;
//	// TODO: Hashtable?
// 	if (w not in H) {  // H is hashtable
// 		v = 0;
// 		for (size_t i = 0; i < n; ++i) {
// 			if (weight[i] <= w) {
// 				v = std::max(v, knapsack_TD(w-weight[i]) + cost[i]);
// 			}
// 		}
// 		H[w] = v;
// 	}

// 	return v;
// }


int main() {
	std::stringstream cin;
	cin << "10 4\n6 3 4 2\n30 14 16 9";  // ~>> 48 (reps), 46 (no reps)
//  =====
	int W;     // knapsack capacity
	size_t n;  // number of items
	cin >> W >> n;

	VI weight(n);  // items weights
	for (size_t i = 0; i < n; ++i) {
		cin >> weight[i];
	}

	VI cost(n);  // items costs
	for (size_t i = 0; i < n; ++i) {
		cin >> cost[i];
	}


	int x = knapsack_with_repetitions(W, weight, cost);
	cout << "~>> Knapsack with repetitions: " << x << endl;

	int y = knapsack_without_repetitions(W, weight, cost);
	cout << "~>> Knapsack without repetitions: " << y << endl;
}
