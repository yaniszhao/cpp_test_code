1、
信号：软中断的通知机制

2、系统已经明确很多事件，使用信号进行通知
 1) SIGHUP	 2) SIGINT	 3) SIGQUIT	 4) SIGILL	 5) SIGTRAP
 6) SIGABRT	 7) SIGBUS	 8) SIGFPE	 9) SIGKILL	
11) SIGSEGV		13) SIGPIPE	14) SIGALRM	15) SIGTERM
16) SIGSTKFLT	17) SIGCHLD	18) SIGCONT	19) SIGSTOP	20) SIGTSTP
21) SIGTTIN	22) SIGTTOU	23) SIGURG	24) SIGXCPU	25) SIGXFSZ
26) SIGVTALRM	27) SIGPROF	28) SIGWINCH	29) SIGIO	30) SIGPWR
31) SIGSYS	

3、用户自定义事件
10) SIGUSR1 12) SIGUSR2	

4、信号处理的方式
A、用户注册一个处理函数
B、系统默认处理
（多数是杀死进程，少量是挂起）
C、忽略信号

5、有些信号是不能被捕获
===============================================
注册/捕获信号
#include <signal.h>
typedef void (*sighandler_t)(int);

/*
 * 功能：注册/捕获信号
 * 参数：
	int signum - 信号
	sighandler_t handler - 信号处理函数
		SIG_IGN - 忽略
		SIG_DFL - 默认处理
 * 返回值：
	SIG_ERR 失败
 */
sighandler_t signal(int signum, sighandler_t handler);


/* ===================================================== */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

int fd[2];

void handler(int sig)
{
	#define MAX 1024
	char buf[MAX+4];
	memset(buf, 0, MAX+4);
	int ret = read(fd[0], buf, MAX);
	if(0 > ret){
		perror("read");
	}else{
		printf("RD[%dbytes]:%s\n", ret, buf);
	}
}

int main()
{
	if(0 > pipe(fd)){
		perror("pipe");
		return -1;
	}

	int flag = fcntl(fd[0], F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(fd[0], F_SETFL, flag);
	
	//after fork, child process can share this
	signal(SIGUSR1, handler);

	pid_t pid = fork();
	if(0 > pid){
		perror("fork");
	}else if(0 == pid){
		close(fd[1]);

		while(1){
			printf("---do something----%d-------\n", getpid());
			sleep(1);
		};

		close(fd[0]);
	}else{
		close(fd[0]);

		while(1){
			char buf[] = "hello world";
			if(0 > write(fd[1], buf, sizeof(buf)-1)){
				perror("write");
				return -1;
			}

			kill(pid, SIGUSR1);
			printf("snd done.\n");
			sleep(3);
		}

		close(fd[1]);
	}
}

/* ================================================ */

#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig)
{
	printf("---%d---\n", sig);
	alarm(3);
}

int mysleep(int s)
{
	signal(SIGALRM, handler);

	alarm(s);//after s second, send SIGALRM signal
	//raise(int sig) :send sig signal to itself
	
	//return pause();
	pause();//block and wait signal come
	return 0;
}

int main()
{
#if 0
	printf("-------1--------\n");
	mysleep(3);
	printf("-------2--------\n");
#else
	signal(SIGALRM, handler);
	alarm(3);
	
	while(1){
		printf("-------1--------\n");
		pause();
		printf("-------2--------\n");
	}

#endif
}

