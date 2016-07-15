#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>


int main(int argc, char* argv[]) {
	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 8192;

	mqd_t mq = mq_open("/test.mq", O_RDONLY | O_CREAT, 0666, &attr);

	char buf[8192];
	int result = mq_receive(mq, buf, 8192, 0);

	int fd = open("message.txt", O_WRONLY | O_CREAT, 0666);
	write(fd, buf, result);
	close(fd);

	mq_unlink("/test.mq");

	return 0;
}
