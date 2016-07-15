#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>

using std::cout;
using std::cin;
using std::endl;
using std::string;


string derivative(string polynomial) {
	std::map<long int, long int> m;  // power - coef

	for (size_t i = 0; i < polynomial.size(); ++i) {
		if (polynomial[i] == 'x') {
			string coef;
			string power;

			int j;
			for (j = i-1; j >= 0 && polynomial[j] != '+' && polynomial[j] != '-'; --j) {
				if (polynomial[j] != '*') {
					coef = polynomial[j] + coef;
				}
			}

			for (size_t k = i + 1; k < polynomial.size() && polynomial[k] != '+' && polynomial[k] != '-'; ++k) {
				if (polynomial[k] != '^') {
					power += polynomial[k];
				}
			}


			long int c;
			if (coef.size() > 0) {
				c = stol(coef);
			}
			else {
				c = 1;
			}
			if (j >= 0 && polynomial[j] == '-') {
				c *= -1;
			}

			long int p;
			if (power.size() > 0) {
				p = stol(power);
			}
			else {
				p = 1;
			}

			if (m.find(p) != m.end()) {
				m[p] += c;
			}
			else {
				m[p] = c;
			}

			// cout << "c*x^p = " << c << "*x^" << p << endl;
		}
	}

	// for (std::map<long int, long int>::iterator it = m.begin(); it != m.end(); it++) {
	// 	cout << it->first << " / " << it->second << endl;
	// }

	std::stringstream ans;

	for (std::map<long int, long int>::reverse_iterator it = m.rbegin(); it != m.rend(); it++) {
		long int pc = it->second;
		long int pp = it->first;
		long int dc = pc;
		long int dp = pp - 1;

		if (pp > 0) {
			dc *= pp;
		}

		if (dc != 0) {
			std::stringstream ts;

			if (dc < -1) {
				ts << dc;
			}
			else if (dc == -1) {
				ts << '-';
			}
			else {
				if (it != m.rbegin()) {
					ts << '+';
				}
				ts << dc;
			}

			if (dp > 0) {
				ts << "*x";
			}
			if (dp > 1) {
				ts << '^' << dp;
			}

			ans << ts.str();
		}
	}

	return ans.str();
}


int main() {
	std::stringstream cin;
	// cin << "x-2*x^100+100*x^2-0*x^0-1";
	// cin << "2*x^100+100*x^2";
	// cin << "x^2+x";
	// cin << "-x^2-x^3";
	cin << "x+x+x+x+x+x+x+x+x+3*x^2";
//  =====
	string polynomial;
	cin >> polynomial;

	string deriv = derivative(polynomial);

	cout << deriv << endl;
}
