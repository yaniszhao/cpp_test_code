#include "myserver.h"

int rws[RWSSIZE];//读写套接字数组，大小可以改，放在静态区不仅开线程的时候不用传参，在函数里也可以直接调用
int rws_num = 0;//存在套接字的个数


int main(int argc, char *argv[])
{	//初始化
	unsigned short port = 9876;
	char ipstr[sizeof("255.255.255.255")] = "127.0.0.1";
	if(2 == argc){
		port = atoi(argv[1]);
	}
	int s;
	if(0 > (s = init_server(NULL, port, 10))){
		return -1;
	}
	printf("Waiting for client.\n");

	
	//初始化线程要用的参数
	int tmp = RWSSIZE;
	while (tmp--) 	//将rws值全部变成-1，方便判断后面rws[]
		rws[tmp] = -1;
	int *s_socket;
	s_socket = &s;//server套接字的地址
	
	//初始化IO多路复用
	int efd;
	if(0 > (efd = epoll_create(1))){
		perror("epoll_create");
		return -1;
	}
	struct epoll_event add_evt[RWSSIZE];
	tmp = RWSSIZE;
	while(tmp--) //初始化套接字为-1，以方便判断下面是否加入IO复用
			add_evt[tmp].data.fd = -1;
	//printf("add_evt[tmp].data.fd=%d\n", add_evt[0].data.fd);
	
	wait_accept_first((void *)s_socket);//等一次，避免主线程找不到套接字空跑
	
	pthread_t tid;//开启线程等其他用户连接
	tid = pthread_create(&tid, NULL, wait_accept, (void *)s_socket);
	
	
	
	int whilenum = 0;//下面第二个while循环次数	
	while(1){
		printf("there ara %d people!\n", rws_num);
		
		//rws_size = get_rws_size(rws);//每次更新连接，得到实际连接的读写套接字个数
		//因为把rws_num用做了全局变量可以省略上面语句
		
		whilenum = rws_num;//没有任何用户时，不会执行到这里，所以最小值为1.
		//printf("---size rws is %d---\n", rws_size);//调试用
		while(whilenum--){		//初始化epoll
			if (-1 == add_evt[whilenum].data.fd){//之前加入的不能加入，否则要报错
				add_evt[whilenum].events = EPOLLIN;
				add_evt[whilenum].data.fd = rws[whilenum];
				if(0 > (epoll_ctl(efd, EPOLL_CTL_ADD, rws[whilenum], &add_evt[whilenum]))){
					perror("epoll_ctl add");
					return -1;
				}
			}
		}
		//等读写套接字发消息
		struct epoll_event wait_evt[rws_num];
		
		//int tmout = 3000;//必须要设置延迟，否则卡在这儿，即使有用户增加了也无法及时知道
		//设置为宏
#define TMOUT 3000		
		//while(1){
			int wait_num = epoll_wait(efd, wait_evt, rws_num, TMOUT);
			if(0 > wait_num){
				perror("epoll_wait");
				continue;
			}else if(0 == wait_num){
				//printf("Timeout.\n");
				//goto loop;
				continue;
			} else {
				int use_rws;//等到的套接字
				int i;
				for(i = 0; i < wait_num; i++){					
					use_rws = wait_evt[i].data.fd;//得到发来消息的套接字
					int rws_index = 0;//存储client想连接目标的下标
					//printf("--index--%d-----\n", rws_index);//调试用
					
					rws_index = get_index(use_rws);
					if(0 > rws_index || 9 < rws_index){//接受得到的数字
						//perror("get_rws_index-111");
						char err_index[] = "please input [0-9]:";
						int len = strlen(err_index);//接收的数字不是0-9，发送错误消息						
						if(0 >= send(use_rws, err_index, len+1, MSG_NOSIGNAL)){
							perror("get_rws_index");
						}
						//goto loop;
						break;
					} 
					
					//printf("--index--%d-----2\n", rws_index);//调试用
					if(-1 == rws[rws_index]){//根据数字判断对象是否存在
						//printf("====don't have===1111");
						char text[RMAX];
						sprintf(text, "don't have this person!");
						int text_len = strlen(text);
						if ((text_len + 1) != send(use_rws, text, text_len + 1, MSG_NOSIGNAL)){
							perror("send_index_error");
							//goto loop;
							break;
						}
						//printf("====don't have===2222");
					} else {
						char text[RMAX];//发送消息
						sprintf(text, "you want to connect [%d],please type your text:", rws_index);
						int text_len = strlen(text);
						if ((text_len + 1) != send(use_rws, text, text_len + 1, MSG_NOSIGNAL)){
							perror("send__use_rws");
							//goto loop;
							break;
						}
						if(0 > recv_msg_send(use_rws, rws_index, rws)){
							perror("recv_msg_send");
							//goto loop;
							break;
						}
					}		
				}
			}
		//}
	//loop:;//结束本次循环
	//将主要部分加入if-else省略此处
	}

	return 0;
}