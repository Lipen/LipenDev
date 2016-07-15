#include <iostream>
#include <sstream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;


template<typename T>
void show(std::vector<T> v) {
	cout << '\t';
	for (size_t i = 0; i < v.size(); ++i) {
		cout << v[i] << " ";
	}
	cout << endl;
}


int coin_change(const std::vector<int> &coins, const int change) {
	std::vector<int> seq(change + 1);
	std::vector<int> S(change + 1, 2147483647);
	S[0] = 0;

	for (int i = 1; i <= change; i++) {
		for(size_t j = 0; j < coins.size(); j++) {
			if (i >= coins[j] && 1 + S[i-coins[j]] < S[i]) {
				S[i] = 1 + S[i - coins[j]];
				seq[i] = j;
			}
		}
	}

	cout << "S:" << endl;
	show(S);

	cout << "Coins:";
	int j = change;
	while (j) {
		cout << " " << coins[seq[j]];
		j -= coins[seq[j]];
	}
	cout << endl;

	return S[change];
}

int main() {
	std::stringstream cin;
	cin << "40 5\n1 5 10 21 25";
//
	int change;
	size_t n;
	cin >> change >> n;

	std::vector<int> coins(n);
	for (size_t i = 0; i < n; ++i) {
		cin >> coins[i];
	}

	cout << coin_change(coins, change) << endl;
}
