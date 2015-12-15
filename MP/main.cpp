#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <windows.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;


INT main() {
	// std::stringstream cin;
	// cin << "5\n2 3 4 8 3\n";
//  =====
	cout << "Main started" << endl;

	cout << "Enter n: " << std::flush;
	int n;
	cin >> n;

	string argsServer = "Server.exe " + std::to_string(n);


//	Server.exe:
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	// Creating new process
	if (CreateProcess(NULL, const_cast<char*>(argsServer.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
	} else {
		cout << "Hm? Errcode " << GetLastError() << "." << endl;
		return 0;
	}
	// Wait for end
	WaitForSingleObject(pi.hProcess, INFINITE);
	// Close handles
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);


	cout << "Main end." << endl;
}
