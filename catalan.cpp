#include <iostream>
#include <vector>

#define pb push_back

using namespace std;

struct Branch {
	string leafe;
	int left, right;

	Branch(string leafe, int left, int right)
	: leafe(leafe), left(left), right(right) {}
};

int main() {
	vector<Branch> tree;
	tree.pb(*(new Branch("(", 1, 0)));
	int n;
	cin >> n;

	for (int i=0; i<2*n-1; ++i) {
		vector<Branch> newtree;
		for (Branch b : tree) {
			if (n - b.left)
				newtree.pb(*(new Branch(b.leafe+'(', b.left+1, b.right)));
			if (b.right < b.left)
				newtree.pb(*(new Branch(b.leafe+')', b.left, b.right+1)));
		}
		tree = newtree;
	}

	for (Branch b : tree) {
		cout << b.leafe << endl;
	}
	cout << tree.size() << endl;
}
