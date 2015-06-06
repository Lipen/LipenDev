/* Copyright (c) 2015 Lipen */
/* Simple PNG Library */
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdint>

using std::cout;
using std::int8_t;
using std::int32_t;
using std::int64_t;
using std::string;
using std::vector;


int32_t read4bytes(ifstream &ifs) {
	int32_t s = 0;
	char c;
	int i = 0;

	while (ifs.get(c) && i<3) {
		s += (1 << (3-i++)) * c;
	}

	if (i < 3) {
		cout << "Stream ran out.\n";
		return -1;
	}
	return s;
}


void write_bytes(ofstream &ofs, int64_t number, int amount) {
	if (number >= (1 << (amount-1)*8)) {
		cout << "Number exceeds bytes amount.\n";
		return;
	}

	for (int i = amount-1; i >= 0; --i) {
		int t = 1 << i*8;
		ofs << (number / t);
		number %= t;
	}
}


int8_t* read_data(ifstream &ifs, int32_t width) {
	int8_t *buf = new int8_t[width];

	for (int i = 0; i < width; ++i) {
		ifs.get(buf[i]);
	}

	return buf;
}


void write_IDAT(ofstream &ofs, int8_t *buf, int32_t length, int32_t i) {
	write_bytes(ofs, length, 4);
	ofs << 'I' << 'D' << 'A' << 'T';
}


void write_data(ofstream &ofs, ifstream &ifs, int32_t width, int32_t height) {
	for (int i = 0; i < height; ++i) {
		int8_t *buf = read_data(ifs, width);
		int32_t length = width;  // would change after compressing..
		// +data.compress
		write_IDAT(ofs, buf, length, i);
	}
}


int write_png(string name_input, string name_output) {
	ifstream ifs(name_input, ios::binary);

	if (ifs) {
		int32_t width = read4bytes(ifs);
		int32_t height = read4bytes(ifs);
		if (width == -1 || height == -1) {
			cout << "Error reading size.\n";
			return 3;
		}
		int64_t size = width * height;

		int temp = ifs.tellg();
		ifs.seekg(0, fi.end)
		int64_t size_file = ifs.tellg();
		if (size_file != size) {
			cout << "Sizes doesn`t match.\n";
			return 4;
		}
		ifs.seekg(temp);

		ofstream ofs(name_output, ios::binary);
		write_pngheader(ofs);
		write_IHDR(ofs, width, height, 8);
		write_data(ofs, ifs, width, height);
	} else {
		cout << "Unable to open input file :c\n";
		return 1;
	}

	return 0;
}


int main() {
	string name_input("file_binary.bin");
	string name_output(std::to_string(time(0)) + ".png");

	int err_code = write_png(name_input, name_output);

	if (err_code) {
		cout << "Something gone wrong (" << err_code << ").\n";
	} else {
		cout << "Successfully done.\n";
	}

	return 0;
}
