#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define N 128

typedef struct sockaddr SA;


void ProcessList(int connfd)
{
	char buf[N];
	DIR *mydir;
	struct dirent *myitem;

	mydir = opendir(".");
	while ((myitem = readdir(mydir)) != NULL)
	{
		if ((strcmp(myitem->d_name, ".") == 0) || (strcmp(myitem->d_name, "..") == 0)) continue;
		strcpy(buf, myitem->d_name);
		send(connfd, buf, N, 0);
	}
	closedir(mydir);

	return;
}


void ProcessGet(int connfd, char buf[])
{
	int fd, nbyte;

	if ((fd = open(buf+1, O_RDONLY)) < 0)
	{
		fprintf(stderr, "fail to open %s : %s\n", buf+1, strerror(errno));
		buf[0] = 'N';
		send(connfd, buf, N, 0);
		return;
	}
		
	buf[0] = 'Y';
	send(connfd, buf, N, 0);
	while ((nbyte = read(fd, buf, N)) > 0)
	{
		send(connfd, buf, nbyte, 0);
	}
	close(fd);

	return;
}


void ProcessPut(int connfd, char buf[])
{
	int fd, nbyte;

	if ((fd = open(buf+1, O_WRONLY|O_CREAT|O_TRUNC, 0666)) < 0)
	{
		printf("fail to create %s on server\n", buf+1);
		return;
	}

	while ((nbyte = recv(connfd, buf, N, 0)) > 0)
	{
		write(fd, buf, nbyte);
	}
	close(fd);

	return;
}


int main(int argc, char *argv[])
{
   int listenfd, connfd;
	char buf[N];
   struct sockaddr_in server_addr;

  // XXX:step 1  int socket(int domain, int type, int protocol);
   if ((listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
   {
      fprintf(stderr, "fail to socket : %s\n", strerror(errno));
      exit(-1);
   }
#ifdef _DEBUG_
   printf("socket is %d\n", listenfd);
#endif  

  // XXX:step 2  int bind(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen);
   memset(&server_addr, 0, sizeof(server_addr));
   server_addr.sin_family = PF_INET;
   server_addr.sin_port = htons(8888);
   server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(listenfd, (SA *)&server_addr, sizeof(server_addr)) < 0)
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
			break;
		}
		recv(connfd, buf, N, 0);
		switch (buf[0])
		{
			case 'L' :
							ProcessList(connfd);
							break;
			case 'G' :
							ProcessGet(connfd, buf);
							break;
			case 'P' :
							ProcessPut(connfd, buf);
							break;
		}
		close(connfd);
	}

   return 0;
}
