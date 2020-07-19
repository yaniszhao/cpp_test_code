/*
 * Demo for epoll
 * writed by panzhh
 * 2013.11.15
 */

#ifndef MYEPOLL_H
#define MYEPOLL_H

#define MYDBG
#ifndef MYDBG
	#define TRACESTR(str) 
	#define TRACEINT(i) 
	#define TRACEPTR(ptr) 
#else
	#define TRACESTR(str) \
		printf("TRACE[%s]:%s-%s-%d\n", (str), __FILE__, __func__, __LINE__)
	#define TRACEINT(i) \
		printf("TRACE[%d]:%s-%s-%d\n", (i), __FILE__, __func__, __LINE__)
	#define TRACEPTR(ptr) \
		printf("TRACE[%p]:%s-%s-%d\n", (ptr), __FILE__, __func__, __LINE__)
#endif

/*事件回调接口原型*/
typedef int (*HANDLER)(int epfd, void *arg);
/*监测对象结构体*/
typedef struct __myevent{
	int fd;
	unsigned int events;
	HANDLER handler;
	void *arg;
}MYEVENT;

/*
 *功能：添加监测事件
 *参数：
 *	int epfd    - epoll的文件描述符
 *	MYEVENT *evp- 指向监测对象
 *返回：
 *	失败：-1
 */
int add_myevent(int epfd, MYEVENT *evp);
/*
 *功能：删除监测事件
 *参数：
 *	int epfd    - epoll的文件描述符
 *	MYEVENT *evp- 指向监测对象
 *返回：
 *	失败：-1
 */
int del_myevent(int epfd, MYEVENT *evp);
/*
 *功能：修改监测事件
 *参数：
 *	int epfd    - epoll的文件描述符
 *	MYEVENT *evp- 指向监测对象
 *返回：
 *	失败：-1
 */
int mod_myevent(int epfd, MYEVENT *evp);

/*
 *功能：初始化监测对象
 *参数：
 *	MYEVENT *evp- 指向监测对象
 *	int fd	    - 监测对象的文件描述符
 *	unsigned int events- 监测对象的被监测事件
 *	HANDLER handler    - 指向事件回调函数
 *	void *arg          - 事件回调函数的参数
 *返回：
 *	NULL
 */
void init_myevent(MYEVENT *evp, int fd, unsigned int events, HANDLER handler, void *arg);

#endif

