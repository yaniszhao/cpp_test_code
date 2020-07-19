#include "myserver.h"

//传入服务器端口号和地址，初始化并与其连接并返回套接字
int client_init_connect(unsigned short port, char *ipstr)
{
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

	return s;
}
//从套接字得到消息并打印
int recv_msg(int rws)
{
	
	char buf[RMAX];
	memset(buf, 0, RMAX);

	int num = recv(rws, buf, RMAX, 0);
	if(0 >= num){
		return -1;
	}
	
	printf("RECV[%dbytes]:%s\n", num, buf);
	
	return 0;
}
//从标准输入接收并送到套接字
int snd_msg(int rws)
{
	#define SMAX 1024
	char buf[SMAX];
	memset(buf, 0, SMAX);
	fgets(buf, SMAX, stdin);
	int len = strlen(buf);
	buf[len - 1] = '\0';//吸收回车
	int num = send(rws, buf, len, MSG_NOSIGNAL);
	if((len) != num){
		printf("snd fail. \n");
		return -1;
	}
	//printf("---sen_msg\n");

	return 0;
}