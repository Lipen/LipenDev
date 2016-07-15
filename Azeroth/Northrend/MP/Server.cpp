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


//	Pipe between Server and Sum:
	string pipeNameSum = "\\\\.\\pipe\\server-sum_pipe";
	DWORD bytesToReadSum = 255;
	char bufSum[255];

	// Create pipe
	HANDLE hPipeSum = CreateNamedPipe(pipeNameSum.c_str(), PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 4096, 4096, NMPWAIT_USE_DEFAULT_WAIT, NULL);

	if (hPipeSum == INVALID_HANDLE_VALUE) {
		cout << "Hm?.. CreatePipe for Sum failed with errcode " << GetLastError() << "." << endl;
	}

//	Pipe between Server and Mult:
	string pipeNameMult = "\\\\.\\pipe\\server-mult_pipe";
	DWORD bytesToReadMult = 255;
	char bufMult[255];

	// Create pipe
	HANDLE hPipeMult = CreateNamedPipe(pipeNameMult.c_str(), PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 4096, 4096, NMPWAIT_USE_DEFAULT_WAIT, NULL);

	if (hPipeMult == INVALID_HANDLE_VALUE) {
		cout << "Hm?.. CreatePipe for Mult failed with errcode " << GetLastError() << "." << endl;
	}


//	Sum.exe:
	STARTUPINFO si_sum;
	PROCESS_INFORMATION pi_sum;

	ZeroMemory(&si_sum, sizeof(STARTUPINFO));
	si_sum.cb = sizeof(STARTUPINFO);

	// Creating new process
	if (!CreateProcess(NULL, const_cast<char*>(argsSum.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si_sum, &pi_sum)) {
		cout << "Hm? CreateProcess failed with errcode " << GetLastError() << "." << endl;
		return 0;
	}


//	Mult.exe:
	STARTUPINFO si_mult;
	PROCESS_INFORMATION pi_mult;

	ZeroMemory(&si_mult, sizeof(STARTUPINFO));
	si_mult.cb = sizeof(STARTUPINFO);

	// Creating new process
	if (!CreateProcess(NULL, const_cast<char*>(argsMult.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si_mult, &pi_mult)) {
		cout << "Hm? Mult.exe: CreateProcess failed with errcode " << GetLastError() << "." << endl;
		return 0;
	}




//	Pipe between Server and Sum:
	if (ConnectNamedPipe(hPipeSum, NULL) == 0) {
		int errcode = GetLastError();
		if (errcode != ERROR_PIPE_CONNECTED) {
			cout << "Hm?!. Client (Sum.exe) got critical errcode " << errcode << "!" << endl;
			return 0;
		}
	}

//	Pipe between Server and Mult:
	if (ConnectNamedPipe(hPipeMult, NULL) == 0) {
		int errcode = GetLastError();
		if (errcode != ERROR_PIPE_CONNECTED) {
			cout << "Hm?!. Client (Mult.exe) got critical errcode " << errcode << "!" << endl;
			return 0;
		}
	}


	// Get message from Sum
	ReadFile(hPipeSum, bufSum, bytesToReadSum, &bytesToReadSum, NULL);
	// Get message from Mult
	ReadFile(hPipeMult, bufMult, bytesToReadMult, &bytesToReadMult, NULL);


	char bufAnswer[] = "\0";
	DWORD bytesToAnswer = 1;
	WriteFile(hPipeSum, bufAnswer, 1, &bytesToAnswer, NULL);


	// Wait for end
	WaitForSingleObject(pi_sum.hProcess, INFINITE);
	bytesToAnswer = 1;
	WriteFile(hPipeMult, bufAnswer, 1, &bytesToAnswer, NULL);
	WaitForSingleObject(pi_mult.hProcess, INFINITE);


//	Pipe between Main and Server processes:
	string pipeName = "\\\\.\\pipe\\main-server_pipe";
	DWORD bytesToWrite;
	// Convert received data to int using atoi function, then convert to_string
	string buf = "Sum was equal to " + std::to_string(atoi(bufSum)) + ". Mult was equal to " + std::to_string(atoi(bufMult)) + ".";

	// Open pipe
	HANDLE hPipe = CreateFile(pipeName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (hPipe == INVALID_HANDLE_VALUE) {
		cout << "Hm?.. CreateFile failed with errcode " << GetLastError() << "." << endl;
	}

	// Send message to Main
	WriteFile(hPipe, const_cast<char*>(buf.c_str()), buf.size(), &bytesToWrite, NULL);


	// Close handles
	CloseHandle(hPipeSum);
	CloseHandle(hPipeMult);
	CloseHandle(pi_sum.hThread);
	CloseHandle(pi_sum.hProcess);
	CloseHandle(pi_mult.hThread);
	CloseHandle(pi_mult.hProcess);

	cout << "Server stopped." << endl;
	return 0;
}
