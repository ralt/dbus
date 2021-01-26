#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int receive_fds(int sock)
{
	struct msghdr msg;
	struct cmsghdr *cmsghdr;
	struct iovec iov[1];
	ssize_t nbytes;
	int *p;
	char buf[CMSG_SPACE(sizeof(int))], c;

	iov[0].iov_base = &c;
	iov[0].iov_len = sizeof(c);
	memset(buf, 0x0d, sizeof(buf));
	cmsghdr = (struct cmsghdr *)buf;
	cmsghdr->cmsg_len = CMSG_LEN(sizeof(int));
	cmsghdr->cmsg_level = SOL_SOCKET;
	cmsghdr->cmsg_type = SCM_RIGHTS;
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_iov = iov;
	msg.msg_iovlen = sizeof(iov) / sizeof(iov[0]);
	msg.msg_control = cmsghdr;
	msg.msg_controllen = CMSG_LEN(sizeof(int));
	msg.msg_flags = 0;

	nbytes = recvmsg(sock, &msg, 0);
	if (nbytes == -1)
		return -1;

	p = (int *)CMSG_DATA(cmsghdr);
	return *p;
}
