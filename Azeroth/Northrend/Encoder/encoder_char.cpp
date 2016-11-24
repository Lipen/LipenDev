#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using std::cout;
using std::endl;
using std::string;
using std::vector;
using Data = std::map<char, vector<size_t>>;  // <char, vector of positions>

string filename_original = "input.txt";
string filename_encoded = "output.txt";


Data* encode(string filename) {
	cout << "[*] Encoding..." << endl;
	std::ifstream fi { filename };  // Input file

	if (fi) {
		Data* data = new Data;
		size_t i = 0;  // Current position
		char c;  // Current char

		while (fi.get(c)) {
			if (data->find(c) != data->end()) {
				(*data)[c].push_back(i);
			} else {
				(*data)[c] = { i };
			}

			++i;
		}

		fi.close();
		cout << "[+] Encoded (" << filename << ")" << endl;
		return data;
	} else {
		cout << "[-] Couldn't open " << filename << endl;
		return nullptr;
	}
}


void dump(const Data& data, string filename) {
	cout << "[*] Writing to file..." << endl;
	std::ofstream fo { filename };  // Output file

	if (fo) {
		for (auto&& item : data) {
			fo << item.first;  // Write char
			for (size_t p : item.second) {  // Write positions
				fo << ' ' << p;
			}
			fo << endl;
		}

		fo.close();
		cout << "[+] File written (" << filename << ")" << endl;
	} else {
		cout << "[-] Couldn't write to " << filename << endl;
	}
}


string decode(string filename) {
	cout << "[*] Trying to restore original..." << endl;
	std::ifstream fe { filename_encoded };

	if (fe) {
		std::vector<char> v;  // Restored string as vector of chars
		// 1) Read first character
		char c;

		while (fe.get(c)) {
			// + maybe skip space, but I don't care
			// 2) Read rest of the line
			string line;
			std::getline(fe, line);
			std::stringstream ss { line };

			while (ss) {
				size_t p;
				ss >> p;
				if (p >= v.size()) {  // Resize vector if need
					v.resize(p+1);
				}
				v[p] = c;  // Set according character
			}
		}

		fe.close();
		cout << "[+] Original restored" << endl;
		return { v.begin(), v.end() };
	} else {
		cout << "[-] Couldn't open " << filename << endl;
		throw "Couldn't open file";
	}
}


int main() {
	Data encoded_data = *encode(filename_original);
	dump(encoded_data, filename_encoded);
	string restored = decode(filename_encoded);

	cout << "[+] Restored file:" << endl;
	cout << restored << endl;

	cout << "[@] END." << endl;
}
