#include <sys/types.h>
#include <sys/socket.h>
/*
 * 功能：发送网络数据包
 * 参数：
	int sockfd - 套接字
	const void *buf - 指向发送数据的首地址
	size_t len - 长度
	int flags - 标志
		MSG_DONTWAIT - 非阻塞
		MSG_NOSIGNAL - 对方断开连接时，不发信息SIGPIPE
 * 返回值：
	成功：实际发送的字节数（TCP流式套接字，就是想发的字节数）
	失败: -1
 */
ssize_t send(int sockfd, const void *buf, size_t len, int flags);

/*
 * 功能：发送网络数据包(指定接收方)
 * 参数：
	int sockfd - 套接字
	const void *buf - 指向发送数据的首地址
	size_t len - 长度
	int flags - 标志
		MSG_DONTWAIT - 非阻塞
		MSG_NOSIGNAL - 对方断开连接时，不发信息SIGPIPE
	const struct sockaddr *dest_addr - 指向接收方的三元组信息（协议族、IP、端口）
	socklen_t addrlen - 三元组信息长度
 * 返回值：
	成功：实际发送的字节数（一般仅仅用于UDP，就是想发的字节数）
	失败: -1
 */
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
	      const struct sockaddr *dest_addr, socklen_t addrlen);
/*
 * 功能：发送数据包(unix套接字域：用于本地进程间通讯)
 * 参数：
	int sockfd - 套接字
	struct msghdr *msg - 消息包
	int flags - 标志
 * 返回值：
	成功：实际发送的字节数
	失败: -1
 */
ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
=====================================================================
/*
 * 功能：接收网络数据包
 * 参数：
	int sockfd - 套接字
	const void *buf - 指向接收缓存首地址
	size_t len - 缓存长度
	int flags - 标志
		MSG_DONTWAIT - 非阻塞
 * 返回值：
	成功：实际接收的字节数
	失败: -1
 */
ssize_t recv(int sockfd, void *buf, size_t len, int flags);

/*
 * 功能：接收网络数据包
 * 参数：
	int sockfd - 套接字
	const void *buf - 指向接收缓存首地址
	size_t len - 缓存长度
	int flags - 标志
		MSG_DONTWAIT - 非阻塞
	struct sockaddr *src_addr - 指向保存发送方的三元组信息的结构首地址
	socklen_t *addrlen - 保存对方三元组信息结构的长度
 * 返回值：
	成功：实际接收的字节数
	失败: -1
 */
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
		struct sockaddr *src_addr, socklen_t *addrlen);

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
============================================================
UDP
1、没有连接
2、通讯不可靠，（发出去不管）
3、不存对方端有效的情况
4、没有同步问题（没有黏包）
============================================================
SERVER			CLIENT
while(1){		while(1){
	sento			recvfrom
	recvfrom		sento
}			}
============================================================
连接0（设置网卡的一个软件）<--->eth0(linux系统的驱动的物理接口)<-虚拟硬件网卡->网络适配器0<--->VMnet0(虚拟网络)<-桥接->PCIe<--->internet
连接1（设置网卡的一个软件）<--->eth1(linux系统的驱动的物理接口)<-虚拟硬件网卡->网络适配器1<--->VMnet1(虚拟网络)<-桥接->8139<--->板子

