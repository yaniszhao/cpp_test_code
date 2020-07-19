#include <stdio.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>

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
			.s_addr = (NULL == ipstr) ? 0 : inet_addr(ipstr),
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

int main(int num, char **arg)
{
	unsigned short port = 9876;
	char ipstr[sizeof("255.255.255.255")] = "127.0.0.1";

	if(2 == num){
		port = atoi(arg[1]);
	}

	int s = init_server(NULL, port, 10);
	if(0 > s){
		return -1;
	}

	//set_nonblock(s);
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

	while(1){
		#define MAX 1024
		char buf[MAX];
		memset(buf, 0, MAX);

		int num = read(rws, buf, MAX-1);
		if(0 >= num){
			close(rws);
			printf("recv fail. \n");
			break;
		}

		printf("RECV[%dbytes]:%s\n", num, buf);
	}

	close(s);
	printf("--------------server.-----------------\n");

	return 0;	
}
