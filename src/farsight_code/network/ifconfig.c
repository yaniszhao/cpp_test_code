#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netinet/in.h>
#include <linux/if.h>
#include <sys/ioctl.h>

#define IFNAME "eth0"

int getip(const int s, const char *ifname, struct in_addr *ip)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_addr;
	addr->sin_family = AF_INET;
	if(0 > ioctl(s, SIOCGIFADDR, &ifr)){
		perror("ioctl-SIOCGIFADDR");
		return -1;
	}

	*ip = addr->sin_addr;
	
	return 0;
}

int getnetmask(const int s, const char *ifname, struct in_addr *netmask)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_addr;
	addr->sin_family = AF_INET;
	if(0 > ioctl(s, SIOCGIFNETMASK, &ifr)){
		perror("ioctl-SIOCGIFNETMASK");
		return -1;
	}

	*netmask = addr->sin_addr;
	
	return 0;
}

int getbroadcast(const int s, const char *ifname, struct in_addr *broadcast)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_addr;
	addr->sin_family = AF_INET;
	if(0 > ioctl(s, SIOCGIFBRDADDR, &ifr)){
		perror("ioctl-SIOCGIFBRDADDR");
		return -1;
	}

	*broadcast = addr->sin_addr;
	
	return 0;
}

int getmtu(const int s, const char *ifname)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	if(0 > ioctl(s, SIOCGIFMTU, &ifr)){
		perror("ioctl-SIOCGIFMTU");
		return -1;
	}
	
	return ifr.ifr_mtu;
}

int gethwaddr(const int s, const char *ifname, struct sockaddr *hwaddr)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	if(0 > ioctl(s, SIOCGIFHWADDR, &ifr)){
		perror("ioctl-SIOCGIFHWADDR");
		return -1;
	}

	*hwaddr = ifr.ifr_hwaddr;
	
	return 0;
}

int setip(const int s, const char *ifname, const char *ipstr)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_addr;
	addr->sin_family	= AF_INET;
	addr->sin_addr.s_addr	= inet_addr(ipstr);

	if(0 > ioctl(s, SIOCSIFADDR, &ifr)){
		perror("ioctl-SIOCSIFADDR");
		return -1;
	}
	
	return 0;
}

int setnetmask(const int s, const char *ifname, const char *netmask)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_addr;
	addr->sin_family	= AF_INET;
	addr->sin_addr.s_addr	= inet_addr(netmask);

	if(0 > ioctl(s, SIOCSIFNETMASK, &ifr)){
		perror("ioctl-SIOCSIFNETMASK");
		return -1;
	}
	
	return 0;
}

int setbroadcast(const int s, const char *ifname, const char *broadcast)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_addr;
	addr->sin_family	= AF_INET;
	addr->sin_addr.s_addr	= inet_addr(broadcast);

	if(0 > ioctl(s, SIOCSIFBRDADDR, &ifr)){
		perror("ioctl-SIOCSIFBRDADDR");
		return -1;
	}
	
	return 0;
}

int setmtu(const int s, const char *ifname, const int mtu)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	ifr.ifr_mtu = mtu;
	if(0 > ioctl(s, SIOCSIFMTU, &ifr)){
		perror("ioctl-SIOCSIFMTU");
		return -1;
	}
	
	return 0;
}

