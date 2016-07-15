#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/mman.h>

int main(int argc, char* argv[]) {
	int shm_fd = shm_open("/test.shm", O_CREAT | O_RDWR, 0666);

	ftruncate(shm_fd, 1<<20);

	void* ptr = mmap(0, 1<<20, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

	if (ptr == MAP_FAILED) {
		perror("Map failed");
		return -1;
	}

	char* s = ptr;
	int i;
	for (i = 0; i < 1<<20; i++) {
		*s++ = 13;
	}

	return 0;
}
