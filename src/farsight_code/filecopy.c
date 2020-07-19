#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#define maxsize 256  
int main(int argc,char *argv[])
{
	if(argc!=3)                        //如果命令格式不正确
    	{  
	 printf("command error!\n");
      	return -1;
    	}

	int fd1,fd2;
	if ((fd1= open(argv[1],O_RDONLY))< 0)
	{
		perror("error");
		return -1;
	}
	if ((fd2 = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644))< 0)
	{
		perror("error");
		return -1;
	}
	char s[maxsize];
	int t=0;
	
	while ((t=read(fd1,s,maxsize))==maxsize )
	{
		write(fd2,s,t);
	}
	write(fd2,s,t);
	
	close(fd1);
	close(fd2);
	return 0;
}