int main()
{
	int s = socket(AF_INET, SOCK_STREAM, 0);
	if(0 > s){
		perror("socket");
		return -1;
	}

	if(0 > setip(s, IFNAME, "192.168.2.211")){
		return -1;
	}

	struct in_addr ip;
	if(0 > getip(s, IFNAME, &ip)){
		return -1;
	}
	printf("%s:ip:%s\n", IFNAME, inet_ntoa(ip));

	setnetmask(s, IFNAME, "255.255.255.0");
	struct in_addr netmask;
	getnetmask(s, IFNAME, &netmask);
	printf("%s:netmask:%s\n", IFNAME, inet_ntoa(netmask));

	setbroadcast(s, IFNAME, "10.111.255.255");
	struct in_addr broadcast;
	getbroadcast(s, IFNAME, &broadcast);
	printf("%s:broadcast:%s\n", IFNAME, inet_ntoa(broadcast));

	setmtu(s, IFNAME, 1024);
	printf("mtu:%d\n", getmtu(s, IFNAME));

	struct sockaddr hwaddr;
	memset(&hwaddr, 0, sizeof(hwaddr));
	gethwaddr(s, IFNAME, &hwaddr);
	printf("MAC ");
	for(int i = 0; i < 6; i++){
		printf("%.2x", (unsigned char)hwaddr.sa_data[i]);
		if(5 != i){
			printf(":");
		}
	}

	printf("\n");

	return 0;	
}

/* ====================================================== */

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


#include <sys/ioctl.h>
/*
 * 功能：读写设备参数，读设备状态，控制设备
 * 参数：
	int d - 文件描述符，表征打开设备
	int request - 命令码
	... - 可变参数
		读写参数时，读状态的参数
 * 返回值：
	失败： -1
 */
int ioctl(int d, int request, ...);

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
#define SIOCGIFHWADDR	0x8927		/* Get hardware address		*/
#define	SIOCSIFHWADDR	0x8924		/* set hardware address 	*/
#define SIOCGIFENCAP	0x8925		/* get/set encapsulations       */
#define SIOCSIFENCAP	0x8926		
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

/* ==================================================== */

1、并行服务器模型（TCP）
A、多进程
空间占用较大
存在进程间通讯

B、多线程
竞态
系统资源消耗（担心不足）


多线程模型框架

int s = socket(...);
bind(s,...);
listen(s,...);

while(1){
	//主线程分工：等连接
	int rws = accpet(s, ...);
	pthread_create(...,thr_proc, (void *)rws);
}

//子线程分工：进行连接读写
void *thr_proc(void *arg)
{
	int rws =(int)arg; 
	while(1){
		...
	}
	return (void *)0;
}



int s = socket(...);
bind(s,...);
listen(s,...);

while(1){
	//主线程分工：等连接
	int rws = accpet(s, ...);
	recv(rws, ....);
	解析请求
	 switch()
	 {
	 case 1:
	 	pthread_create(...,thr_proc, (void *)rws);
		break;
	case 2:
	 	pthread_create(...,thr_proc, (void *)rws);
		break;
	}
}

//子线程分工：进行连接读写
void *thr_proc(void *arg)
{
	int rws =(int)arg; 
	while(1){
		...
	}
	return (void *)0;
}

int s = socket(...);
bind(s,...);
listen(s,...);

while(1){
	//主线程分工：等连接
	int rws = accpet(s, ...);
	recv(rws, ....);
	解析请求
	 switch()
	 {
	 case 1:
	 	pthread_create(...,thr_proc, (void *)rws);
		break;
	case 2:
	 	pthread_create(...,thr_proc, (void *)rws);
		break;
	}
}

//子线程分工：进行连接读写
void *thr_proc(void *arg)
{
	int rws =(int)arg; 
	while(1){
		...
	}
	return (void *)0;
}

int s = socket(...);
bind(s,...);
listen(s,...);

while(1){
	//主线程分工：等连接
	int rws = accpet(s, ...);
 	pthread_create(...,thr_proc, (void *)rws);
}

//子线程分工：进行连接读写
void *thr_proc(void *arg)
{
	int rws =(int)arg; 
	recv();
	解析请求


	while(1){
		...
	}
	return (void *)0;
}



多进程模型框架

int s = socket(...);
bind(s,...);
listen(s,...);

while(1){
	//父进程分工：等连接
	int rws = accpet(s, ...);
	pid_t pid = fork();
	if(0 > pid){
	}else if(0 == pid){
		close(s);
		rw_proc(rws, ...);
		exit(-1);
	}
	close(rws);
}

