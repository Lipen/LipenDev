#include <iostream>
#include <iomanip>
#include <string>
#include <windows.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;


INT main(int argc, char* argv[]) {
	cout << "Server.exe works..." << endl;

	if (argc < 2) {
		cout << "Incorrect Server.exe usage. Pass <n> as first argument." << endl;
		return 0;
	}

	int n = atoi(argv[1]);

	if (n < 1) {
		cout << "Incorrect Server.exe usage. Pass positive <n> greater then zero as first argument." << endl;
		return 0;
	}


	string argsSum = "Sum.exe";
	string argsMult = "Mult.exe";

	cout << "Enter n = " << n << " numbers: " << std::flush;
	for (long int i = 0; i < n; ++i) {
		int tmp;
		cin >> tmp;
		cout << "\t" << tmp << " has been read" << endl;

		string tmp_s = " " + std::to_string(tmp);
		// Append args strings
		argsSum += tmp_s;
		argsMult += tmp_s;
	}


//	Sum.exe:
	STARTUPINFO si_sum;
	PROCESS_INFORMATION pi_sum;

	ZeroMemory(&si_sum, sizeof(STARTUPINFO));
	si_sum.cb = sizeof(STARTUPINFO);

	// Creating new process
	if (CreateProcess(NULL, const_cast<char*>(argsSum.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si_sum, &pi_sum)) {
	} else {
		cout << "Hm? Errcode " << GetLastError() << "." << endl;
		return 0;
	}
	// Wait for end
	WaitForSingleObject(pi_sum.hProcess, INFINITE);
	// Close handles
	CloseHandle(pi_sum.hThread);
	CloseHandle(pi_sum.hProcess);


//	Mult.exe:
	STARTUPINFO si_mult;
	PROCESS_INFORMATION pi_mult;

	ZeroMemory(&si_mult, sizeof(STARTUPINFO));
	si_mult.cb = sizeof(STARTUPINFO);

	// Creating new process
	if (CreateProcess(NULL, const_cast<char*>(argsMult.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si_mult, &pi_mult)) {
	} else {
		cout << "Hm? Errcode " << GetLastError() << "." << endl;
		return 0;
	}
	// Wait for end
	WaitForSingleObject(pi_mult.hProcess, INFINITE);
	// Close handles
	CloseHandle(pi_mult.hThread);
	CloseHandle(pi_mult.hProcess);


	cout << "Server stopped." << endl;
	return 0;
}
