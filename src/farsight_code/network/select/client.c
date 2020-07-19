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

int main(int num, char **arg)
{
	unsigned short port = 9999;
	char ipstr[IPMAX]   = "127.0.0.1";
	char *msg = "hello world";

	if(4 == num){
		strncpy(ipstr, arg[1], IPMAX);
		port = atoi(arg[2]);
		msg = arg[3];
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

	signal(SIGPIPE, SIG_IGN);
	shutdown(s, SHUT_RD);

	int i = 0;
	while(1){
	#define MAX 1024
		char buf[MAX];
		snprintf(buf, MAX, "NO.%d:%s", i++, msg);
		int len = strlen(buf);
		int num = send(s, buf, len, 0);
		if(len != num){
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
