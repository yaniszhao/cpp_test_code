#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

//#define ENABLENONBLOCK

int main()
{
	int fd[2];
	if(0 > pipe(fd)){
		perror("pipe");
		return -1;
	}

#ifdef ENABLENONBLOCK
	int flag = fcntl(fd[0], F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(fd[0], F_SETFL, flag);
#endif

	pid_t pid = fork();
	if(0 > pid){
		perror("fork");
	}else if(0 == pid){
		close(fd[1]);
		
		#define MAX 1024
		char buf[MAX+4];
		memset(buf, 0, MAX+4);
		int ret = read(fd[0], buf, MAX);
		if(0 > ret){
			perror("read");
			return -1;
		}
		printf("RD[%dbytes]:%s\n", ret, buf);
		close(fd[0]);
	}else{
		close(fd[0]);
		sleep(3);
		while(1){
			char buf[] = "hello world";
			if(0 > write(fd[1], buf, sizeof(buf)-1)){
				perror("write");
				return -1;
			}
		}

		printf("snd done.\n");
		close(fd[1]);
	}
}

