有名管道注意事项
1、阻塞
读写都会阻塞（当打开管道文件时是阻塞版）

2、同步问题
发送（write()）---》接收（read()）次数一致
//一般先开读端
3、写端必须在读端存在情况下才有意义
//所以写端不用考虑创建的问题

/* =============================================== */

#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>

#define FILENAME "myfifo"

int main()
{
	if (-1 == access(FILENAME, F_OK)) {//if pipe is not exist
		int ret = mkfifo(FILENAME, 0666);
		if(0 > ret){
			if(EEXIST != errno){
				perror("mkfifo");
				return -1;
			}			
		}
	}
	
	printf("----------1-------------\n");

//	int fd = open(FILENAME, O_RDONLY | O_NONBLOCK);
	int fd = open(FILENAME, O_RDONLY);
	if(0 > fd){
		perror("open");
		return -1;
	}

	printf("stardby to read.\n");
	getchar();
	printf("start to read.\n");

	int i = 3;
	while(i--){
		#define MAX 1024
		char buf[MAX];
		memset(buf, 0, MAX);
		ret = read(fd, buf, MAX);
		if(0 > ret){
			perror("read");
			return -1;
		}
		printf("RD[%dbytes]:%s\n", ret, buf);
	}

	getchar();

	close(fd);
	//remove(FILENAME);
}

/* =================================================== */

#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>

#define FILENAME "myfifo"

int main()
{
	int ret = mkfifo(FILENAME, 0666);
	if(0 > ret){
		if(EEXIST != errno){
			perror("mkfifo");
			return -1;
		}
		printf("-----file exist-----\n");
	}

//	int fd = open(FILENAME, O_WRONLY | O_NONBLOCK);
	int fd = open(FILENAME, O_WRONLY);
	if(0 > fd){
		perror("open");
		return -1;
	}

	int i = 100;
	while(i--){
		char buf[]="hello world";
		ret = write(fd, buf, sizeof(buf));
		if(0 > ret){
			perror("write");
			return -1;
		}

		printf("NO.%d: wr done. \n", i);
	}

	printf("----------333----------\n");
	getchar();

	close(fd);
	//remove(FILENAME);
}
