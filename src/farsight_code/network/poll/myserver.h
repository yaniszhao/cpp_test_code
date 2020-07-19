/*
 * Demo for epoll
 * writed by panzhh
 * 2013.11.15
 */

#ifndef MYSERVER_H
#define MYSERVER_H

#include <sys/types.h>

/*
 * 功能：初始化tcp server
 * 参数：
	const char *ipstr - 指向ip的字符串首地址，如果为NULL则系统自动绑定IP
			示例："192.168.1.111"
	u_short port - 指定server的端口号
	int backlog  - 指定server最大等待连接数
 * 返回值：
	失败：-1（错误信息查看errno）
	成功：监听套接字	
 */
int init_server(const char *ipstr, u_short port, int backlog);

/*
 * 功能：设置指定文件描述符是非阻塞
 * 参数：
	int fd - 指定文件描述符
 * 返回值：
	失败：-1（错误信息查看errno）
 */
int set_nonblock(int fd);

/*
 * 功能：读连接并加入到poll集合
 * 参数：
	int s - 监听套接字
	struct pollfd *arr - 指向poll集合结构首地址
	const int lenmax   - poll集合结构最大数量
 * 返回值：
	失败：-1（错误信息查看errno）
 */
int newconn(int s, struct pollfd *arr, const int lenmax);

/*
 * 功能：接收客户端数据
 * 参数：
	int rws - 读写套接字
 * 返回值：
	失败：-1（错误信息查看errno）
 */
int recvdata(int rws);

#endif

