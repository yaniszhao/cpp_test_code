#include <iostream>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <string>

using namespace std;

# define THREAD_NUMS 1000

int fd;

void *pthread_f(void *args){
	ioctl(fd, 1, 0);
}

int main()
{
	int ret;

	if((fd = open("/dev/msg_printer", O_RDWR)) < 0)
	{
		cerr << strerror(errno) << endl;	
		return -1;
	}

    	pthread_t tids[THREAD_NUMS];
    	for(int i=0; i<THREAD_NUMS; i++)
	{
		ret = pthread_create(&tids[i], NULL, pthread_f, NULL);
		if(ret < 0)
		{
			cerr << strerror(errno) << endl;
			close(fd);
			return -1;	
		}	
    	}

	cout<<"wait threads"<<endl;
	for(int i=0; i<THREAD_NUMS; i++)
	{
		pthread_join(tids[i], NULL);
	}

	cout<<"close fd"<<endl;
	close(fd);

	return 0;
}