//子进程分工：进行连接读写
int rw_proc(int rws, void *arg)
{
	while(1){
		...
	}
	return 0;
}
--------------------------------------------
int s = socket(...);
bind(s,...);
listen(s,...);

while(1){
	//父进程分工：等连接
	int rws = accpet(s, ...);
	pid_t pid = fork();
	if(0 > pid){
	}else if(0 == pid){
		close(s);
		rw_proc(rws, ...);
		exit(-1);
	}
	close(rws);
}

//子进程分工：进行连接读写
int rw_proc(int rws, void *arg)
{
	recv();
	解析;
	switch(){
	};
	return 0;
}
=======================================================
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
/*
 * 功能：读/写套接字选项
 * 参数：
	int sockfd - 被操作的套接字
	int level - 选项
		SOL_SOCKET
		IPPROTO_IP
		IPPROTO_TCP
	int optname - 命令码
		SO_REUSEADDR - 端口复用
		（当绑定了端口，进程异常退出，系统没有及时释放端口，这个开关就能够使用系统立即2释放端口）
	void *optval - 指向获得/设置参数的首地址
	socklen_t *optlen - 缓存长度
 * 返回值：
	失败：-1
 */
int getsockopt(int sockfd, int level, int optname,
	      void *optval, socklen_t *optlen);
int setsockopt(int sockfd, int level, int optname,
	      const void *optval, socklen_t optlen);
		  
/* ======================================================== */

#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if_arp.h>

#include <netinet/in.h>
#include <linux/if.h>
#include <sys/ioctl.h>

/*
 * 功能：得到IP地址
 * 参数：
		const int s - 套接字（用于操作网络设备）
		const char *ifname - 网络的物理接口
		struct in_addr *ip - 指向输出的IP地址
 * 返回值：
		失败：-1
 */
int getip(const int s, const char *ifname, struct in_addr *ip)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_addr;
	addr->sin_family = AF_INET;
	if(0 > ioctl(s, SIOCGIFADDR, &ifr)){ //得到IP地址
		perror("ioctl-SIOCGIFADDR");
		return -1;
	}

	*ip = addr->sin_addr;
	
	return 0;
}

/*
 * 功能：得到子网掩码
 * 参数：
		const int s - 套接字（用于操作网络设备）
		const char *ifname - 网络的物理接口
		struct in_addr *netmask - 指向输出的子网掩码
 * 返回值：
		失败：-1
 */
int getnetmask(const int s, const char *ifname, struct in_addr *netmask)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_addr;
	addr->sin_family = AF_INET;
	if(0 > ioctl(s, SIOCGIFNETMASK, &ifr)){ //得到netmask
		perror("ioctl-SIOCGIFNETMASK");
		return -1;
	}

	*netmask = addr->sin_addr;
	
	return 0;
}

/*
 * 功能：得到广播地址
 * 参数：
		const int s - 套接字（用于操作网络设备）
		const char *ifname - 网络的物理接口
		struct in_addr *broadcast - 指向输出的广播地址
 * 返回值：
		失败：-1
 */
int getbroadcast(const int s, const char *ifname, struct in_addr *broadcast)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_addr;
	addr->sin_family = AF_INET;
	if(0 > ioctl(s, SIOCGIFBRDADDR, &ifr)){//得到广播地址
		perror("ioctl-SIOCGIFBRDADDR");
		return -1;
	}

	*broadcast = addr->sin_addr;
	
	return 0;
}

/*
 * 功能：得到mtu值
 * 参数：
		const int s - 套接字（用于操作网络设备）
		const char *ifname - 网络的物理接口
 * 返回值：
		失败：-1
		成功：mtu值
 */
int getmtu(const int s, const char *ifname)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	if(0 > ioctl(s, SIOCGIFMTU, &ifr)){//得到mtu值
		perror("ioctl-SIOCGIFMTU");
		return -1;
	}
	
	return ifr.ifr_mtu;
}

