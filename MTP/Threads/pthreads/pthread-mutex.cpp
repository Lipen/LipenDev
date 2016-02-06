#include <cstdio>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t m;
pthread_spinlock_t sl;
pthread_rwlock_t rl;
pthread_rwlock_t wl;


void* waiter_cb(void* arg) {
	pthread_mutex_lock(&m);
	pthread_mutex_unlock(&m);
	return NULL;
}

void* spin_cb(void* arg) {
	while (pthread_spin_trylock(&sl));
	// pthread_spin_unlock(&sl);
	return NULL;
}

void* reader_cb(void* arg) {
	pthread_rwlock_rdlock(&rl);
	pthread_rwlock_unlock(&rl);
	return NULL;
}

void* writer_cb(void* arg) {
	pthread_rwlock_wrlock(&wl);
	pthread_rwlock_unlock(&wl);
	return NULL;
}

int main(int argc, char* argv[]) {
	FILE* f = fopen("main.pid", "w");
	fprintf(f, "%d\n", getpid());
	fclose(f);

	printf("Did you know that pid is %d?\n", getpid());

	// Initialize all threads
	pthread_mutex_init(&m, NULL);
	pthread_spin_init(&sl, PTHREAD_PROCESS_SHARED);
	pthread_rwlock_init(&rl, NULL);
	pthread_rwlock_init(&wl, NULL);

	// Initially lock all mutexes
	pthread_mutex_lock(&m);
	pthread_spin_lock(&sl);
	pthread_rwlock_rdlock(&rl);
	pthread_rwlock_wrlock(&wl);

	// Create threads
	pthread_t th_waiter, th_spin, th_reader, th_writer;

	pthread_create(&th_waiter, NULL, waiter_cb, NULL);
	pthread_create(&th_spin, NULL, spin_cb, NULL);
	pthread_create(&th_reader, NULL, reader_cb, NULL);
	pthread_create(&th_writer, NULL, writer_cb, NULL);

	// Wait for threads to end
	pthread_join(th_writer, NULL);
	pthread_join(th_spin, NULL);
	pthread_join(th_reader, NULL);
	pthread_join(th_writer, NULL);
}
