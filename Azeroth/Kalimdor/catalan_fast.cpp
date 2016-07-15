#include <iostream>
#include <ctime>
#include <cstring>

#define pb push_back

using namespace std;

int n;
long int amount;
char* builder;

void gen(int left, int right) {
	if (left + right == 2*n) {
		// cout << builder << endl;
		++amount;
		return;
	}
	if (left < n) {
		char* t = new char[n*2+1];
		strcpy(t, builder);
		builder[left + right] = '(';
		gen(left+1, right);
		strcpy(builder, t);
		delete[] t;
	}
	if (right < n && right < left) {
		char* t = new char[n*2+1];
		strcpy(t, builder);
		builder[left + right] = ')';
		gen(left, right+1);
		strcpy(builder, t);
		delete[] t;
	}
}

int main() {
	cin >> n;
	cout << "Processing n = " << n << "..." << endl;
	clock_t timeStart = clock();

	builder = new char[n*2+1];
	builder[0] = '(';
	gen(1, 0);
	cout << "Total: " << amount << endl;

	cout << "Time: " << (double)(clock() - timeStart)/CLOCKS_PER_SEC << " seconds" << endl;
}
