#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cstring>

#define pb push_back

using namespace std;

int n;
vector<string> answer;
char* builder;

void gen(int left, int right) {
	if (left + right == 2*n) {
		answer.pb(string(builder));
		return;
	}
	if (left < n) {
		char* t = new char[2*n];
		strcpy(t, builder);
		builder[left+right] = '(';
		gen(left+1, right);
		strcpy(builder, t);
		delete[] t;
	}
	if (right < n && right < left) {
		char* t = new char[2*n];
		strcpy(t, builder);
		builder[left+right] = ')';
		gen(left, right+1);
		strcpy(builder, t);
		delete[] t;
	}
}

int main() {
	cin >> n;
	clock_t timeStart = clock();

	builder = new char[2*n];
	builder[0] = '(';
	gen(1, 0);
	// for (vector<string>::iterator item = answer.begin(); item != answer.end(); ++item) {
	// 	cout << *item << endl;
	// }
	cout << "Total: " << answer.size() << endl;

	cout << "Time: " << (double)(clock() - timeStart)/CLOCKS_PER_SEC << " seconds" << endl;
}
