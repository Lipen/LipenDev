#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(int argc, char** argv) {
	int socket_master = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(12345);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(socket_master, (struct sockaddr*)(&sa), sizeof(sa));

	listen(socket_master, SOMAXCONN);

	while (1) {
		int socket_slave = accept(socket_master, 0, 0);

		char buf[5] = {0, 0, 0, 0, 0};
		recv(socket_slave, buf, 4, MSG_NOSIGNAL);
		printf("Server got: %s\n", buf);
		buf[1] = '@';
		send(socket_slave, buf, 4, MSG_NOSIGNAL);
		shutdown(socket_slave, SHUT_RDWR);
		close(socket_slave);
	}

	printf("Server closed.");

	return 0;
}
