#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <iterator>
#include <algorithm>

using std::cout;
using std::cin;
using std::endl;

using HuffCode = std::vector<bool>;
using HuffCodeMap = std::map<char, HuffCode>;


struct INode {
	const int f;

	virtual ~INode() {}

protected:
	INode(int f) : f(f) {}
};

struct InternalNode : public INode {
	INode *const left;
	INode *const right;

	InternalNode(INode* c0, INode* c1) : INode(c0->f + c1->f), left(c0), right(c1) {}

	~InternalNode() {
		delete left;
		delete right;
	}
};

struct LeafNode : public INode {
	const char c;

	LeafNode(int f, char c) : INode(f), c(c) {}
};

struct NodeCmp {
	bool operator()(const INode* lhs, const INode* rhs) const {
		return lhs->f > rhs->f;
	}
};


INode* BuildTree(const int (&frequencies)[256]) {
	std::priority_queue<INode*, std::vector<INode*>, NodeCmp> trees;

	for (int i = 0; i < 256; ++i) {
		if (frequencies[i] != 0) {
			trees.push(new LeafNode(frequencies[i], (char)i));
		}
	}
	while (trees.size() > 1) {
		INode* childR = trees.top();
		trees.pop();

		INode* childL = trees.top();
		trees.pop();

		INode* parent = new InternalNode(childR, childL);
		trees.push(parent);
	}
	return trees.top();
}


void GenerateCodes(const INode* node, const HuffCode& prefix, HuffCodeMap& outCodes) {
	if (const LeafNode* lf = dynamic_cast<const LeafNode*>(node)) {
		outCodes[lf->c] = prefix;
	}
	else if (const InternalNode* in = dynamic_cast<const InternalNode*>(node)) {
		HuffCode leftPrefix = prefix;
		leftPrefix.push_back(false);
		GenerateCodes(in->left, leftPrefix, outCodes);

		HuffCode rightPrefix = prefix;
		rightPrefix.push_back(true);
		GenerateCodes(in->right, rightPrefix, outCodes);
	}
}


int main() {
	const char* ptr = "this is an example for huffman encoding";

	int frequencies[256];  // should be initialized with all zeros
	while (*ptr != '\0') ++frequencies[*ptr++];

	INode* root = BuildTree(frequencies);

	HuffCodeMap codes;
	GenerateCodes(root, HuffCode(), codes);
	delete root;

	for (HuffCodeMap::const_iterator it = codes.begin(); it != codes.end(); ++it) {
		cout << it->first << " ";
		std::copy(it->second.begin(), it->second.end(), std::ostream_iterator<bool>(cout));
		cout << endl;
	}
}
