#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <fcntl.h>


struct message {
	long mtype;
	char mtext[80];
};


int main(int argc, char* argv[]) {
	key_t key = ftok("/tmp/msg.temp", 0);
	int msqid = msgget(key, 0666 | IPC_CREAT);

	struct message snd_msg;
	snd_msg.mtype = 1;
	strcpy(snd_msg.mtext, "Hello?");
	if (msgsnd(msqid, &snd_msg, 80, 0) < 0) {
		perror("msgsnd");
	}

	struct message rcv_msg;
	if (msgrcv(msqid, &rcv_msg, 80, 0, 0) < 0) {
		perror("msgrcv");
	}

	FILE* f = fopen("message.txt", "w");
	fprintf(f, "%s\n", rcv_msg.mtext);
	printf("%s\n", rcv_msg.mtext);
	fclose(f);

	return 0;
}
