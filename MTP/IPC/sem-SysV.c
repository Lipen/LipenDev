#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <semaphore.h>

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};

int main(int argc, char *argv[]) {
	key_t key = ftok("/tmp/sem.temp", 0);
	int semid = semget(key, 16, IPC_CREAT | 0666);

	printf("semid = %d\n", semid);
	if (semid < 0) {
		perror("Bad semid");
	}

	int i, result;
	for (i = 0; i < 16; i++) {
		union semun arg;
		arg.val = i;
		result = semctl(semid, i, SETVAL, arg);
		if (result < 0) {
			perror("Bad set");
		}
	}

	printf("Sleeping for 5 secs...");
	sleep(5);
	printf("\n");

	result = semctl(semid, 0, IPC_RMID);
	if (result < 0) {
		perror("Bad remove");
	}

	return 0;
}
