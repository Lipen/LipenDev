#include <iostream>

#include <SFML/Network.hpp>

using namespace sf;
using std::cout;
using std::endl;
using std::string;


int do_something() {
	TcpSocket socket;

	cout << "Connecting to server..." << endl;
	if (socket.connect("127.0.0.1", 9090, sf::seconds(5.)) != Socket::Done) {
		cout << "Bad connect" << endl;
		return -1;
	}
	cout << "Connected!" << endl;

	string data = "Hello, world";
	cout << "Sending \"Hello, world\"..." << endl;
	if (socket.send(data.c_str(), data.size()) != Socket::Done) {
		cout << "Bad send" << endl;
		return -2;
	}
	cout << "Sent successfully" << endl;

	return 0;
}


int main() {
	if (int ret = do_something()) {
		cout << "Error (" << ret << ") occured" << endl;
	}

	cout << "End." << endl;
}
