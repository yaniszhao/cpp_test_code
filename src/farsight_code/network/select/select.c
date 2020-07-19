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

#include <sys/select.h>

int init_server(const char *ipstr, u_short port, int connmax)
{
	int s = socket(AF_INET, SOCK_STREAM, 0);
	if(0 > s){
		perror("socket");
		return -1;
	}

	struct sockaddr_in s_addr = {
		.sin_family = AF_INET,
		.sin_port   = htons(port),
		.sin_addr   = {
			.s_addr = (NULL == ipstr) ? INADDR_ANY : inet_addr(ipstr),
		}, 
	};
	memset(s_addr.sin_zero, 0, 8);
	socklen_t addrlen= sizeof(s_addr);

	if(0 > bind(s, (struct sockaddr*)&s_addr, addrlen)){
		perror("bind");
		return -1;
	}

	if(0 > listen(s, connmax)){
		perror("listen");
		return -1;
	}

	return s;
}

int main()
{
	int s = init_server(NULL, 9999, 100);
	if(0 > s){
		return -1;
	}

	fd_set new_set, old_set;
	FD_ZERO(&new_set);
	FD_SET(s, &new_set);

	int fdmax = s+1;

	printf("Wait for a new comming.\n");

	while(1){
		FD_ZERO(&old_set);
		old_set = new_set;
		int ret = select(fdmax, &old_set, NULL, NULL, NULL);
		if(0 > ret){
			perror("select");
			break;
		}
	
		if(FD_ISSET(s, &old_set)){
			struct sockaddr_in c_addr;
			memset(&c_addr, 0, sizeof(c_addr));
			socklen_t len = sizeof(c_addr);
			int rws = accept(s, (struct sockaddr*)&c_addr, &len);
			if(0 > rws){
				perror("accept");
				break;
			}
			printf("Newcomming:%s:%d\n", inet_ntoa(c_addr.sin_addr), ntohs(c_addr.sin_port));
			FD_SET(rws, &new_set);

			if(fdmax <= rws){
				fdmax = rws + 1;
			}
		}

		for(int i = s+1; i < fdmax; i++){
			if(FD_ISSET(i, &old_set)){
			#define MAX 1024
				char buf[MAX];
				memset(buf, 0, MAX);
				int num = read(i, buf, MAX);
				if(0 >= num){
					printf("read fail.\n");
					close(i);
					FD_CLR(i, &new_set);
				}else{
					printf("SOCK=%d,%dbytes:%s\n", i, num, buf);
				}
			}
		}
	}

	close(s);
	return 0;	
}
