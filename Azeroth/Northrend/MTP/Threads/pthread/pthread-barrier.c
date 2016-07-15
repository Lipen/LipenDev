#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


pthread_barrier_t bp;


void* th1_cb(void* arg) {
	puts("Thread 1 awaits barrier...");
	pthread_barrier_wait(&bp);

	puts("Thread 1 done");
	return NULL;
}

void* th2_cb(void* arg) {
	puts("Thread 2 sleeps...");
	sleep(3);

	puts("Thread 2 awaits barrier...");
	pthread_barrier_wait(&bp);

	puts("Thread 2 done");
	return NULL;
}


int main(int argc, char* argv[]) {
	pthread_barrier_init(&bp, NULL, 2);

	pthread_t th1, th2;
	pthread_create(&th1, NULL, th1_cb, NULL);
	pthread_create(&th2, NULL, th2_cb, NULL);

	puts("Main awaits...");
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);

	puts("Main done.");
	return 0;
}
