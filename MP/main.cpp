#include <iostream>
#include <iomanip>
#include <string>
#include <windows.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;


INT main() {
	cout << "Main started" << endl;

	cout << "Enter n: " << std::flush;
	int n;
	cin >> n;

	string argsServer = "Server.exe " + std::to_string(n);


//	Pipe between Main and Server processes:
	string pipeName = "\\\\.\\pipe\\main-server_pipe";
	DWORD bytesToRead = 255;
	char buf[bytesToRead];

	// Create pipe
	HANDLE hPipe = CreateNamedPipe(pipeName.c_str(), PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 4096, 4096, NMPWAIT_USE_DEFAULT_WAIT, NULL);

	if (hPipe == INVALID_HANDLE_VALUE) {
		cout << "Hm?.. CreatePipe failed with errcode " << GetLastError() << "." << endl;
	}


//	Server.exe:
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	// Creating new process
	if (CreateProcess(NULL, const_cast<char*>(argsServer.c_str()), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
	} else {
		cout << "Hm?.. CreateProcess failed with errcode " << GetLastError() << "." << endl;
		return 0;
	}



//	Establish pipe connection with Server:
	if (ConnectNamedPipe(hPipe, NULL) == 0) {
		int errcode = GetLastError();
		cout << "Hm... Client (Server.exe) got errcode " << errcode << "." << endl;
		if (errcode != ERROR_PIPE_CONNECTED) {
			cout << "Critical errcode " << errcode << "!" << endl;
			return 0;
		}
	}
	// Get message from Server
	ReadFile(hPipe, buf, bytesToRead, &bytesToRead, NULL);


	// Wait for end
	WaitForSingleObject(pi.hProcess, INFINITE);
	// Close handles
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);


	cout << "Message from Server.exe: ";
	for (DWORD i = 0; i < bytesToRead; ++i) {
		cout << buf[i];
	}
	cout << endl;


	cout << "Main end." << endl;
}
