#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define pb push_back

using namespace std;

void align(vector<string> &f_lines, int N) {
	for (int i=0; i<(int)f_lines.size(); i++) {
		string &line = f_lines[i];
		if (line[0] == ' ') {
			cout << "DBG::Inserting\n";
			line.insert(line.begin(), N-1, ' ');
		}
	}
}

vector<string> getlines(ifstream &ifs) {
	vector<string> f_lines;

	while (ifs.good()) {
		string t;
		getline(ifs, t);
		f_lines.pb(t);
	}

	return f_lines;
}

void writelines(ofstream &ofs, vector<string> f_lines) {
	for(int i=0; i<(int)f_lines.size(); i++) {
		ofs << f_lines[i];
		if (i < (int)f_lines.size()-1) {
			ofs << '\n';
		}
	}
}

int main() {
	ifstream fi("input.txt");

	if (fi) {
		int N = 8; //TODO: input

		// Read file
		vector<string> f_lines = getlines(fi);

		// Align file (TaskB)
		align(f_lines, N);

		// Write aligned file
		ofstream fo("output.txt");
		writelines(fo, f_lines);

		// Close all
		fi.close();
		fo.close();
	} else {
		std::cout << "Unable to open input file :c\n";
	}

	return 0;
}
