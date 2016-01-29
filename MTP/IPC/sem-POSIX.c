#include <fcntl.h>
#include <semaphore.h>


int main(int argc, char* argv[]) {
	sem_t* sem = sem_open("/test.sem", O_CREAT, 0666, 66);
	sem_wait(sem);
}
