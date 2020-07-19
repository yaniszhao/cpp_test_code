#include "myserver.h"
int main(int argc, char *argv[])
{
	unsigned short port = 9876;
	char ipstr[IPMAX]   = "127.0.0.1";

	if(3 == argc){
		strncpy(ipstr, argv[1], IPMAX);
		port = atoi(argv[2]);
	}
	int s;//初始化和连接
	if(0 > (s = client_init_connect(port, ipstr))){
		perror("client_init_connect");
		return -1;
	}
	//IO多路复用，等标准输入和服务器的响应
	int efd;
	if(0 > (efd = epoll_create(1))){
		perror("epoll_create");
		return -1;
	}
	
	struct epoll_event add_evt[2] = {//epoll初始化
		[0] = {
			.events = EPOLLIN,
			.data = {
				.fd = 0,
			},
		},
		[1] = {
			.events = EPOLLIN,
			.data = {
				.fd = s,
			},
		},
	};
	if(0 > (epoll_ctl(efd, EPOLL_CTL_ADD, 0, &add_evt[0]))){
			perror("epoll_ctl add_0");
			return -1;
	}
	if(0 > (epoll_ctl(efd, EPOLL_CTL_ADD, s, &add_evt[1]))){
			perror("epoll_ctl add_1");
			return -1;
	}
	struct epoll_event wait_evt[2];//设置等操作
#define TMOUT 3000
	
	while(1){
		int wait_num = epoll_wait(efd, wait_evt, 2, TMOUT);
		if(0 > wait_num){
			perror("epoll_wait");
			break;
		}else if(0 == wait_num){
			//printf("Timeout.\n");
			continue;
		}
		int i;
		for(i = 0; i < wait_num; i++){
			//printf("-----%d------\n", i);
			if(s == wait_evt[i].data.fd){
				if(0 > recv_msg(s)){
					break;
				}
			}else if(0 == wait_evt[i].data.fd){
				if(0 > snd_msg(s)){
					break;
				}
			}
		}
	}
	
	close(s);
	return 0;
}