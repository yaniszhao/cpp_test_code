1������
������������������㣬�����ߵȣ�ֱ���ܹ���������ϵͳ���ִ���

2��������
������������������㣬�򱨴��������ء�
---------------------------------------------------------------
I/O��·����
������ļ���������ָ���Ķ���¼�

A�����������
ssize_t read(int fd, void *buf, size_t count)
{
	if(�ɶ�)��
		ֱ�Ӷ�
		����ֵʵ�ʶ����ֽ���
	��else{
		����;
	}

	ֱ�Ӷ�
	����ֵʵ�ʶ����ֽ���
};

B���ֲ���
�������湦�ܺ����ֲ���������֣�wait + do something

C�����ʵ��
select����
ʹ�����̣�5������1������2������3������4������5��

��1��������������¼�
fd_set - ���ϣ�������ļ��ϣ�

�������ϵĺ�����
��ռ���
void FD_ZERO(fd_set *set);
���һ�������󵽼�����
void FD_SET(int fd, fd_set *set);
�Ӽ�����ɾ��һ��������
void FD_CLR(int fd, fd_set *set);
����ָ���ļ���������ָ���¼��Ƿ���
int  FD_ISSET(int fd, fd_set *set);

��2��֪ͨϵͳ
��3�������鷢��
/*
 * ����:֪ͨϵͳҪ��˭��ʲô���飬Ȼ��ȥ��
 * ������
	int nfds - ����ļ���������ֵ+1
	fd_set *readfds - �����ϣ���������е��ļ��������ȴ��Ƕ��¼�
	fd_set *writefds- д���ϣ���������е��ļ��������ȴ���д�¼�
	fd_set *exceptfds- �쳣
	struct timeval *timeout - ��ʱʱ��ṹ
		struct timeval {
			long    tv_sec;         /* seconds */
			long    tv_usec;        /* microseconds */
		};
			- 0 ������
			- �����ʱ��ֵ ��ʱ��
			- �ܴ�ܴ�ܴ��ʱ��ֵ ����
 * ����ֵ��
	ʧ�ܣ�-1
	��ʱ��0
	�ɹ���>1 --->�����鷢��
 */
int select(int nfds, fd_set *readfds, fd_set *writefds,
                  fd_set *exceptfds, struct timeval *timeout);

4����ѯ������˭������ʲô����
for(int i = 0; i <��nfds; i++){
	if(FD_ISSET(int fd, fd_set *set)){
		��5��������
		�������ڲ������ĸ�λ�ã�
		read/write()
	}
}

ע�����
select�Ĳ��������ϼ���ʱ�ṹ��Ҫÿ�γ�ʼ����������ܻ����select�����쳣
---------------------------------------------------------
I/O��·����
������ļ���������ָ���Ķ���¼�
poll����
ʹ�����̣�5������1������2������3������4������5��

��1��������������¼�
struct pollfd {
	int   fd;         /*������ file descriptor */
	short events;     /*����¼� requested events */
	short revents;    /*poll���غ�ϵͳͨ�������֪�Ƿ��¼����� returned events */
};
���¼��꣺POLLIN
д�¼��꣺POLLOUT

��2��֪ͨϵͳ
��3�������鷢��
/*
 * ����:֪ͨϵͳҪ��˭��ʲô���飬Ȼ��ȥ��
 * ������
	struct pollfd *fds - ָ��������¼��Ľṹ���׵�ַ
	nfds_t nfds - �������¼��ṹ������
	int timeout - ��ʱʱ�䣨��λ�Ǻ��룩
		��������0
		��ʱ�棺�����ʱ��ֵ
		�����棺�ܴ�ܴ��ʱ��ֵ
 * ����ֵ��
	ʧ�ܣ�-1
	��ʱ��0
	�ɹ���>1 --->�����鷢��
 */
int poll(struct pollfd *fds, nfds_t nfds, int timeout){
	for(int i = 0; i < nfds; i++)
		����ָ���ļ��������Ƿ���
		if(����){
			...
		}
	����
}

��4����ѯ������˭������ʲô����
for(int i = 0; i <��nfds; i++){
	if(POLLIN & fds[i].revents){
		(5)������
		read()
	}
	if(POLLOUT & fds[i].revents){
		(5)д����
		write()
	}
}

=====================================
select/poll
����
1�������������¼�����ʽ��һ��
select�����ϣ��������������󣬰Ѽ��Ϸ���select�����Ĳ�ͬ����λ�ã�������ͬ�¼�
poll���ṹ�壬�Ѷ�����¼�����
2���������¼��Ĺ���ʽ
select��ͨ��������������̬����
poll���ǽṹ�����飬˳�����

��ͬ��ȱ�㣺
�������¼��ܶ��ʱ��ÿ����ѯʵ�ʷ����������٣����Ч�ʲ���
=====================================
I/O��·����
������ļ���������ָ���Ķ���¼�
epoll����
ʹ�����̣�5������1������2������3������4������5��
��1��������������¼�
/*
 * ���ܣ�����һ��epoll
 * ������
	int size - �ܹ���������������������û�����壩
 * ����ֵ��
	�ɹ�����һ�������ļ�������
	ʧ�ܣ�-1
 */
int epoll_create(int size);

typedef union epoll_data {
	void        *ptr;
	int          fd;
	uint32_t     u32;
	uint64_t     u64;
} epoll_data_t;

/*����Ҫ��������¼�*/
struct epoll_event {
	uint32_t     events;      /* �����¼�Epoll events */
	epoll_data_t data;        /* ˽������User data variable */
};
���¼��꣺EPOLLIN
д�¼��꣺EPOLLOUT

��2��֪ͨϵͳ
/*
 * ���ܣ�epollͨ�ýӿ�
 * ������
	int epfd - epoll���ļ�������
	int op - ������
		EPOLL_CTL_ADD - ��ָ���������¼���ӽ�ָ����epoll
		EPOLL_CTL_DEL - ��ָ����epoll��ָ���������¼�ɾ��
	int fd - ������
	struct epoll_event *event - ����¼�
 * ����ֵ��
	ʧ�ܣ�-1
 */
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);

��3�������鷢��
 /*
 * ���ܣ���eppoll�ļ����
 * ������
	int epfd - epoll���ļ�������
	struct epoll_event *events - ָ�򱣴����¼�����Ļ����׵�ַ
	int maxevents - Ҫ�뱣����������
	int timeout - ��ʱֵ����λ�����룩
		��������0
		�����棺�ܴ�ܴ�ܴ��ʱ��ֵ
		��ʱ�棺�����ʱ��ֵ
 * ����ֵ��
	ʧ�ܣ�-1
	��ʱ��0
	�ɹ���ʵ�ʷ���������
 */
int epoll_wait(int epfd, struct epoll_event *events,
                      int maxevents, int timeout);

��4����ѯ������˭������ʲô����
for(int i =0; i < ʵ�ʷ���������; i++){
	if(EPOLLIN & events[i].events){
		(5)���¼�������
		read();
	}
	if(EPOLLOUT & events[i].events){
		(5)д�¼�������
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
			.fd	= rws,		/*������ file descriptor */
			.events	= POLLIN,	/*����¼� requested events */
			.revents= 0,		/*poll���غ�ϵͳͨ�������֪�Ƿ��¼����� returned events */
		},
		[1] = {
			.fd	= 0,		/*������ file descriptor */
			.events	= POLLIN,	/*����¼� requested events */
			.revents= 0,		/*poll���غ�ϵͳͨ�������֪�Ƿ��¼����� returned events */
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
