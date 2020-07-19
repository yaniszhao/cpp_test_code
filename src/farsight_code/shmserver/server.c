#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "myshm.h"

typedef struct msghead{
    char type;	/*类型(ACSII)：'0'～视频数据；'1'～温湿度数据*/
	char len[5];	/*len：数据长度*/
}MSGHEAD;

static int mypow(int len)
{
	int i = 0;
	int num = 1;
	for(i = 0; i<len; i++) {
		num = num *10;	
	}
	return num;
}

int int_switch_char(int num, char *dest, int len)
{
	int i = 0;
	char a  = '\0';
	int ret = 0;
	int bun = 0;
	for (i = 0; i < len; i++) {
		bun = mypow(len-i-1);
		ret = num/bun;
		num = num - ret*bun;
		dest[i] = ret + '0';
	}
		return 0;
}

int main()
{
	struct shm_id s_shm_id;
	s_shm_id = shm_create();
	
	void *shared_memory = NULL;
	shared_memory = shmat(s_shm_id.shmid, (void *)0, 0);
	if ((void *)-1 == shared_memory) {
		perror("shmat");		
		exit(1);
	}
	printf("Memory attached at %p\n", shared_memory);
	struct shm_buff *shm_buff_inst;
	shm_buff_inst = (struct shm_buff *)shared_memory;
	
	//
	char ip[sizeof("255.255.255.255")] = "192.168.1.159";
	int port = 8888;

	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if(0 > sfd)
	{
		perror("socket");
		return -1;
	}

	struct sockaddr_in s_addr= {
		.sin_family = AF_INET,
		.sin_port = htons(port),
		.sin_addr = {
			.s_addr = inet_addr(ip),
			},
	};
	memset(s_addr.sin_zero, 0, 8);

	if (0 > bind(sfd, (struct sockaddr *)&s_addr, sizeof(struct sockaddr)))
	{
		perror("bind");
		return -1;
	}

	if (0 > listen(sfd, 10))
	{
		perror("listen");
		return -1;
	}

	struct sockaddr_in c_addr;
	memset(&c_addr, 0, sizeof(c_addr));
	socklen_t len = sizeof(c_addr);
	printf("try..\n");
	int  s = accept(sfd, (struct sockaddr *)&c_addr, &len);
	if(0 > s)
	{
		perror("accept");
		return -1;
	}
	printf("connect success...\n");
#define MAX 1024
char buf[MAX]="{tempe:24,humid:36}";
	
	MSGHEAD head = {
		.type = '1',
		.len[0] = '0',
		.len[1] = '0',
		.len[2] = '0',
		.len[3] = '2',
		.len[4] = '0',
	};

#if 0
	int fd = open("/home/ccc/test/1.jpg", O_RDONLY);
	if (0 > fd)
	{
		perror("open");
		return -1;
	}
#define IMAGMAX 14650
	char imagbuf[IMAGMAX];
	memset(imagbuf, 0, IMAGMAX);
	 ssize_t imaglen = read( fd, imagbuf, IMAGMAX);
	 printf("read image %d\n", imaglen);
#endif
#define IMAGLEN 30720 
	MSGHEAD imaghead = {
		.type = '0',
		.len[0] = '0',
		.len[1] = '0',
		.len[2] = '0',
		.len[3] = '0',
		.len[4] = '0',
		};
#if 0
	write(s, &imaghead, sizeof(imaghead));
	write(s, imagbuf, IMAGMAX);

	write(s, &head, sizeof(head));
	write(s, buf, strlen(buf)+1);
	
#endif
	int size;  
	char *p = (char *)shm_buff_inst->buffer[0];
	while(1)
	{
	#if 0	
		memset(buf, 0, MAX);
		int num = read(s, buf, MAX-1);
		if(0 > num)
		{
			close(s);
			printf("recv failed\n");
			break;
		}

		printf("RECV[%dbytes]:%s\n", num, buf);
	#endif
		size = SHM_BUFF_SZ;//shm_buff_inst->written;
	//	printf("written = %d\n", size);
		int_switch_char(size, imaghead.len, 5);
	//	printf("written = %d\n", size);

		usleep(20000);
		sem_p(s_shm_id.semid);
		write(s, &imaghead, sizeof(head));
		write(s, p, size);
		printf("get image\n");
		sem_v(s_shm_id.semid);

	}


	return 0;
}
