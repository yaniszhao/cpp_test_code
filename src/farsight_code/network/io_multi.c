1、阻塞
当做事情的条件不满足，则休眠等，直到能够完成事情或系统出现错误。

2、非阻塞
当做事情的条件不满足，则报错立即返回。
---------------------------------------------------------------
I/O多路复用
监测多个文件描述符上指定的多个事件

A、针对阻塞版
ssize_t read(int fd, void *buf, size_t count)
{
	if(可读)｛
		直接读
		返回值实际读的字节数
	｝else{
		休眠;
	}

	直接读
	返回值实际读的字节数
};

B、分拆功能
把阻塞版功能函数分拆成两个部分：wait + do something

C、如何实现
select机制
使用流程，5步：（1）、（2）、（3）、（4）、（5）

（1）描述监测对象和事件
fd_set - 集合（监测对象的集合）

操作集合的函数：
清空集合
void FD_ZERO(fd_set *set);
添加一个监测对象到集合中
void FD_SET(int fd, fd_set *set);
从集合中删除一个监测对象
void FD_CLR(int fd, fd_set *set);
测试指定文件描述符的指定事件是否发送
int  FD_ISSET(int fd, fd_set *set);

（2）通知系统
（3）等事情发生
/*
 * 功能:通知系统要等谁等什么事情，然后去等
 * 参数：
	int nfds - 最大文件描述符的值+1
	fd_set *readfds - 读集合，这个集合中的文件描述符等待是读事件
	fd_set *writefds- 写集合，这个集合中的文件描述符等待是写事件
	fd_set *exceptfds- 异常
	struct timeval *timeout - 超时时间结构
		struct timeval {
			long    tv_sec;         /* seconds */
			long    tv_usec;        /* microseconds */
		};
			- 0 非阻塞
			- 合理的时间值 超时版
			- 很大很大很大的时间值 阻塞
 * 返回值：
	失败：-1
	超时：0
	成功：>1 --->有事情发生
 */
int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);

4）轮询测试是谁发生了什么事情
for(int i = 0; i <　nfds; i++){
	if(FD_ISSET(int fd, fd_set *set)){
		（5）做事情
		看集合在参数的哪个位置？
		read/write()
	}
}

注意事项：
select的参数（集合及超时结构）要每次初始化，否则可能会造成select工作异常
---------------------------------------------------------
I/O多路复用
监测多个文件描述符上指定的多个事件
poll机制
使用流程，5步：（1）、（2）、（3）、（4）、（5）

（1）描述监测对象和事件
struct pollfd {
	int   fd;         /*监测对象 file descriptor */
	short events;     /*监测事件 requested events */
	short revents;    /*poll返回后，系统通过该域告知是否事件发生 returned events */
};
读事件宏：POLLIN
写事件宏：POLLOUT

（2）通知系统
（3）等事情发生
/*
 * 功能:通知系统要等谁等什么事情，然后去等
 * 参数：
	struct pollfd *fds - 指向监测对象及事件的结构体首地址
	nfds_t nfds - 监测对象及事件结构的数量
	int timeout - 超时时间（单位是豪秒）
		非阻塞：0
		超时版：合理的时间值
		阻塞版：很大很大的时间值
 * 返回值：
	失败：-1
	超时：0
	成功：>1 --->有事情发生
 */
int poll(struct pollfd *fds, nfds_t nfds, int timeout){
	for(int i = 0; i < nfds; i++)
		测试指定文件的事情是否发生
		if(发生){
			...
		}
	休眠
}

（4）轮询测试是谁发生了什么事情
for(int i = 0; i <　nfds; i++){
	if(POLLIN & fds[i].revents){
		(5)读事情
		read()
	}
	if(POLLOUT & fds[i].revents){
		(5)写事情
		write()
	}
}

=====================================
select/poll
差异
1、描述监测对象及事件的形式不一样
select：集合，仅仅描述监测对象，把集合放在select函数的不同参数位置，表征不同事件
poll：结构体，把对象和事件绑定了
2、监测对象及事件的管理方式
select：通过操作方法来动态管理
poll：是结构体数组，顺序存贮

共同的缺点：
监测对象及事件很多的时候，每次轮询实际发生数量较少，造成效率不高
=====================================
I/O多路复用
监测多个文件描述符上指定的多个事件
epoll机制
使用流程，5步：（1）、（2）、（3）、（4）、（5）
（1）描述监测对象和事件
/*
 * 功能：创建一个epoll
 * 参数：
	int size - 能够监测的最大对象数量（现在没有意义）
 * 返回值：
	成功：是一个特殊文件描述符
	失败：-1
 */
int epoll_create(int size);

typedef union epoll_data {
	void        *ptr;
	int          fd;
	uint32_t     u32;
	uint64_t     u64;
} epoll_data_t;

/*描述要监测对象的事件*/
struct epoll_event {
	uint32_t     events;      /* 描述事件Epoll events */
	epoll_data_t data;        /* 私有数据User data variable */
};
读事件宏：EPOLLIN
写事件宏：EPOLLOUT

（2）通知系统
/*
 * 功能：epoll通用接口
 * 参数：
	int epfd - epoll的文件描述符
	int op - 操作码
		EPOLL_CTL_ADD - 把指定监测对象及事件添加进指定的epoll
		EPOLL_CTL_DEL - 从指定的epoll把指定监测对象及事件删除
	int fd - 监测对象
	struct epoll_event *event - 监测事件
 * 返回值：
	失败：-1
 */
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

