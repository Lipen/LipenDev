ssize_t sock_fd_write(int sock, void *buf, ssize_t buflen, int fd) {
	ssize_t size;
	struct msghdr msg;
	struct iovec iov;
	union {
		struct cmsghdr  cmsghdr;
		char        control[CMSG_SPACE(sizeof (int))];
	} cmsgu;
	struct cmsghdr *cmsg;

	iov.iov_base = buf;
	iov.iov_len = buflen;

	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;

	if (fd != -1) {
		msg.msg_control = cmsgu.control;
		msg.msg_controllen = sizeof(cmsgu.control);

		cmsg = CMSG_FIRSTHDR(&msg);
		cmsg->cmsg_len = CMSG_LEN(sizeof (int));
		cmsg->cmsg_level = SOL_SOCKET;
		cmsg->cmsg_type = SCM_RIGHTS;

		printf ("passing fd %d\n", fd);
		*((int*) CMSG_DATA(cmsg)) = fd;
	} else {
		msg.msg_control = NULL;
		msg.msg_controllen = 0;
		printf ("not passing fd\n");
	}

	size = sendmsg(sock, &msg, 0);

	if (size < 0)
		perror ("sendmsg");
	return size;
}
