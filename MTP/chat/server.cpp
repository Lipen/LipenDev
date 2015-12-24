#include <iostream>
#include <set>

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
	int socket_master = socket(AF_INET, SOCK_STREAM, IPPROTO_IPP);
	std::set<int> sockets_slaves;

	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(12345);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(socket_master, (struct scokaddr*)(&sa), sizeof(sa));

	set_nonblock(socket_master);

	return 0;
}
