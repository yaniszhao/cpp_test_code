1、网络编程是进程间通讯
A、不同主机间通讯
B、怎么描述主机（发到哪里、哪里接收）
C、怎么描述进程（谁收、谁发）
进程可以用进程号来表征，但是进程在程序运行前无法预知，是系统自动分配

主机+进程 <----> 主机+进程
从哪里来，要到哪里去
谁发送数据，谁接收数据
=====================================================
TCP/IP协议族
【协议头】+【正文数据】《----》【协议头】+【正文数据】
现实生活中的邮件系统
邮政编码（哪里）
收件人、发件人

接收邮政地址
	收件人			+ 信件正文
		发件人
		发送邮政地址

=====================================================
OSI模型（7层）
应用层：想收发的是什么
表示层：对收发数据逻辑处理：加密、压缩，等等
会话层：进程号+逻辑名称（端口号）
传输层：数据收发的逻辑处理：可靠通讯（应答），校验，
网络层：从哪里来，到哪里去：路由选择（）
	
	效率：从可靠性角度看，正文数据越小越可靠
		从通讯角度看，正正文越大效率越高
	解决办法：分组

链路层：字节流数据《---》帧数据
物理层：帧数据收发控制
===================================================
工业标志
应用层（应用程序）：想收发什么				
 网页（HTTP），文件传输（FTP）

传输层(内核网络子系统的协议层)：谁收谁发（port）、校验、可靠与否

	效率：从可靠性角度看，正文数据越小越可靠
		从通讯角度看，正正文越大效率越高
	拆包（以太网要求tcp包 46~1500字节）

网络层(内核网络子系统的协议层)：从哪里来，到哪里去（IP）
	分组（如果应用层发送数据包过大，大于协议约定的大小（65536），分组）

物理层(内核驱动)：字节流数据《---》帧数据
	帧数据收发控制
========================================================
1、如果操作网络
A、普通文件
有文件名（带路径）----》用于数据存储
B、终端
字符设备
设备文件（/dev）----》找终端设备
C、管道
管道文件（带路径）---》找管道
D、IPC对象
key值

E、网络
物理接口名：网络设备
套接字：是一个整型，一个特殊的文件描述符，表征网络对象
(基于TCP/IP的套接字)
--------------------------------------------------
2、端口
是一个特殊整型（unsigned short），用于标识进程
--------------------------------------------------
3、IP
是一个特殊的整型（IPv4，32bits，unsigned int），用来标识主机
A、格式
网络类型：A、B、C、D...
网络号+主机号
B、子网掩码
用于提取网络号
C、专用IP
譬如最大的主机号就是广播地址
组播地址,....

D、DNS
域名解析
域名---主机名（http协议）
IP <----> 域名
....

--------------------------------------------------
4、大小端
协议约定：网络字节序
uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(uint16_t netshort);
================================================
字符串的IP到整型IP的转换
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*
 * 把字符串IP转成整型
	const char *cp - 指向要被转换的IP字符串首地址
	struct in_addr *inp - 指向被转换的成功IP地址结构首地址
 * 返回值：
	IP的整型值
 */
int inet_aton(const char *cp, struct in_addr *inp);
/*
 * 把字符串IP转成整型
	const char *cp - 指向要被转换的IP字符串首地址
 * 返回值：
	IP的整型值
 */
in_addr_t inet_addr(const char *cp);

/*
 * 把字符串IP转成整型
	struct in_addr in - IP地址
 * 返回值：
	IP字符串的首地址
 */
char *inet_ntoa(struct in_addr in);


netinet/in.h
typedef unsigned int uint32_t;
typedef uint32_t in_addr_t;
struct in_addr
{
	in_addr_t s_addr;
};

//通用地址结构
struct sockaddr
{    
	u_short  sa_family;    // 地址族, AF_xxx
	char  sa_data[14];     // 14字节协议地址
};

//Internet协议地址结构
struct sockaddr_in
{           
	u_short sin_family;      // 地址族, AF_INET，2 bytes
	u_short sin_port;      // 端口，2 bytes
	struct in_addr sin_addr;  // IPV4地址，4 bytes 	
	char sin_zero[8];        // 8 bytes unused，作为填充
}; 
========================================================
1、套接字类型
A、流式套接字
TCP:面向连接的可靠通讯

