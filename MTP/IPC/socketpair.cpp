#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char* argv[]) {
	int sv[2];
	socketpair(AF_UNIX, SOCK_STREAM, 0, sv);
	pid_t pid = fork();

	if (pid == -1) {
		perror("Fork error");
	}
	else if (pid == 0) {  /* Child */
		close(sv[0]);
		printf("Child pid: %d\n", getpid());
		pause();
	}
	else {
		close(sv[1]);
		printf("Parent pid: %d\n", getpid());
		wait(NULL);
		pause();
	}

	return 0;
}
