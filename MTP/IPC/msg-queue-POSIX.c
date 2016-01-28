#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>


int main(int argc, char* argv[]) {
	struct mq_attr attr;
	attr.flags = 0;
	attr.mq_maxmsg = 32;
	attr.mq_msgsize = 512;

	mqd_t mq = mq_open("/test.mq", O_RDONLY | O_CREAT, 0666, &attr);

	char buf[512];
	int result = mq_recieve(mq, buf, 512, 0);

	int fd = open("message.txt", O_WRONLY | O_CREAT);
	write(fd, buf, result);
	close(fd);

	return 0;
}
