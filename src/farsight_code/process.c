{
1、进程是资源分配的单位：32bits系统下是表征4G的虚拟空间
2、进程空间一定有一个活动序列，就是主线程
3、进程是动态变化（程序是外存上存储的数据，是静态不变）
4、进程空间代码和数据有具体的占用区域

==========================================
进程号：管理进程的一个编号
亲缘关系：表征进程间的关系
进程号是唯一的，标识某一个进程

===========================
创建进程
1、进程复制
#include <unistd.h>
/*
 * 功能：复制进程
 * 参数：
 * 返回值：
	返回值是子进程号，则从父进程返回
	返回值是0 则从子进程返回
	返回值是-1，则复制失败
 */
pid_t fork(void);


pid_t pid = fork();
if(0 > pid){
	perror("fork");
}else if(0 = pid){
	子进程
}else{
	父进程
}
----------------------------------------
2、进程替换
/*
 *功能：进程替换
参数：
	const char *path - 指向程序路径及程序名的字符串首地址
	const char *arg - 程序名
	... - 传给程序的参数（加结束符号， NULL）
 */
int execl(const char *path, const char *arg, ...);

程序执行：路径（环境变量），参数
=================================================
#include <sys/types.h>
#include <unistd.h>

//得到进程ID
pid_t getpid(void);
//得到父进程ID
pid_t getppid(void);

//得到实际组ID
gid_t getgid(void);
//得到有效组ID
gid_t getegid(void);
=================================================
孤儿进程：
	父进程先于子进程退出，子进程交由init进程托管，这个时候子进程叫孤儿进程。

僵尸进程：
	子进程先于父进程退出，父进程没有及时处理
	虽然子进程已经运行结束，但是进程空间没有回收，此时子进程叫僵尸进程。

守护进程
}

/* ======================================================= */

{
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <stdio.h>

#define ASMCODE

#ifdef ASMCODE
int my_open(const char *pathname, int flags)
{
	int ret;
	asm volatile(
		"int $0x80 \n"
		: "=a" (ret)
#if 0
		: "a" (5) , "b" (pathname), "c" (flags)
#else
		: "0" (5) , "b" (pathname), "c" (flags)
#endif
		);

	return ret;
}
#else
int my_open(const char *pathname, int flags)
{
	return syscall(5, pathname, flags);
}
#endif

#ifdef ASMCODE
ssize_t my_read(int fd, void *buf, size_t count)
{
	int ret;
	asm volatile(
		"int $0x80 \n"
		: "=a" (ret)
#if 0
		: "a" (3) , "b" (fd), "c" (buf), "d" (count)
#else
		: "0" (3) , "b" (fd), "c" (buf), "d" (count)
#endif
		);

	return ret;
}
#else
ssize_t my_read(int fd, void *buf, size_t count)
{
	return syscall(3, fd, buf, count);
}
#endif

int main()
{
	int fd = my_open("test.c", O_RDONLY);
	if(0 > fd){
		perror("open");
		return -1;
	}

#define MAX 1024
	char buf[MAX+1];
	memset(buf, 0, MAX+1);
	if(0 > my_read(fd, buf, MAX)){
		perror("read");
		close(fd);
		return -1;
	}
	printf("%s\n", buf);

	return 0;
}

}

