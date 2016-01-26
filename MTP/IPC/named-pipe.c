#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
	mkfifo("in.fifo", 0666);
	mkfifo("out.fifo", 0666);

	int fifo_in = open("in.fifo", O_RDONLY | O_CREAT);
	int fifo_out = open("out.fifo", O_WRONLY | O_CREAT);

	char buf[512];
	ssize_t r;

	printf("Line 12...\n");

	while ((r = read(fifo_in, buf, 10)) > 0) {
		printf("Read %zu: %s\n", r, buf);
		write(fifo_out, buf, r);
	}

	if (r < 0) {
		perror("Read -1");
	}

	printf("Closing...\n");

	close(fifo_in);
	close(fifo_out);
}
