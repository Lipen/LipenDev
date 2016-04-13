#include <iostream>
#include <sstream>
#include <vector>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;


void show(std::vector<int> v) {
	cout << "[";
	for (size_t i = 0; i < v.size(); ++i) {
		cout << v[i];
		if (i+1 < v.size())  cout << ", ";
	}
	cout << "]";
}


std::vector<int> prefix_function_trivial(string s) {
	int n = s.size();
	std::vector<int> pi(n);

	for (int i = 0; i < n; ++i) {
		for (int k = 0; k <= i; ++k) {
			if (s.substr(0, k) == s.substr(i-k+1, k)) {
				pi[i] = k;
			}
		}
	}

	return pi;
}


std::vector<int> prefix_function(string s) {
	int n = s.size();
	std::vector<int> pi(n);

	for (int i = 1; i < n; ++i) {
		int j = pi[i-1];

		while (j > 0  &&  s[i] != s[j])
			j = pi[j-1];

		if (s[i] == s[j])
			++j;

		pi[i] = j;
	}

	return pi;
}


int main() {
	std::stringstream cin;
	// cin << "abcabcd";  // 0 0 0 1 2 3 0
	cin << "aabaaab";  // 0 1 0 1 2 2 3
//  =====
	string s;
	cin >> s;

	cout << "prefix_function_trivial(\"" << s << "\") = ";
	show(prefix_function_trivial(s));
	cout << endl;

	cout << "prefix_function(\"" << s << "\") = ";
	show(prefix_function(s));
	cout << endl;
}
