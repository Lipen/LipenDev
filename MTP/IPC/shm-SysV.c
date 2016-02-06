#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char* argv[]) {
	key_t key = ftok("/tmp/mem.temp", 0);
	int shmid = shmget(key, 1048576, IPC_CREAT | 0666);
	char* shm = shmat(shmid, NULL, 0);

	if (shmid < 0) {
		perror("shmget");
		return 1;
	}
	if (shm == (char*) -1) {
		perror("shmat");
		return 2;
	}

	char* s = shm;
	int i;
	for (i = 0; i < 1048576; i++) {
		*s++ = 42;
	}

	return 0;
}
