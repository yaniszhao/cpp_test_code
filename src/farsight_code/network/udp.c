#include <stdio.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int init_server(const char *ipstr, u_short port)
{
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if(0 > s){
		perror("socket");
		return -1;
	}

	struct sockaddr_in s_addr = {
		.sin_family = AF_INET,
		.sin_port   = htons(port),
		.sin_addr   = {
			.s_addr = (NULL == ipstr) ? INADDR_ANY : inet_addr(ipstr),
		}, 
	};
	memset(s_addr.sin_zero, 0, 8);
	socklen_t addrlen= sizeof(s_addr);	
	if(0 > bind(s, (struct sockaddr*)&s_addr, addrlen)){
		perror("bind");
		return -1;
	}

	return s;
}

int wait_client(int s, struct sockaddr_in *c_addr, socklen_t *len)
{
	#define MAX 4
	char buf[MAX];
	int num = recvfrom(s, buf, MAX, 0, (struct sockaddr *)c_addr, len);
	if(MAX != num){
		perror("recvfrom");
		return -1;
	}

	if(strcmp(buf, "ACK")){
		printf("strcmp fail.\n");
		return -1;
	}

	return 0;
}

int main()
{
	int s = init_server("10.0.0.111", 9876);
	if(0 > s){
		return -1;
	}

	printf("Wait for client.\n");

	struct sockaddr_in c_addr;
	memset(&c_addr, 0, sizeof(c_addr));
	socklen_t len = sizeof(c_addr);
	if(0 > wait_client(s, &c_addr, &len)){
		return -1;
	}
	printf("ACK:%s:%d\n", inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));

	int i = 0;
	while(1){
		char buf[] = "hello world";
		int num = sendto(s, buf, sizeof(buf), 0, (struct sockaddr*) &c_addr, len);
		if(sizeof(buf) != num){
			perror("sendto");
			break;
		}

		printf("SND %d done.\n", i++);
		//sleep(1);
	}
	
	close(s);
	return 0;
}

/* ====================================================== */

#include <stdio.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>


int conn_server(int s, const struct sockaddr_in *s_addr, const socklen_t len)
{
	char buf[]="ACK";
	int num = sendto(s, buf, sizeof(buf), 0, (struct sockaddr *)s_addr, len);
	if(sizeof(buf) != num){
		perror("recvfrom");
		return -1;
	}

	return 0;
}

int main(int num, char *arg[])
{
	if(3 != num){
		printf("Usage: %s ipstr port \n", arg[0]);
		return -1;
	}

	u_short port = atoi(arg[2]);

	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if(0 > s){
		perror("socket");
		return -1;
	}

	struct sockaddr_in s_addr = {
		.sin_family = AF_INET,
		.sin_port   = htons(port),
		.sin_addr   = {
			.s_addr = inet_addr(arg[1]),
		}, 
	};
	memset(s_addr.sin_zero, 0, 8);
	socklen_t len= sizeof(s_addr);	
	if(0 > conn_server(s, &s_addr, len)){
		return -1;
	}
	printf("SERVER:%s:%d\n", arg[1], port);

	int i = 0;
	while(1){
		#define MAX 1024
		char buf[MAX];
		memset(buf, 0, MAX);

		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(addr));
		socklen_t len = sizeof(addr);	
		int num = recvfrom(s, buf, MAX, 0, (struct sockaddr*)&addr, &len);
		if(0 >= num){
			perror("recvf");
			break;
		}
		printf("From:%s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
		printf("RECV[%d] %dbytes : %s\n", i++, num, buf);
	}
	
	close(s);
	return 0;
}
