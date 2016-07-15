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

inline int min(int a, int b) {
	return (a < b) ? a : b;
}


std::vector<int> z_function_trivial(string s) {
	int n = s.size();
	std::vector<int> z(n);
	z[0] = n;  // Or leave it be zero

	for (int i = 1; i < n; ++i) {
		while (i+z[i] < n  &&  s[z[i]] == s[i+z[i]]) {
			++z[i];
		}
	}

	return z;
}


std::vector<int> z_function(string s) {
	int n = s.size();
	std::vector<int> z(n);
	z[0] = n;  // Or leave it be zero

	int l = 0, r = 0;

	for (int i = 1; i < n; ++i) {
		if (i <= r) {
			// Use fact that `s[l..r] == s[0..r-l]` => `z[i] = z[i-l]`
			// But that value may overflow, so:
			z[i] = min(r-i+1, z[i-l]);
		}
		// Use trivial algorithm
		while (i+z[i] < n  &&  s[z[i]] == s[i+z[i]]) {
			++z[i];
		}

		// Update bounds:
		if (i+z[i]-1 > r) {
			l = i;
			r = i + z[i] - 1;
		}
	}

	return z;
}


int main() {
	std::stringstream cin;
	// cin << "abacaba";  // 7(0) 0 1 0 3 0 1
	cin << "abracadabra";  // 11(0) 0 0 1 0 1 0 4 0 0 1
//  =====
	string s;
	cin >> s;

	cout << "z_function_trivial(\"" << s << "\") = ";
	show(z_function_trivial(s));
	cout << endl;

	cout << "z_function(\"" << s << "\") = ";
	show(z_function(s));
	cout << endl;
}
