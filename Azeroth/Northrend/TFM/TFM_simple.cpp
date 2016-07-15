#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <map>
#include <string>
#include <sstream>

#define pb push_back

using namespace std;

void align(vector<string> &fLines, int N) {
	for (int i=0; i<(int)fLines.size(); ++i) {
		string &line = fLines[i];

		if (line[0] == ' ') {
			cout << "DBG::Inserting\n";
			line.insert(line.begin(), N-1, ' ');
		}
	}
}

vector<string> getLines(ifstream &ifs) {
	ifs.clear();
	ifs.seekg(0, ifs.beg);

	vector<string> fLines;

	while (ifs.good()) {
		string t;
		getline(ifs, t);
		fLines.pb(t);
	}

	return fLines;
}

void writeLines(ofstream &ofs, vector<string> fLines) {
	for(int i=0; i<(int)fLines.size(); ++i) {
		ofs << fLines[i];

		if (i < (int)fLines.size()-1) {
			ofs << '\n';
		}
	}
}

vector<string> getWords(ifstream &ifs) {
	ifs.clear();
	ifs.seekg(0, ifs.beg);

	vector<string> words;
	string t;
	char c;

	while (ifs.get(c)) {
		if (c == ' ' or c == '\n') {
			if (!t.empty()) {
				words.pb(t);
				t.clear();
			}
		} else {
			t += c;
		}
	}
	// Extra action for possible last word
	if (!t.empty()) {
		words.pb(t);
		t.clear();
	}

	return words;
}

void findString(ifstream &ifs, string s, int &pT, int &pL, int &pC) {
	ifs.clear();
	ifs.seekg(0, ifs.beg);

	int t = 0;  // total counter
	int l = -1;  // line counter
	int c = -1;  // column counter

	while (ifs.good()) {
		l++;
		cout << "DBG:processing line#" << l+1 << '\n';
		string line;
		getline(ifs, line);
		size_t p = line.find(s); // default 0..?

		if (p != string::npos) {
			cout << "DBG:found!\n";
			t += (int)p;
			c = (int)p;
			break;
		} else {
			t += (int)line.size();
		}
	}

	if (c != -1) {
		pT = t;
		pL = l;
		pC = c;
	} else {
		pT = -1;
		pL = -1;
		pC = -1;
	}
}

map<char, int> countCharsSimple(ifstream &ifs) {
	ifs.clear();
	ifs.seekg(0, ifs.beg);

	map<char, int> charsAmount;

	// For 'not simple' version:
	// for (char c : "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~ ") {
	// 	charsAmount[c] = 0;
	// }

	for (char i='A'; i<='Z'; i++) {
		charsAmount[i] = 0;
	}
	for (char i='0'; i<='9'; i++) {
		charsAmount[i] = 0;
	}

	char c;
	while (ifs.get(c)) {
		if (charsAmount.find(std::toupper(c)) != charsAmount.end()) {
			charsAmount[std::toupper(c)]++;
		} // else just skip
	}

	return charsAmount;
}

void printHistrogramSimple(map<char, int> &charsAmount) {
	map<char, vector<char>> matrix;
	int maxAmount = 0;

	for (map<char, int>::iterator iter=charsAmount.begin(); iter!=charsAmount.end(); ++iter) {
		if (iter->second > maxAmount) {
			maxAmount = iter->second;
		}
		// there is no condition ('simple')
		vector<char> head;
		head.pb(iter->first);
		head.pb('-');
		matrix[iter->first] = head;
	}

	for (map<char, vector<char>>::iterator iter=matrix.begin(); iter!=matrix.end(); ++iter) {
		for (int i=0; i<maxAmount; ++i) {
			if (i < charsAmount[iter->first]) {
				iter->second.pb(iter->first);
			} else {
				iter->second.pb(' ');
			}
		}
	}

	for (int i=0; i<maxAmount; ++i) {
		// C++0x
		for (auto kv : matrix) {
			cout << kv.second[i];
		}
		cout << '\n';
	}
}

string joinWithSeparator(vector<string> s, string sep) {
	stringstream ss;
	vector<string>::iterator iter = s.begin();

	if (s.begin() == s.end())
		return "";

	while (true) {
		ss << *iter;
		++iter;
		if (iter == s.end()) {
			break;
		} else {
			ss << sep;
		}
	}

	return ss.str();
}

void mixFiles(ofstream &ofs, int M, vector<string> words1, vector<string> words2, string sep=" ") {
	if (M <= 0)
		return;

	// Reverse, so we can pop_back proper things
	std::reverse(words1.begin(), words1.end());
	std::reverse(words2.begin(), words2.end());

	vector<string> mixed;
	int i=0, j=0, k=0;
	int ie = (int)words1.size();
	int je = (int)words2.size();

	while (i < ie and j < je) {
		if (k%2 == 0) {
			for (int t=0; t<M and i<ie; ++t) {
				mixed.pb(words1.back());
				words1.pop_back();
				++i;
			}
		} else {
			for (int t=0; t<M and j<je; ++t) {
				mixed.pb(words2.back());
				words2.pop_back();
				++j;
			}
		}
		k++;
	}
	// Extra loops for remain words
	while (i < ie) {
		mixed.pb(words1.back());
		words1.pop_back();
		++i;
	}
	while (j < je) {
		mixed.pb(words2.back());
		words2.pop_back();
		++j;
	}

	ofs << joinWithSeparator(mixed, sep);
}

int main() {
	system("cls");
	ifstream fi("input.txt");
	ifstream fi1("input1.txt");
	ifstream fi2("input2.txt");

	if (fi && fi1 && fi2) {
		int N = 8; //TODO: input
		int M = 2; //TODO: input
		string stringToFind("gentleman"); //TODO: input

		// Read file
		vector<string> fileLines = getLines(fi);

		// Align file (TaskB)
		align(fileLines, N);

		// Write aligned file
		ofstream fo("output.txt");
		writeLines(fo, fileLines);

		// Find stringToFind and output it (TaskC)
		int posTotal = 0;
		int posLine = 0;
		int posColumn = 0;
		findString(fi, stringToFind, posTotal, posLine, posColumn);

		if (posTotal != -1) {
			cout << "Found " << stringToFind << " at pos " << posTotal+1 << " (Line " << posLine+1 << ", Column " << posColumn+1 << ")\n";
		} else {
			cout << stringToFind << " not found :c\n";
		}

		// Count amount of every char and output bar chart(histogram) (TaskD)
		map<char, int> charsAmount = countCharsSimple(fi);
		cout << "Chars` frequency histogram:\n";
		printHistrogramSimple(charsAmount);

		// Mix files by M words (TaskA)
		// (continiously get M words from each and put into output file)
		ofstream fo12("output12.txt");
		mixFiles(fo12, M, getWords(fi1), getWords(fi2));
		cout << "input1 and input2 has been mixed.\n";

		// Close all
		fi.close();
		fi1.close();
		fi2.close();
		fo.close();
		fo12.close();
	} else {
		std::cout << "Unable to open input file :c\n";
	}

	std::cout << "All done.\n";
	return 0;
}
