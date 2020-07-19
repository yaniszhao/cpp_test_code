#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/epoll.h>
#include "myepoll.h"

int init_server(const char *ipstr, u_short port, int backlog)
{
	int s = socket(AF_INET, SOCK_STREAM, 0);
	if(0 > s){
		perror("socket");
		return -1;
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
		return -1;
	}

	if(0 > listen(s, backlog)){
		perror("listen");
		return -1;
	}

	return s;		
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

int recv_msg(int epfd, void *arg)
{
	#define RMAX 1024
	char buf[RMAX];
	memset(buf, 0, RMAX);

	int rws = (int)(arg);

	int num = recv(rws, buf, RMAX, 0);
	if(0 >= num){
		printf("recv[%d] fail.\n", rws);
		return -1;
	}

	printf("RECV[%dbytes]:%s\n", num, buf);
	return 0;
}

int snd_msg(int epfd, void *arg)
{
	#define SMAX (1024)
	char buf[SMAX];
	memset(buf, 0, SMAX);
	if(NULL == fgets(buf, SMAX, stdin)){
		perror("fgets");
		return -1;
	}

	int rws = (int)arg;

	int len = strlen(buf);
	int num = send(rws, buf, len+1, MSG_NOSIGNAL);
	if((len+1) != num){
		printf("snd fail. \n");
		return -1;
	}

	return 0;
}

int main(int num, char **arg)
{
	unsigned short port = 9876;

	if(2 == num){
		port = atoi(arg[1]);
	}

	int s = init_server(NULL, port, 10);
	if(0 > s){
		return -1;
	}

	printf("Wait for a new comming.\n");

	struct sockaddr_in c_addr;
	memset(&c_addr, 0, sizeof(c_addr));
	socklen_t len = sizeof(c_addr);
	int rws = accept(s, (struct sockaddr*)&c_addr, &len);
	if(0 > rws){
		perror("accept");
		return -1;
	}
	printf("Newcomming:%s:%d\n", inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));

	set_nonblock(rws);
	set_nonblock(0);

	int efd = epoll_create(1);
	if(0 > efd){
		perror("epoll_create");
		return -1;
	}

#define EVTMAX 2
	MYEVENT evt[EVTMAX];

	init_myevent(&evt[0], rws, EPOLLIN, recv_msg, (void *)rws);
	if(0 > add_myevent(efd, &evt[0])){
		printf("add_myevent err.\n");
		return -1;
	}
	init_myevent(&evt[1], 0, EPOLLIN, snd_msg, (void *)rws);
	if(0 > add_myevent(efd, &evt[1])){
		printf("add_myevent err.\n");
		return -1;
	}

	int tmout = 3000;
	struct epoll_event events[EVTMAX];

	while(1){
		printf("Please input:\n");
		int num = epoll_wait(efd, events, EVTMAX, tmout);
		if(0 > num){
			perror("epoll_wait");
			break;
		}else if(0 == num){
			printf("Timeout.\n");
			continue;
		}

		for(int i = 0; i < num; i++){
			MYEVENT *evp = events[i].data.ptr;
			if(0 > evp->handler(efd, evp->arg)){
				return -1;
			}
		}
	}

	close(rws);
	close(s);

	return 0;	
}
