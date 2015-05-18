#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <string>
#include <sstream>

#define pb push_back

using std::cout;
using std::cin;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::string;
using std::map;

class TaskA {
	TaskA() {}

public:
	static vector<string> getWords(ifstream &ifs);
	static void mixFiles(ofstream &ofs, int M, vector<string> words1, vector<string> words2, string sep);
};

class TaskB {
	TaskB() {}

public:
	static vector<string> getWords(ifstream &ifs);
	static void align(vector<string> &fLines, int N);
	static vector<string> getLines(ifstream &ifs);
	static void writeLines(ofstream &ofs, vector<string> fLines);
};

class TaskC {
	TaskC() {}

public:
	static vector<string> getWords(ifstream &ifs);
	static void findString(ifstream &ifs, string s, int &pT, int &pL, int &pC);
};

class TaskD {
	TaskD() {}

public:
	static map<char, int> countCharsSimple(ifstream &ifs);
	static void printHistrogramSimple(const map<char, int> &charsAmount);
};

class ExtraUtils {
	ExtraUtils() {}

public:
	static string joinWithSeparator(vector<string> s, string sep);
	static string caseString(string s, bool upperCase);
	static bool compareStrings(string s, string t, bool ignoreCase);
	template<size_t N>
	static bool match(string toMatch, const string (&matchWith)[N], bool ignoreCase);
};


