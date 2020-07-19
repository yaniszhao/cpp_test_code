#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 50

typedef struct sockaddr SA;

int main(int argc, char **argv)
{        
  int listenfd, connfd, pid;
  struct sockaddr_in  myaddr;
  char  buf[MAXLINE];
        
  if ((listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("fail to socket");
    exit(-1);
  }
  
  bzero(&myaddr, sizeof(myaddr));
  myaddr.sin_family      = PF_INET;
  myaddr.sin_addr.s_addr = inet_addr("192.168.1.100");  //htonl(INADDR_ANY);
  myaddr.sin_port        = htons(8888);   /* port number */
        
  if (bind(listenfd, (SA *) &myaddr, sizeof(myaddr)) < 0)
  {
    perror("fail to bind");
    exit(-1);
  }
  
	listen(listenfd, 5);

  while ( 1 )
  {
		 if ((connfd = accept(listenfd, NULL, NULL)) < 0)
		 {
				perror("fail to accept");
		 }
		 else
		 {
				if ((pid = fork()) < 0)
				{
				  perror("fail to fork");
				}
				else if (pid == 0)  //  child process: interact with client
				{
					while (recv(connfd, buf, sizeof(buf), 0) > 0)
					{
						send(connfd, buf, sizeof(buf), 0);
					}
					exit(0);
				}
				else
				{
					close(connfd);
				}
		 }
  }

  return 0;
}