（3）等事情发生
 /*
 * 功能：等eppoll的监测结果
 * 参数：
	int epfd - epoll的文件描述符
	struct epoll_event *events - 指向保存监测事件结果的缓存首地址
	int maxevents - 要想保存的最大数量
	int timeout - 超时值（单位：毫秒）
		非阻塞：0
		阻塞版：很大很大很大的时间值
		超时版：合理的时间值
 * 返回值：
	失败：-1
	超时：0
	成功：实际发生的数量
 */
int epoll_wait(int epfd, struct epoll_event *events,
                      int maxevents, int timeout);

（4）轮询测试是谁发生了什么事情
for(int i =0; i < 实际发生的数量; i++){
	if(EPOLLIN & events[i].events){
		(5)读事件发生了
		read();
	}
	if(EPOLLOUT & events[i].events){
		(5)写事件发生了
		write();
	}
}

/* =================================================== */

#include <sys/select.h>
#include <stdio.h>

int mysleep(long um)
{
	struct timeval tm = {
		.tv_sec = 0,
		.tv_usec= um,
	};

	return select(1, NULL, NULL, NULL, &tm);	
} 

#define TMOUT (3000*1000)
int main()
{
	printf("--------1-----------\n");
	mysleep(TMOUT);
	printf("--------2-----------\n");
	mysleep(TMOUT);
	printf("--------3-----------\n");
	mysleep(TMOUT);
	printf("--------4-----------\n");
	return 0;
}

/* ============================================ */
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
#include <poll.h>

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

int recv_msg(int rws)
{
	#define RMAX 1024
	char buf[RMAX];
	memset(buf, 0, RMAX);

	int num = recv(rws, buf, RMAX, 0);
	if(0 >= num){
		return -1;
	}

	printf("RECV[%dbytes]:%s\n", num, buf);
	return 0;
}

int snd_msg(int rws)
{
	#define SMAX (1024)
	char buf[SMAX];
	memset(buf, 0, SMAX);
	fgets(buf, SMAX, stdin);
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
	char ipstr[sizeof("255.255.255.255")] = "127.0.0.1";

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

	struct pollfd pfd[] = {
		[0] = {
			.fd	= rws,		/*监测对象 file descriptor */
			.events	= POLLIN,	/*监测事件 requested events */
			.revents= 0,		/*poll返回后，系统通过该域告知是否事件发生 returned events */
		},
		[1] = {
			.fd	= 0,		/*监测对象 file descriptor */
			.events	= POLLIN,	/*监测事件 requested events */
			.revents= 0,		/*poll返回后，系统通过该域告知是否事件发生 returned events */
		},
	};

#define ARRSZ(arr) (sizeof(arr)/sizeof(arr[0]))

	int tmout = 3000;

	while(1){
		printf("Please input:\n");

		int ret = poll(pfd, ARRSZ(pfd), tmout);
		if(0 > ret){
			perror("poll");
			break;
		}else if(0 == ret){
			printf("timeout.\n");
			continue;
		}

		if(POLLIN & pfd[0].revents){
			recv_msg(rws);
		}

		if(POLLIN & pfd[1].revents){
			snd_msg(rws);
		}
	}

	close(rws);
	close(s);

	return 0;	
}
/* ================================================ */

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

int recv_msg(int rws)
{
	#define RMAX 1024
	char buf[RMAX];
	memset(buf, 0, RMAX);

	int num = recv(rws, buf, RMAX, 0);
	if(0 >= num){
		return -1;
	}

	printf("RECV[%dbytes]:%s\n", num, buf);
	return 0;
}

int snd_msg(int rws)
{
	#define SMAX (1024)
	char buf[SMAX];
	memset(buf, 0, SMAX);
	fgets(buf, SMAX, stdin);
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
	char ipstr[sizeof("255.255.255.255")] = "127.0.0.1";

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

	struct epoll_event add_evt[2] = {
		[0] = {
			.events = EPOLLIN,
			.data   = {
				.fd = rws,
			},
		},
		[1] = {
			.events = EPOLLIN,
			.data   = {
				.fd = 0,
			},
		},
	};
	if(0 > epoll_ctl(efd, EPOLL_CTL_ADD, rws, &add_evt[0])){
		perror("epoll_ctl add");
		return -1;
	}
	if(0 > epoll_ctl(efd, EPOLL_CTL_ADD, 0, &add_evt[1])){
		perror("epoll_ctl add");
		return -1;
	}

	int tmout = 3000;

#define WAITMAX 2
	struct epoll_event wait_evt[WAITMAX];

	while(1){
		printf("Please input:\n");
		int num = epoll_wait(efd, wait_evt, WAITMAX, tmout);
		if(0 > num){
			perror("epoll_wait");
			break;
		}else if(0 == num){
			printf("Timeout.\n");
			continue;
		}

		for(int i = 0; i < num; i++){
			if(rws == wait_evt[i].data.fd){
				if(0 > recv_msg(rws)){
					break;
				}
			}else if(0 == wait_evt[i].data.fd){
				if(0 > snd_msg(rws)){
					break;
				}
			}
		}
	}

	close(rws);
	close(s);

	return 0;	
}
