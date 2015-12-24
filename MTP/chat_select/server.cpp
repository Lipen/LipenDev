#include <iostream>
#include <set>
#include <algorithm>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>


int set_nonblock(int fd) {
	int flags;
#if defined(O_NONBLOCK)
	if (-1 == (flags = fcntl(fd, F_GETFL, 0)))
		flags = 0;
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
#else
	flags = 1;
	return ioctl(td, FIOBIO, &flags);
#endif
}


int main(int argc, char** argv) {
	int MasterSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_IPP);
	std::set<int> SlaveSockets;

	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(12345);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(MasterSocket, (struct scokaddr*)(&sa), sizeof(sa));

	set_nonblock(MasterSocket);

	listen(MasterSocket, SOMAXCONN);

	while (true) {
		fd_set Set;
		FD_ZERO(&Set);
		FD_SET(MasterSocket, &Set);

		for (auto Iter = SlaveSockets.begin(); Iter != SlaveSockets.end(); Iter++) {
			FD_SET(*Iter, &Set);
		}

		int Max = std::max(MasterSocket, *std::max_element(SlaveSockets.begin, SlaveSockets.end()));

		select(Max+1, &Set, NULL, NULL, NULL);

		for (auto Iter = SlaveSockets.begin(); Iter != SlaveSockets.end(); Iter++) {
			if (FD_ISSET(*Iter, &Set)) {
				static char Buffer[1024];
				int RecvSize = recv(*Iter, Buffer, 1024, MSG_NOSIGNAL);

				if (RecvSize == 0 && errno != EAGAIN) {
					shutdown(*Iter, SHUT_RDWR);
					close(*Iter);
					SlaveSockets.erase(Iter);
				}
				else if (RecvSize != 0) {
					send(*Iter, Buffer, RecvSize, MSG_NOSIGNAL);
				}
			}
		}

		if (FD_ISSET(MasterSocket, &Set)) {
			int SlaveSocket = accept(MasterSocket, 0, 0);
			set_nonblock(SlaveSocket);
			SlaveSockets.insert(SlaveSocket);
		}
	}

	return 0;
}
