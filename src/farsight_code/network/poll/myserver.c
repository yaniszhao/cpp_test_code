#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <fcntl.h>
#include <unistd.h>
#include <poll.h>

#include <myserver.h>

int init_server(const char *ipstr, u_short port, int backlog)
{
	int s = socket(AF_INET, SOCK_STREAM, 0);
	if(0 > s){
		perror("socket");
		goto ERR_STEP;
	}

	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port   = htons(port),
		.sin_addr   = {
			.s_addr = (NULL == ipstr) ? INADDR_ANY : inet_addr(ipstr),
		}, 
	};
	memset(addr.sin_zero, 0, 8);
	socklen_t addrlen= sizeof(addr);	
	if(0 > bind(s, (struct sockaddr*)&addr, addrlen)){
		perror("bind");
		goto ERR_STEP;
	}

	if(0 > listen(s, backlog)){
		perror("listen");
		goto ERR_STEP;
	}

	return s;

ERR_STEP:
	return -1;		
}

int set_nonblock(int fd)
{
	int flag = fcntl(fd, F_GETFL);
	if(0 > flag){
		perror("fcntl");
		return -1;
	}

	flag |= O_NONBLOCK;
	return fcntl(fd, F_SETFL, flag);
}

int newconn(int s, struct pollfd *arr, const int lenmax)
{
	struct sockaddr_in c_addr;
	memset(&c_addr, 0, sizeof(struct sockaddr_in));
	socklen_t len = sizeof(struct sockaddr_in);
	int rws = accept(s, (struct sockaddr*)&c_addr, &len);
	if(0 > rws){
		perror("accept");
		return -1;
	}

	int id = rws-s;
	if(id > lenmax){
		printf("No more space to save new conn.\n");
		close(rws);
		return -1;
	}

	printf("id = %d\n", id);
	arr[id].fd = rws;

	/*设置套接字为非阻塞*/
	set_nonblock(rws);

	printf("new con[%d]: %s:%d\n", rws,\
		inet_ntoa(c_addr.sin_addr),\
		ntohs(c_addr.sin_port));

	return 0;
}

int recvdata(int rws)
{
#define BUFMAX 1024
	char buf[BUFMAX];
	bzero(buf, BUFMAX);
	int num = read(rws, buf, BUFMAX-1);
	if(0 >= num){
		printf("read [%d] fail.\n", rws);
		return -1;
	}

	printf("[socket=%d] recv[%dbytes]: %s\n", rws, num, buf);
	return num;
}