/*
 * 功能：得到MAC地址
 * 参数：
		const int s - 套接字（用于操作网络设备）
		const char *ifname - 网络的物理接口
		struct sockaddr *hwaddr - 指向输出的MAC地址
 * 返回值：
		失败：-1
 */
int gethwaddr(const int s, const char *ifname, struct sockaddr *hwaddr)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;

	if(0 > ioctl(s, SIOCGIFHWADDR, &ifr)){
		perror("ioctl-SIOCGIFHWADDR");
		return -1;
	}

	*hwaddr = ifr.ifr_hwaddr;
	
	return 0;
}

/*
 * 功能：设置MAC地址
 * 参数：
		const int s - 套接字（用于操作网络设备）
		const char *ifname - 网络的物理接口
		const unsigned char addr[6] - 输入的MAC地址
 * 返回值：
		失败：-1
 */
int sethwaddr(const int s, const char *ifname, const unsigned char addr[6])
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	ifr.ifr_hwaddr.sa_family = ARPHRD_ETHER;
	memcpy(ifr.ifr_hwaddr.sa_data, addr, 6);

	if(0 > ioctl(s, SIOCSIFHWADDR, &ifr)){ //
		perror("ioctl-SIOCSIFHWADDR");
		return -1;
	}
	
	return 0;
}

/*
 * 功能：设置IP
 * 参数：
		const int s - 套接字（用于操作网络设备）
		const char *ifname - 网络的物理接口
		const char *ipstr - 输入的IP地址字符串
 * 返回值：
		失败：-1
 */
int setip(const int s, const char *ifname, const char *ipstr)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_addr;
	addr->sin_family     = AF_INET;
	addr->sin_addr.s_addr= inet_addr(ipstr);

	if(0 > ioctl(s, SIOCSIFADDR, &ifr)){
		perror("ioctl-SIOCSIFADDR");
		return -1;
	}
	
	return 0;
}

/*
 * 功能：设置子网掩码
 * 参数：
		const int s - 套接字（用于操作网络设备）
		const char *ifname - 网络的物理接口
		const char *netmask - 输入的子网掩码字符串
 * 返回值：
		失败：-1
 */
int setnetmask(const int s, const char *ifname, const char *netmask)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_addr;
	addr->sin_family     = AF_INET;
	addr->sin_addr.s_addr= inet_addr(netmask);

	if(0 > ioctl(s, SIOCSIFNETMASK, &ifr)){
		perror("ioctl-SIOCSIFNETMASK");
		return -1;
	}
	
	return 0;
}

/*
 * 功能：设置广播地址
 * 参数：
		const int s - 套接字（用于操作网络设备）
		const char *ifname - 网络的物理接口
		const char *netmask - 输入的广播地址字符串
 * 返回值：
		失败：-1
 */
int setbroadcast(const int s, const char *ifname, const char *broadcast)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_addr;
	addr->sin_family     = AF_INET;
	addr->sin_addr.s_addr= inet_addr(broadcast);

	if(0 > ioctl(s, SIOCSIFBRDADDR, &ifr)){
		perror("ioctl-SIOCSIFBRDADDR");
		return -1;
	}
	
	return 0;
}

/*
 * 功能：设置MTU值
 * 参数：
		const int s - 套接字（用于操作网络设备）
		const char *ifname - 网络的物理接口
		const int mtu - 输入的MTU值
 * 返回值：
		失败：-1
 */
int setmtu(const int s, const char *ifname, const int mtu)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);

	ifr.ifr_mtu = mtu;
	if(0 > ioctl(s, SIOCSIFMTU, &ifr)){
		perror("ioctl-SIOCSIFMTU");
		return -1;
	}
	
	return 0;
}

