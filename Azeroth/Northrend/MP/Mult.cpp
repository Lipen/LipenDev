#include <iostream>
#include <string>
#include <windows.h>

using std::cout;
using std::endl;
using std::string;


int main(int argc, char* argv[]) {
	cout << "Mult.exe works..." << endl;

//	Calculate product:
	long int mult = 1;

	for (int i = 1; i < argc; ++i) {
		mult *= atoi(argv[i]);
	}

	cout << "Mult: " << mult << endl;


//	Pipe between Server and Mult processes:
	string pipeName = "\\\\.\\pipe\\server-mult_pipe";
	DWORD bytesToWrite;
	string buf;

	// Open pipe
	HANDLE hPipe = CreateFile(pipeName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (hPipe == INVALID_HANDLE_VALUE) {
		cout << "Hm?.. Mult.exe: CreateFile failed with errcode " << GetLastError() << "." << endl;
	}

	// Send message to Server
	buf = std::to_string(mult);
	WriteFile(hPipe, const_cast<char*>(buf.c_str()), buf.size(), &bytesToWrite, NULL);

	// Wait for reply from Server:
	DWORD bytesToRead = 1;
	char buffer[1];
	ReadFile(hPipe, buffer, 1, &bytesToRead, NULL);

	cout << "Mult.exe got reply from server: <" << buffer << ">." << endl;


	CloseHandle(hPipe);

	cout << "Mult.exe end." << endl;

	return 0;
}
