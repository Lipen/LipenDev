#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


int main(int argc, char** argv) {
	int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	struct sockaddr_in sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(12345);
	sa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

	connect(sock, (struct sock_addr*)(&sa), sizeof(sa));

	char buf[] = "PINg";
	send(sock, buf, 4, MSG_NOSIGNAL);
	recv(sock, buf, 4, MSG_NOSIGNAL);

	shutdown(sock, SHUT_RDWR);
	close(sock);

	printf("%s\n", buf);

	return 0;
}