vector<string> TaskA::getWords(ifstream &ifs) {
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

void TaskA::mixFiles(ofstream &ofs, int M, vector<string> words1, vector<string> words2, string sep=" ") {
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

	ofs << ExtraUtils::joinWithSeparator(mixed, sep);
}


vector<string> TaskB::getWords(ifstream &ifs) {
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

void TaskB::align(vector<string> &fLines, int N) {
	for (int i=0; i<(int)fLines.size(); ++i) {
		string &line = fLines[i];

		if (line[0] == ' ') {
			cout << "DBG::Inserting\n";
			line.insert(line.begin(), N-1, ' ');
		}
	}
}

vector<string> TaskB::getLines(ifstream &ifs) {
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

void TaskB::writeLines(ofstream &ofs, vector<string> fLines) {
	for(int i=0; i<(int)fLines.size(); ++i) {
		ofs << fLines[i];

		if (i < (int)fLines.size()-1) {
			ofs << '\n';
		}
	}
}


vector<string> TaskC::getWords(ifstream &ifs) {
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

void TaskC::findString(ifstream &ifs, string s, int &pT, int &pL, int &pC) {
	ifs.clear();
	ifs.seekg(0, ifs.beg);

	int t = 0;  // total counter
	int l = -1;  // line counter
	int c = -1;  // column counter

	while (ifs.good()) {
		l++;
		cout << "DBG:processing line#" << l+1 << '\n';
		string line;
		std::getline(ifs, line);
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


map<char, int> TaskD::countCharsSimple(ifstream &ifs) {
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

void TaskD::printHistrogramSimple(const map<char, int> &charsAmount) {
	map<char, vector<char>> matrix;
	int maxAmount = 0;

	for (map<char, int>::const_iterator iter=charsAmount.begin(); iter!=charsAmount.end(); ++iter) {
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
			if (i < charsAmount.at(iter->first)) {
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


string ExtraUtils::joinWithSeparator(vector<string> s, string sep) {
	std::stringstream ss;
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

string ExtraUtils::caseString(string s, bool upperCase) {
	std::stringstream ss;

	for (int i=0; i<(int)s.size(); ++i) {
		ss << (upperCase) ? std::toupper(s[i]) : std::tolower(s[i]);
	}

	return ss.str();
}

bool ExtraUtils::compareStrings(string s, string t, bool ignoreCase=false) {
	if (s.size() != t.size())
		return false;

	for (int i=0; i<(int)s.size(); ++i) {
		if (ignoreCase) {
			if (std::tolower(s[i]) != std::tolower(t[i]))
				return false;
		} else {
			if (s[i] != t[i])
				return false;
		}
	}

	return true;
}

template<size_t N>
bool ExtraUtils::match(string toMatch, const string (&matchWith)[N], bool ignoreCase=true) {
	for (int i=0; i<(int)N; ++i) {
		if (ExtraUtils::compareStrings(toMatch, matchWith[i], ignoreCase)) {
			return true;
		}
	}

	return false;
}

template<class T>
using matcher = T;


int process() {
	cout << "Choose task:\n\t0) Exit\n\t...\n\t1) A: mix\n\t2) B: align\n\t3) C: search\n\t4) D: count\n...";

	string task;
	cin >> task;

	if (ExtraUtils::match(task, matcher<string[]>{"0", "E", "Exit"})) {
		system("cls");
		cout << "Quitting...\n";

		return 4;

	} else if (ExtraUtils::match(task, matcher<string[]>{"1", "A", "mix"})) {
		system("cls");
		cout << "Chosen task A.\n";

		cout << "Enter first input file`s name: ";
		string inName1;
		cin >> inName1;
		ifstream fi1(inName1);
		if (!fi1) {
			cout << "Couldn`t open '" << inName1 << "'.\n";
			return 2;
		}

		cout << "Enter second input file`s name: ";
		string inName2;
		cin >> inName2;
		ifstream fi2(inName2);
		if (!fi2) {
			cout << "Couldn`t open '" << inName2 << "'\n";
			return 2;
		}

		cout << "Enter output file`s name: ";
		string outName;
		cin >> outName;
		ofstream fo(outName);

		cout << "Enter M ('mix by M words'): ";
		string Mstr;
		cin >> Mstr;
		int M = 0;

		try {
			M = std::stoi(Mstr);
		} catch (...) {
			cout << "Couldn`t convert '" << Mstr << "' to integer.\n";
			return 3;
		}

		cout << "Mixing " << inName1 << " and " << inName2 << " by " << M << " words into " << outName << "...\n";
		TaskA::mixFiles(fo, M, TaskA::getWords(fi1), TaskA::getWords(fi2));
		cout << "Mixed successfully.\n";

		// Don`t forget to close all
		fi1.close();
		fi2.close();
		fo.close();

	} else if (ExtraUtils::match(task, matcher<string[]>{"2", "B", "align"})) {
		system("cls");
		cout << "Chosen task B.\n";

		cout << "Enter input file`s name: ";
		string inName;
		cin >> inName;
		ifstream fi(inName);
		if (!fi) {
			cout << "Couldn`t open '" << inName << "'.\n";
			return 2;
		}

		cout << "Enter output file`s name: ";
		string outName;
		cin >> outName;
		ofstream fo(outName);

		cout << "Enter N ('align with N spaces'): ";
		string Nstr;
		cin >> Nstr;
		int N = 0;

		try {
			N = std::stoi(Nstr);
		} catch (...) {
			cout << "Couldn`t convert '" << Nstr << "' to integer.\n";
			return 3;
		}

		cout << "Aligning " << inName << " with " << N << " spaces into " << outName << "...\n";
		vector<string> fLines = TaskB::getLines(fi);
		TaskB::align(fLines, N);
		TaskB::writeLines(fo, fLines);
		cout << "Aligned successfully.\n";

		// Don`t forget to close all
		fi.close();
		fo.close();

	} else if (ExtraUtils::match(task, matcher<string[]>{"3", "C", "search", "find"})) {
		system("cls");
		cout << "Chosen task C.\n";

		cout << "Enter input file`s name: ";
		string inName;
		cin >> inName;
		ifstream fi(inName);
		if (!fi) {
			cout << "Couldn`t open '" << inName << "'.\n";
			return 2;
		}

		cout << "Enter string to find: ";
		string stringToFind;
		cin >> stringToFind;

		cout << "Searching for '" << stringToFind << "' in " << inName << "...\n";
		int posTotal = 0;
		int posLine = 0;
		int posColumn = 0;
		TaskC::findString(fi, stringToFind, posTotal, posLine, posColumn);

		if (posTotal != -1) {
			cout << "Found '" << stringToFind << "' at pos " << posTotal+1 << " (Line " << posLine+1 << ", Column " << posColumn+1 << ")\n";
		} else {
			cout << "'" << stringToFind << "' not found :c\n";
		}

		// Don`t forget to close all
		fi.close();

	} else if (ExtraUtils::match(task, matcher<string[]>{"4", "D", "histogram", "count", "frequency"})) {
		system("cls");
		cout << "Chosen task D.\n";

		cout << "Enter input file`s name: ";
		string inName;
		cin >> inName;
		ifstream fi(inName);
		if (!fi) {
			cout << "Couldn`t open '" << inName << "'.\n";
			return 2;
		}

		cout << "Counting chars in " << inName << "...\n";
		map<char, int> charsAmount = TaskD::countCharsSimple(fi);
		cout << "Counted.\nChars` frequency histogram:\n";
		TaskD::printHistrogramSimple(charsAmount);

		// Don`t forget to close all
		fi.close();

	} else {
		return 1;
	}

	return 0;
}

int main() {
	/*
		// TODO: exceptions
		CALLBACKERRORCODES:
		0 - OK
		1 - wrong task
		2 - input file openning error
		3 - expected integer, given not
		4 - manual(user) exit
	*/
	bool running = true;
	system("cls");

	while (running) {
		int e = process();

		if (e == 0) {
			system("pause");
			system("cls");
		} else if (e == 1) {
			system("cls");
			cout << "Wrong choice, try one more time\n";
		} else if (e == 4) {
			std::cout << "Manual quit\n";
			running = false;
		} else {
			std::cout << "Something gone wrong (" << e << ").\n";
			running = false;
			system("pause");
		}
	}

	return 0;
}
