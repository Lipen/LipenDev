#include <iostream>

#include <SFML/Network.hpp>

using namespace sf;
using std::cout;
using std::endl;
using std::string;


int do_something() {
	TcpListener listener;

	if (listener.listen(9090) != Socket::Done) {
		cout << "Bad listen" << endl;
		return -1;
	}

	TcpSocket client;
	cout << "Waiting for client..." << endl;
	if (listener.accept(client) != Socket::Done) {
		cout << "Bad accept" << endl;
		return -2;
	}
	cout << "Client accepted!" << endl;

	char data[1024];
	size_t received;
	cout << "Receiving 1024 bytes..." << endl;
	if (client.receive(data, 1024, received) != Socket::Done) {
		cout << "Bad receive" << endl;
		return -3;
	}

	cout << "Received: " << string(data) << endl;

	return 0;
}


int main() {
	if (int ret = do_something()) {
		cout << "Error (" << ret << ") occured" << endl;
	}

	cout << "End." << endl;
}
