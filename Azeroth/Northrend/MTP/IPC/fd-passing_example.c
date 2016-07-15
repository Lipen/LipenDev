#include <sys/socket.h>
#include <errno.h>

#include "fd-passing_write.inl"
#include "fd-passing_read.inl"


void child(int sock) {
	int fd;
	char buf[16];
	ssize_t size;

	sleep(1);
	for (;;) {
		size = sock_fd_read(sock, buf, sizeof(buf), &fd);
		if (size <= 0)
			break;
		printf("Read %d\n", size);
		if (fd != -1) {
			write(fd, "Hello, world!\n", 13);
			close(fd);
		}
	}
}

void parent(int sock) {
	ssize_t size;
	int i;
	int fd;

	fd = 1;
	size = sock_fd_write(sock, "1", 1, 1);
	printf("Wrote %d\n", size);
}


int main(int argc, char* argv[]) {
	int sv[2];
	int pid;

	if (socketpair(AF_LOCAL, SOCK_STREAM, 0, sv) < 0) {
		perror("Socketpair error");
		exit(1);
	}

	switch ((pid = fork())) {
	case 0:
		close(sv[0]);
		child(sv[1]);
		break;
	case -1:
		perror("Fork error");
		exit(1);
	default:
		close(sv[1]);
		parent(sv[0]);
		break;
	}

	return 0;
}
