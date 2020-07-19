#include "myserver.h"
#define RMAX 1024
extern int rws[RWSSIZE];
extern int rws_num;
//初始化服务器
int init_server(const char *ipstr, u_short port, int backlog)
{
	int s;
	if (0 > (s = socket(AF_INET, SOCK_STREAM, 0))) {
		perror("tcp_server_socket");
		exit(-1);
	}

	struct sockaddr_in addr = {
		.sin_family = AF_INET,
		.sin_port   = htons(port),
		.sin_addr   = {
			.s_addr = (NULL == ipstr) ? 0 : inet_addr(ipstr),
		}, 
	};	
	memset(addr.sin_zero, 0, 8);
	
	socklen_t addrlen = sizeof(addr);	
	if (0 > bind(s, (struct sockaddr*)&addr, addrlen)) {
		perror("tcp_server_bind");
		exit(-1);
	}

	if (0 > listen(s, backlog)) {
		perror("listen");
		exit(-1);
	}

	return s;		
}

//在wait_accept使用等待client连接，如果连接则打印让client想连接的对象
int myaccept(int s)
{
	struct sockaddr_in c_addr;
	memset(&c_addr, 0, sizeof(c_addr));
	socklen_t len = sizeof(c_addr);
	int rws = accept(s, (struct sockaddr*)&c_addr, &len);
	int num = 0; 
	char buf[] = "connect is successful!";
	int lenth = strlen(buf);
	if (0 < rws) {
		num = send(rws, buf, lenth + 1, MSG_NOSIGNAL);
	}
	if((lenth + 1) != num){
		printf("myaccept error!\n");
		return -1;
	}
	return rws;
}

//在线程开启之前等一次
void *wait_accept_first(void *s_socket) //参数第一个为服务器的套接字的地址，第二个为读写套接字指针
{	
	if (0 < (rws[0] = myaccept(*(int *)s_socket))){		
		rws_num++;
		char buf[RMAX] = {0};
		sprintf(buf, "there ara %d people, input who you want to connect:", rws_num );		
		int lenth = strlen(buf);
		sleep(1);
		int num = send(rws[0], buf, lenth + 1, MSG_NOSIGNAL);
		if((lenth + 1) != num){
			perror("wait_accept_1");
			exit(0);
		}
	} else {
		perror("wait_accept_2");
		exit(0);
	}
	
	return NULL;
}
//在线程里等待client的连接
void *wait_accept(void *s_socket) //参数第一个为服务器的套接字的地址，第二个为读写套接字指针
{	
	int tmp_rws = 0;
	while (1){	
		if (RWSSIZE == rws_num){//判断用户是否满了
			printf("rws_num is full!\n");
			break;
		}	
		
		if (0 < (tmp_rws = myaccept(*(int *)s_socket))){
			int i = RWSSIZE;
			while (i--) {
				if (-1 == rws[9 - i]){
					rws[9 - i] = tmp_rws;
					rws_num++; //得到用户个数，因为有第一次的连接，故初始值为1
					break;
				}				
			}
					
			char buf[RMAX] = {0};
			sprintf(buf, "there ara %d people, input who you want to connect:", rws_num );
			//printf("%s", buf);
			int lenth = strlen(buf);
			sleep(1);
			int num = send(tmp_rws, buf, lenth+1, MSG_NOSIGNAL);
			if((lenth+1) != num){
				perror("wait_accept_1");
				exit(0);
			}
		} else {//注意这个else接的是while下的if，如果少写一个'{'就会变成上面一个的else。
			perror("wait_accept_2");
			exit(0);
		}
		//printf("=======2=======\n");
	}
	return NULL;
}

/*
//得到实际连接的读写套接字个数
int get_rws_size(int *rws)
{
	int i = RWSSIZE;
	while (i--) {
		if (rws[i-1] != -1)
			return i;
	}
	return -1;
}
*/

//得到想连接的下标
int get_index(int rws)
{
	char buf[2] = {0};
	int num = recv(rws, buf, 2, 0);
	if(0 >= num){
		return -1;
	}
	int tmp = (int)buf[0] - 48;
	//num = recv(rws, buf, 2, 0);//吸收回车
	//printf("%d\n", tmp);//调试
	return tmp;
}

//从第一个套接字接受消息，发送到第二个套接字
int recv_msg_send(int rws_s, int rws_index, int *rws)
{
	
	char buf[RMAX];
	memset(buf, 0, RMAX);
	int num;
	if(0 >= (num = recv(rws_s, buf, RMAX, 0))){
		return -1;
	}
	//printf("------recv-msg-send-------111\n");
	int len = strlen(buf);
	if(0 >= (num = send(rws[rws_index], buf, len+1, MSG_NOSIGNAL))){
		return -1;
	}
	//printf("------recv-msg-send-------222\n");
	return 0;
}