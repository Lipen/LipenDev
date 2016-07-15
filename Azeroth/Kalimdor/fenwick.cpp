/* Copyright (C) 2016, Lipen */
#include <iostream>
#include <sstream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;


void show(std::vector<int> v) {
	for (size_t i = 1; i < v.size(); ++i) {
		cout << v[i];
		if (i + 1 < v.size())  cout << " ";
	}
	cout << endl;
}


int main() {
	std::stringstream cin;
	cin << "12\n4 2 7 5 1 3 6 4 6 6 3 3";
//  =====
	cout << ">\tEnter size (n) of Frequency table: ";
	int n;
	cin >> n;


	std::vector<int> freq(n+1);  // Frequency table

	cout << ">\tEnter n (" << n << ") numbers to Frequency table:" << endl;
	for (int i = 1; i <= n; ++i) {
		cin >> freq[i];
	}


	std::vector<int> bit(n+1);  // Fenwick Tree

	cout << "\n>\tBuilding Fenwick Tree..." << endl;
	for (int i = 1; i <= n; ++i) {
		int j = i;
		cout << "\tfreq[" << i << "] = " << freq[i] << endl;

		while (j <= n) {
			bit[j] += freq[i];
			j += j & -j;
		}

		cout << "\tFT: ";
		show(bit);
	}


	cout << "\n>\tFinal Fenwick Tree:" << endl;
	show(bit);
}
