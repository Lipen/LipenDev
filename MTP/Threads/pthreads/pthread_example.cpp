#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

static void* th_cb(void* arg) {
	FILE* f = fopen("main.pid", "w");
	fprintf(f, "%d", getpid());
	fclose(f);
	printf("The pid is %d.\n", getpid());

	while (1);

	return NULL;
}

int main(int argc, char* argv[]) {
	pthread_t th;

	pthread_create(&th, NULL, th_cb, NULL);

	pthread_join(th, NULL);
}