B、数据报套接字
UDP：无连接不可靠通讯

C、原始套接字
操作的网络层，得到的是IP包


#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

2、创建套接字

/*
 * 功能：创建套接字
	int domain - 协议族/地址族：AF_INET IPv4地址族、PF_INET IPv4协议族
	int type   - 套接字类型
			SOCK_STREAM - 流式套接字	--->TCP协议
			SOCK_DGRAM  - 数据报套接字	--->UDP协议
			SOCK_RAW    - 原始套接字	--->对应的是网络层
	int protocol - 协议编号

 * 返回值：
	成功：套接字（一个特殊文件描述符）
		流式套接字，用于等连接
		数据包套接字用于通讯
	失败：-1
 */
int socket(int domain, int type, int protocol);


3、绑定
A、系统自动绑定

B、程序员自己绑定
套接字，关联（IP+端口） 《-----》套接字，关联（IP+端口）

/*
 * 功能：绑定
 * 参数：
	int sockfd - 被绑定的套接字
	const struct sockaddr *addr - 绑定的三元组信息
				协议族
				IP
				端口
	socklen_t addrlen - 三元组信息的长度
 * 返回值：
	失败：-1
 */
int bind(int sockfd, const struct sockaddr *addr,
                socklen_t addrlen);

4、连接
TCP是面向连接
A、套接字-关联-源IP+源端口，目标IP+目标端口） 《-----》套接字-关联-源IP+源端口，目标IP+目标端口）
B、可靠
（1）、最连接等待数量
/*
 * 功能：设置最大等待的监听数
 * 参数：
	int sockfd - 监听的套接字（前台接待）
	int backlog- 最大等待监听数
 * 返回值：
	失败：-1
 */
int listen(int sockfd, int backlog);

（2）、连接
/*
 * 功能：连接
 * 参数：
	int sockfd - 套接字
	const struct sockaddr *addr - 要连接谁：连接对方的三元组信息
			协议族
			IP
			端口
	socklen_t addrlen - 三元组信息长度
 * 返回值：
	失败：-1
 */
int connect(int sockfd, const struct sockaddr *addr,
                   socklen_t addrlen);
（3）、等连接
/*
 * 功能：等连接
 * 参数：
	int sockfd - 用于监听套接字（前台接待）
	const struct sockaddr *addr - （输出参数）谁来连：连接方的三元组信息
			协议族
			IP
			端口
	socklen_t *addrlen - 得到三元组信息长度
 * 返回值：
	失败：-1
	成功：得到用于读写的套接字（表征连接）
 */
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
---------------------------------------------------------------------
服务器				客户端
1.创建等连接套接字		1.创建套接字
int s = socket()		int s = socket()

2.绑定三元组信息
bind(s, IP+PORT,...)		...

3.设置最大等待连接数	
listen(s, MAX);			...

4.等连接			2.准备好服务器的IP和端口并连接
int rws = accept(s, ...) <--->	connect(s,服务器的三元组)

5. 收/发数据		<--->	3.发/收数据

read/write(rws, ...)	<--->	write/read(s, ...)