int getifname(const int s, char *ifname, size_t len)
{
	struct ifreq ifr;
	memset(&ifr, 0, sizeof(ifr));  
	struct sockaddr_in *addr = (struct sockaddr_in *)&ifr.ifr_addr;
	addr->sin_family = AF_INET;

	if(0 > ioctl(s, SIOCGIFNAME, &ifr)){
		perror("ioctl-SIOCGIFNAME");
		return -1;
	}

	if(len < strlen(ifr.ifr_name)){
		return -1;
	}
	
	strncpy(ifname, ifr.ifr_name, IFNAMSIZ);
	return 0;
}

void showusage(const char *app)
{
	printf( "Usage: \n"
		"\tread var: %s ifname [ip/mtu/netmask/broadcast/mac] \n", app);
	printf( "\t set var: %s ifname [ip/mtu/netmask/broadcast/mac] [value] \n", app);
}

#define ARG1 1
#define ARG2 2
#define ARG3 3

int main(int num, char *arg[])
{
	int s = socket(AF_INET, SOCK_STREAM, 0);
	if(0 > s){
		perror("socket");
		return -1;
	}

	if(3 == num){
		if(!strcmp("ip", arg[ARG2])){
			struct in_addr ip;
			if(0 > getip(s, arg[ARG1], &ip)){
				goto EXIT_STEP;
			}
			printf("%s:ip:%s\n", arg[ARG1], inet_ntoa(ip));
		}else if(!strcmp("netmask", arg[ARG2])){
			struct in_addr netmask;
			if(0 > getnetmask(s, arg[ARG1], &netmask)){
				goto EXIT_STEP;
			}
			printf("%s:netmask:%s\n", arg[ARG1], inet_ntoa(netmask));
		}else if(!strcmp("broadcast", arg[ARG2])){
			struct in_addr broadcast;
			if(0 > getbroadcast(s, arg[ARG1], &broadcast)){
				goto EXIT_STEP;
			}
			printf("%s:broadcast:%s\n", arg[ARG1], inet_ntoa(broadcast));
		}else if(!strcmp("mtu", arg[ARG2])){
			int mtu = getmtu(s, arg[ARG1]);
			if(0 > mtu){
				goto EXIT_STEP;
			}
			printf("mtu:%d\n", mtu);
		}else if(!strcmp("mac", arg[ARG2])){
			struct sockaddr hwaddr;
			memset(&hwaddr, 0, sizeof(hwaddr));
			if(0 > gethwaddr(s, arg[ARG1], &hwaddr)){
				goto EXIT_STEP;
			}
			printf("MAC ");
			int i;
			for(i = 0; i < 6; i++){
				printf("%.2x", (unsigned char)hwaddr.sa_data[i]);
				if(5 != i){
					printf(":");
				}
			}
			printf("\n");
		}else{
			goto EXIT_STEP;
		}
	}else if(4 == num){
		if(!strcmp("ip", arg[ARG2])){
			if(0 > setip(s, arg[ARG1], arg[ARG3])){
				goto EXIT_STEP;
			}
		}else if(!strcmp("netmask", arg[ARG2])){
			if(0 > setnetmask(s, arg[ARG1], arg[ARG3])){
				goto EXIT_STEP;
			}
		}else if(!strcmp("broadcast", arg[ARG2])){
			if(0 > setbroadcast(s, arg[ARG1], arg[ARG3])){
				goto EXIT_STEP;
			}
		}else if(!strcmp("mtu", arg[ARG2])){
			int mtu = atoi(arg[ARG3]);
			if(0 > setmtu(s, arg[ARG1], mtu)){
				goto EXIT_STEP;
			}
		}else if(!strcmp("mac", arg[ARG2])){
			unsigned char mac[] = {0x11,0x22,0x33,0x44,0x55,0x66};
			if(0 > sethwaddr(s, arg[ARG1], mac)){
				goto EXIT_STEP;
			}
		}else{
			goto EXIT_STEP;
		}
	}else{
		goto EXIT_STEP;
	}
	
	printf("%s done.\n", arg[0]);
	close(s);
	return 0;

EXIT_STEP:
	close(s);
	showusage(arg[0]);

	return 0;	
}

