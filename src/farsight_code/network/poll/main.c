#include <stdio.h>

#include <poll.h>
#include <unistd.h>

#include <myserver.h>

#define INVALSOC -1

int main()
{
	int s = init_server(NULL, 9999, 100);
	if(0 > s){
		return -1;
	}

	printf("Wait for a new comming.\n");

	int i;
	#define SOCMAX 1024
	struct pollfd pfd[SOCMAX];
	for(i = 1; i < SOCMAX; i++){
		pfd[i].fd	= INVALSOC;
		pfd[i].events	= POLLIN;
		pfd[i].revents	= 0;
	}

	pfd[0].fd	= s;
	pfd[0].events	= POLLIN;
	pfd[0].revents	= 0;

	int timeout = -1;
	while(1){
		int ret = poll(pfd, SOCMAX, timeout);
		if(0 > ret){
			perror("poll");
			return -1;
		}else if(0 == ret){
			printf("\tTimeout.\n");
			continue;
		}

		if(POLLIN & pfd[0].revents){
			if(0 > newconn(s, pfd, SOCMAX)){
				break;
			}
		}

		for(i = 1; i < SOCMAX; i++){
			if(POLLIN & pfd[i].revents){
				if(0 > recvdata(pfd[i].fd)){
					close(pfd[i].fd);
					pfd[i].fd = INVALSOC;
				}
			}
		}		
	}	
}
