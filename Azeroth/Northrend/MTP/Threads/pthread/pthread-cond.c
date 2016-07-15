#include <stdio.h>
#include <unistd.h>
#include <pthread.h>


pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;


void* th1_cb(void* arg) {
	puts("Chuck sleeps...");
	sleep(3);

	puts("Chuck signals!");
	pthread_cond_signal(&cond);

	puts("Chuck sleeps again...");
	sleep(3);

	puts("Chuck broadcasts!");
	pthread_cond_broadcast(&cond);

	puts("Chuck done.");

	return NULL;
}

void* th2_cb(void* arg) {
	puts("Bob awaits...");
	pthread_cond_wait(&cond, &m);
	puts("Bob woke up");

	pthread_mutex_unlock(&m);

	return NULL;
}

void* th3_cb(void* arg) {
	puts("Alice awaits...");
	pthread_cond_wait(&cond, &m);
	puts("Alice woke up");

	pthread_mutex_unlock(&m);

	return NULL;
}

void* th4_cb(void* arg) {
	puts("Frank awaits...");
	pthread_cond_wait(&cond, &m);
	puts("Frank woke up");

	pthread_mutex_unlock(&m);

	return NULL;
}


int main(int argc, char* argv[]) {
	pthread_t th1, th2, th3, th4;
	pthread_create(&th1, NULL, th1_cb, NULL);
	pthread_create(&th2, NULL, th2_cb, NULL);
	pthread_create(&th3, NULL, th3_cb, NULL);
	pthread_create(&th4, NULL, th4_cb, NULL);

	puts("Main awaits...");
	pthread_join(th1, NULL);
	pthread_join(th2, NULL);
	pthread_join(th3, NULL);
	pthread_join(th4, NULL);

	puts("Main done.");
	return 0;
}
