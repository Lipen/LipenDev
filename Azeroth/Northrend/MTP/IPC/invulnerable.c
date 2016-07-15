#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char **argv) {
	signal(SIGTERM, SIG_IGN);
	signal(SIGINT, SIG_IGN);

	FILE* f = fopen("pid", "w");
	fprintf(f, "%d\n", getpid());
	fclose(f);

	pause();
	return 0;
}
