#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main(int argc, char* argv[]) {
	int fifo_in = mkfifo("in.fifo", 0666);
	int fifo_out = mkfifo("out.fifo", 0666);

	char buf[512];
	ssize_t r;

	printf("Line 12...\n");

	while ((r = read(fifo_in, buf, 512)) > 0) {
		printf("Read %zu: %s\n", r, buf);
		write(fifo_out, buf, r);
		memset(buf, 0, 512);
	}

	if (r < 0) {
		perror("Read -1");
	}

	printf("Closing...\n");

	close(fifo_in);
	close(fifo_out);
}
