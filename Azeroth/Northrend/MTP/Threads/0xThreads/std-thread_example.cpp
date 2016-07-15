#include <iostream>
#include "mingw.thread.h"

using std::cout;
using std::endl;

void callback() {
	cout << "Thread callback!" << endl;
}

int main() {
	std::thread t(callback);

	t.join();

	cout << "Main end." << endl;
}
