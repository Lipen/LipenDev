#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
	signal(SIGINT, SIG_IGN);

	pid_t pid = fork();

	if (pid) {
		signal(SIGTERM, SIG_IGN);

		FILE* fp = fopen("pid_parent", "w");
		fprintf(fp, "%d\n", getpid());
		fclose(fp);

		FILE* fc = fopen("pid_child", "w");
		fprintf(fc, "%d\n", pid);
		fclose(fc);

		int ec;
		waitpid(pid, &ec, 0);
	}

	pause();
	return 0;
}