6. 关闭读写套接字(断开连接）	4.关闭套接字

7. 关闭连接套接字
=============================================================
注意事项
1、阻塞
2、非阻塞
3、正在发送的时候，接收端异常断开
4、同步
read() <--次数不一致--> write();

/* ==================================================== */

//netinet/in.h
typedef uint32_t in_addr_t;
struct in_addr
{
	in_addr_t s_addr;
};

//通用地址结构
struct sockaddr
{    
	u_short  sa_family;    // 地址族, AF_xxx
	char  sa_data[14];     // 14字节协议地址
};

//Internet协议地址结构
struct sockaddr_in
{           
	u_short sin_family;      // 地址族, AF_INET，2 bytes
	u_short sin_port;      // 端口，2 bytes
	struct in_addr sin_addr;  // IPV4地址，4 bytes 	
	char sin_zero[8];        // 8 bytes unused，作为填充
}; 


socket(port+ip) <-----> socket(port+ip) 

/* =================================================== */

AF 表示ADDRESS FAMILY 地址族
PF 表示PROTOCOL FAMILY 协议族
但这两个宏定义是一样的
所以使用哪个都没有关系
Winsock2.h中
#define AF_INET 2
#define PF_INET AF_INET
所以在windows中AF_INET与PF_INET完全一样
而在Unix/Linux系统中，在不同的版本中这两者有微小差别
对于BSD,是AF,对于POSIX是PF
UNIX系统支持AF_INET，AF_UNIX，AF_NS等，而DOS,Windows中仅支持AF_INET，它是网际网区域。
在函数socketpair与socket的domain参数中有AF_UNIX,AF_LOCAL,AF_INET,PF_UNIX,PF_LOCAL,PF_INET.
这几个参数有AF_UNIX=AF_LOCAL, PF_UNIX=PF_LOCAL, AF_LOCAL=PF_LOCAL, AF_INET=PF_INET.
建议:对于socketpair与socket的domain参数,使用PF_LOCAL系列,
而在初始化套接口地址结构时,则使用AF_LOCAL.

例如:
z = socket(PF_LOCAL, SOCK_STREAM, 0);
adr_unix.sin_family = AF_LOCAL;


include/i386-linux-gnu/bits/socket.h
/* Protocol families.  */
#define	PF_UNSPEC	0	/* Unspecified.  */
#define	PF_LOCAL	1	/* Local to host (pipes and file-domain).  */
#define	PF_UNIX		PF_LOCAL /* POSIX name for PF_LOCAL.  */
#define	PF_FILE		PF_LOCAL /* Another non-standard name for PF_LOCAL.  */
#define	PF_INET		2	/* IP protocol family.  */
#define	PF_AX25		3	/* Amateur Radio AX.25.  */
#define	PF_IPX		4	/* Novell Internet Protocol.  */
#define	PF_APPLETALK	5	/* Appletalk DDP.  */
#define	PF_NETROM	6	/* Amateur radio NetROM.  */
#define	PF_BRIDGE	7	/* Multiprotocol bridge.  */
#define	PF_ATMPVC	8	/* ATM PVCs.  */
#define	PF_X25		9	/* Reserved for X.25 project.  */
#define	PF_INET6	10	/* IP version 6.  */
#define	PF_ROSE		11	/* Amateur Radio X.25 PLP.  */
#define	PF_DECnet	12	/* Reserved for DECnet project.  */
#define	PF_NETBEUI	13	/* Reserved for 802.2LLC project.  */
#define	PF_SECURITY	14	/* Security callback pseudo AF.  */
#define	PF_KEY		15	/* PF_KEY key management API.  */
#define	PF_NETLINK	16
#define	PF_ROUTE	PF_NETLINK /* Alias to emulate 4.4BSD.  */
#define	PF_PACKET	17	/* Packet family.  */
#define	PF_ASH		18	/* Ash.  */
#define	PF_ECONET	19	/* Acorn Econet.  */
#define	PF_ATMSVC	20	/* ATM SVCs.  */
#define PF_RDS		21	/* RDS sockets.  */
#define	PF_SNA		22	/* Linux SNA Project */
#define	PF_IRDA		23	/* IRDA sockets.  */
#define	PF_PPPOX	24	/* PPPoX sockets.  */
#define	PF_WANPIPE	25	/* Wanpipe API sockets.  */
#define PF_LLC		26	/* Linux LLC.  */
#define PF_CAN		29	/* Controller Area Network.  */
#define PF_TIPC		30	/* TIPC sockets.  */
#define	PF_BLUETOOTH	31	/* Bluetooth sockets.  */
#define	PF_IUCV		32	/* IUCV sockets.  */
#define PF_RXRPC	33	/* RxRPC sockets.  */
#define PF_ISDN		34	/* mISDN sockets.  */
#define PF_PHONET	35	/* Phonet sockets.  */
#define PF_IEEE802154	36	/* IEEE 802.15.4 sockets.  */
#define PF_CAIF		37	/* CAIF sockets.  */
#define PF_ALG		38	/* Algorithm sockets.  */
#define PF_NFC		39	/* NFC sockets.  */
#define	PF_MAX		40	/* For now..  */

/* Address families.  */
#define	AF_UNSPEC	PF_UNSPEC
#define	AF_LOCAL	PF_LOCAL
#define	AF_UNIX		PF_UNIX
#define	AF_FILE		PF_FILE
#define	AF_INET		PF_INET
#define	AF_AX25		PF_AX25
#define	AF_IPX		PF_IPX
#define	AF_APPLETALK	PF_APPLETALK
#define	AF_NETROM	PF_NETROM
#define	AF_BRIDGE	PF_BRIDGE
#define	AF_ATMPVC	PF_ATMPVC
#define	AF_X25		PF_X25
#define	AF_INET6	PF_INET6
#define	AF_ROSE		PF_ROSE
#define	AF_DECnet	PF_DECnet
#define	AF_NETBEUI	PF_NETBEUI
#define	AF_SECURITY	PF_SECURITY
#define	AF_KEY		PF_KEY
#define	AF_NETLINK	PF_NETLINK
#define	AF_ROUTE	PF_ROUTE
#define	AF_PACKET	PF_PACKET
#define	AF_ASH		PF_ASH
#define	AF_ECONET	PF_ECONET
#define	AF_ATMSVC	PF_ATMSVC
#define AF_RDS		PF_RDS
#define	AF_SNA		PF_SNA
#define	AF_IRDA		PF_IRDA
#define	AF_PPPOX	PF_PPPOX
#define	AF_WANPIPE	PF_WANPIPE
#define AF_LLC		PF_LLC
#define AF_CAN		PF_CAN
#define AF_TIPC		PF_TIPC
#define	AF_BLUETOOTH	PF_BLUETOOTH
#define	AF_IUCV		PF_IUCV
#define AF_RXRPC	PF_RXRPC
#define AF_ISDN		PF_ISDN
#define AF_PHONET	PF_PHONET
#define AF_IEEE802154	PF_IEEE802154
#define AF_CAIF		PF_CAIF
#define AF_ALG		PF_ALG
#define AF_NFC		PF_NFC
#define	AF_MAX		PF_MAX

/* ===================================================== */

#include <stdio.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

union data{
	unsigned int i;
	unsigned char ch[4];
}data;


int main()
{
	data.ch[0] = 192;
	data.ch[1] = 168;
	data.ch[2] = 1;
	data.ch[3] = 249;
	printf("%u\n", data.i);

	struct in_addr addr;
	/*inet_addr之后得到的结果和上面一样，
	 *应该只是将点做了处理。相反，加上了点。
	 */
	addr.s_addr = inet_addr("192.168.1.249");
	printf("%u\n", addr.s_addr);
	printf("%s\n", inet_ntoa(addr));
}

/* =============================================== */

linux/if.h
struct ifreq {
#define IFHWADDRLEN	6
	union
	{
		char	ifrn_name[IFNAMSIZ];		/* if name, e.g. "en0" */
	} ifr_ifrn;
	
	union {
		struct	sockaddr ifru_addr;
		struct	sockaddr ifru_dstaddr;
		struct	sockaddr ifru_broadaddr;
		struct	sockaddr ifru_netmask;
		struct  sockaddr ifru_hwaddr;
		short	ifru_flags;
		int	ifru_ivalue;
		int	ifru_mtu;
		struct  ifmap ifru_map;
		char	ifru_slave[IFNAMSIZ];	/* Just fits the size */
		char	ifru_newname[IFNAMSIZ];
		void *	ifru_data;
		struct	if_settings ifru_settings;
	} ifr_ifru;
};
# define ifr_name	ifr_ifrn.ifrn_name	/* interface name 	*/
# define ifr_hwaddr	ifr_ifru.ifru_hwaddr	/* MAC address 		*/
# define ifr_addr	ifr_ifru.ifru_addr	/* address		*/
# define ifr_dstaddr	ifr_ifru.ifru_dstaddr	/* other end of p-p lnk	*/
# define ifr_broadaddr	ifr_ifru.ifru_broadaddr	/* broadcast address	*/
# define ifr_netmask	ifr_ifru.ifru_netmask	/* interface net mask	*/
# define ifr_flags	ifr_ifru.ifru_flags	/* flags		*/
# define ifr_metric	ifr_ifru.ifru_ivalue	/* metric		*/
# define ifr_mtu	ifr_ifru.ifru_mtu	/* mtu			*/
# define ifr_map	ifr_ifru.ifru_map	/* device map		*/
# define ifr_slave	ifr_ifru.ifru_slave	/* slave device		*/
# define ifr_data	ifr_ifru.ifru_data	/* for use by interface	*/
# define ifr_ifindex	ifr_ifru.ifru_ivalue    /* interface index      */
# define ifr_bandwidth	ifr_ifru.ifru_ivalue	/* link bandwidth	*/
# define ifr_qlen	ifr_ifru.ifru_ivalue	/* queue length		*/
# define ifr_newname	ifr_ifru.ifru_newname	/* New name		*/
# define _IOT_ifreq	_IOT(_IOTS(char),IFNAMSIZ,_IOTS(char),16,0,0)
# define _IOT_ifreq_short _IOT(_IOTS(char),IFNAMSIZ,_IOTS(short),1,0,0)
# define _IOT_ifreq_int	_IOT(_IOTS(char),IFNAMSIZ,_IOTS(int),1,0,0)

linux/sockios.h
#define SIOCGIFNAME	0x8910		/* get iface name		*/
#define SIOCSIFLINK	0x8911		/* set iface channel		*/
#define SIOCGIFCONF	0x8912		/* get iface list		*/
#define SIOCGIFFLAGS	0x8913		/* get flags			*/
#define SIOCSIFFLAGS	0x8914		/* set flags			*/
#define SIOCGIFADDR	0x8915		/* get PA address		*/
#define SIOCSIFADDR	0x8916		/* set PA address		*/
#define SIOCGIFDSTADDR	0x8917		/* get remote PA address	*/
#define SIOCSIFDSTADDR	0x8918		/* set remote PA address	*/
#define SIOCGIFBRDADDR	0x8919		/* get broadcast PA address	*/
#define SIOCSIFBRDADDR	0x891a		/* set broadcast PA address	*/
#define SIOCGIFNETMASK	0x891b		/* get network PA mask		*/
#define SIOCSIFNETMASK	0x891c		/* set network PA mask		*/
#define SIOCGIFMETRIC	0x891d		/* get metric			*/
#define SIOCSIFMETRIC	0x891e		/* set metric			*/
#define SIOCGIFMEM	0x891f		/* get memory address (BSD)	*/
#define SIOCSIFMEM	0x8920		/* set memory address (BSD)	*/
#define SIOCGIFMTU	0x8921		/* get MTU size			*/
#define SIOCSIFMTU	0x8922		/* set MTU size			*/
#define SIOCSIFNAME	0x8923		/* set interface name */
#define	SIOCSIFHWADDR	0x8924		/* set hardware address 	*/
#define SIOCGIFENCAP	0x8925		/* get/set encapsulations       */
#define SIOCSIFENCAP	0x8926		
#define SIOCGIFHWADDR	0x8927		/* Get hardware address		*/
#define SIOCGIFSLAVE	0x8929		/* Driver slaving support	*/
#define SIOCSIFSLAVE	0x8930
#define SIOCADDMULTI	0x8931		/* Multicast address lists	*/
#define SIOCDELMULTI	0x8932
#define SIOCGIFINDEX	0x8933		/* name -> if_index mapping	*/
#define SIOGIFINDEX	SIOCGIFINDEX	/* misprint compatibility :-)	*/
#define SIOCSIFPFLAGS	0x8934		/* set/get extended flags set	*/
#define SIOCGIFPFLAGS	0x8935
#define SIOCDIFADDR	0x8936		/* delete PA address		*/
#define	SIOCSIFHWBROADCAST	0x8937	/* set hardware broadcast addr	*/
#define SIOCGIFCOUNT	0x8938		/* get number of devices */
