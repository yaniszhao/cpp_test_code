#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <syslog.h>

int main(void) 
{
	pid_t pid, sid;
	int fd;
	char *buf = "This is a Daemon\n";

	pid = fork();
	if (0 > pid) {
		printf("Error fork\n");
		exit(1);
	} else if (0 < pid) {//parrent proccess exit
		exit(0);
 	}

	openlog("daemon_syslog", LOG_PID, LOG_DAEMON);

	if (0 > (sid = setsid())) {//create session
		syslog(LOG_ERR, "%s\n", "setsid");
		exit(1);		
	}

	if (0 > (sid = chdir("/"))) {//change working directory
		syslog(LOG_ERR, "%s\n", "chdir");
		exit(1);
	}

	umask(0);//reset umask;

	int i;
	for (i = 0; i < getdtablesize(); i++) {//close all fd
		close(i);
	}

	while (1) {
		if (0 > (fd = open("/tmp/daemon.log", O_CREAT|O_WRONLY|O_APPEND, 0666))) {
			syslog(LOG_ERR, "open");
			exit(1);
		}
	
		write(fd, buf, strlen(buf) + 1);

		close(fd);
		sleep(10);
	}
	
	closelog();
	exit(0);
}
