#include <stdio.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define IPMAX sizeof("255.255.255.255")

void handler(int sig)
{
	printf("----pipe:%d-----\n", sig);
}

int main(int num, char **arg)
{
	unsigned short port = 9876;
	char ipstr[IPMAX]   = "127.0.0.1";

	if(3 == num){
		strncpy(ipstr, arg[1], IPMAX);
		port = atoi(arg[2]);
	}

	int s = socket(AF_INET, SOCK_STREAM, 0);
	if(0 > s){
		perror("socket");
		return -1;
	}

	struct sockaddr_in s_addr = {
		.sin_family = AF_INET,
		.sin_port   = htons(port),
		.sin_addr   = {
			.s_addr = inet_addr(ipstr),
		}, 
	};
	memset(s_addr.sin_zero, 0, 8);
	socklen_t len = sizeof(s_addr);

	if(0 > connect(s, (struct sockaddr*)&s_addr, len)){
		perror("connect");
		return -1;
	}

	signal(SIGPIPE, handler);

	while(1){
		char buf[] = "hello world";
		int num = write(s, buf, sizeof(buf));
		if(sizeof(buf) != num){
			close(s);
			printf("snd fail. \n");
			break;
		}
	
		printf("SND[%dbytes]:%s\n", num, buf);

		sleep(1);
	}

	close(s);
	printf("--------------client.-----------------\n");

	return 0;	
}
