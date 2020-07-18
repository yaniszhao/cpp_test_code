#include <iostream>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <cstdio>

using namespace std;
struct Msg{
	int num1;
	int num2;
	int sum;
};

int main()
{
	int fd = 0;
	int ret;

	if((fd = open("/dev/msg_printer", O_RDWR)) < 0)
	{
		cerr << strerror(errno) << endl;
		return -1;
	}
	
	cout<<"input num1 and num2:"<<endl;
	Msg msg_user;
	cin>>msg_user.num1>>msg_user.num2;

	//cout<<msg_user.num1<<endl;
	//cout<<msg_user.num2<<endl;
	//cout<<&msg_user<<endl;

	ret = ioctl(fd, 1, (void *)&msg_user);
	if(ret < 0)
	{
		cerr << strerror(errno) << endl;
		close(fd);
		return -1;	
	}

	cout<<msg_user.num1<<"+"<<msg_user.num2<<"="<<msg_user.sum<<endl;
	close(fd);

	return 0;
}
