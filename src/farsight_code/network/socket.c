1���������ǽ��̼�ͨѶ
A����ͬ������ͨѶ
B����ô�����������������������գ�
C����ô�������̣�˭�ա�˭����
���̿����ý��̺������������ǽ����ڳ�������ǰ�޷�Ԥ֪����ϵͳ�Զ�����

����+���� <----> ����+����
����������Ҫ������ȥ
˭�������ݣ�˭��������
=====================================================
TCP/IPЭ����
��Э��ͷ��+���������ݡ���----����Э��ͷ��+���������ݡ�
��ʵ�����е��ʼ�ϵͳ
�������루���
�ռ��ˡ�������

����������ַ
	�ռ���			+ �ż�����
		������
		����������ַ

=====================================================
OSIģ�ͣ�7�㣩
Ӧ�ò㣺���շ�����ʲô
��ʾ�㣺���շ������߼��������ܡ�ѹ�����ȵ�
�Ự�㣺���̺�+�߼����ƣ��˿ںţ�
����㣺�����շ����߼������ɿ�ͨѶ��Ӧ�𣩣�У�飬
����㣺����������������ȥ��·��ѡ�񣨣�
	
	Ч�ʣ��ӿɿ��ԽǶȿ�����������ԽСԽ�ɿ�
		��ͨѶ�Ƕȿ���������Խ��Ч��Խ��
	����취������

��·�㣺�ֽ������ݡ�---��֡����
����㣺֡�����շ�����
===================================================
��ҵ��־
Ӧ�ò㣨Ӧ�ó��򣩣����շ�ʲô				
 ��ҳ��HTTP�����ļ����䣨FTP��

�����(�ں�������ϵͳ��Э���)��˭��˭����port����У�顢�ɿ����

	Ч�ʣ��ӿɿ��ԽǶȿ�����������ԽСԽ�ɿ�
		��ͨѶ�Ƕȿ���������Խ��Ч��Խ��
	�������̫��Ҫ��tcp�� 46~1500�ֽڣ�

�����(�ں�������ϵͳ��Э���)������������������ȥ��IP��
	���飨���Ӧ�ò㷢�����ݰ����󣬴���Э��Լ���Ĵ�С��65536�������飩

�����(�ں�����)���ֽ������ݡ�---��֡����
	֡�����շ�����
========================================================
1�������������
A����ͨ�ļ�
���ļ�������·����----���������ݴ洢
B���ն�
�ַ��豸
�豸�ļ���/dev��----�����ն��豸
C���ܵ�
�ܵ��ļ�����·����---���ҹܵ�
D��IPC����
keyֵ

E������
����ӿ����������豸
�׽��֣���һ�����ͣ�һ��������ļ��������������������
(����TCP/IP���׽���)
--------------------------------------------------
2���˿�
��һ���������ͣ�unsigned short�������ڱ�ʶ����
--------------------------------------------------
3��IP
��һ����������ͣ�IPv4��32bits��unsigned int����������ʶ����
A����ʽ
�������ͣ�A��B��C��D...
�����+������
B����������
������ȡ�����
C��ר��IP
Ʃ�����������ž��ǹ㲥��ַ
�鲥��ַ,....

D��DNS
��������
����---��������httpЭ�飩
IP <----> ����
....

--------------------------------------------------
4����С��
Э��Լ���������ֽ���
uint32_t htonl(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(uint16_t netshort);
================================================
�ַ�����IP������IP��ת��
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*
 * ���ַ���IPת������
	const char *cp - ָ��Ҫ��ת����IP�ַ����׵�ַ
	struct in_addr *inp - ָ��ת���ĳɹ�IP��ַ�ṹ�׵�ַ
 * ����ֵ��
	IP������ֵ
 */
int inet_aton(const char *cp, struct in_addr *inp);
/*
 * ���ַ���IPת������
	const char *cp - ָ��Ҫ��ת����IP�ַ����׵�ַ
 * ����ֵ��
	IP������ֵ
 */
in_addr_t inet_addr(const char *cp);

/*
 * ���ַ���IPת������
	struct in_addr in - IP��ַ
 * ����ֵ��
	IP�ַ������׵�ַ
 */
char *inet_ntoa(struct in_addr in);


netinet/in.h
typedef unsigned int uint32_t;
typedef uint32_t in_addr_t;
struct in_addr
{
	in_addr_t s_addr;
};

//ͨ�õ�ַ�ṹ
struct sockaddr
{    
	u_short  sa_family;    // ��ַ��, AF_xxx
	char  sa_data[14];     // 14�ֽ�Э���ַ
};

//InternetЭ���ַ�ṹ
struct sockaddr_in
{           
	u_short sin_family;      // ��ַ��, AF_INET��2 bytes
	u_short sin_port;      // �˿ڣ�2 bytes
	struct in_addr sin_addr;  // IPV4��ַ��4 bytes 	
	char sin_zero[8];        // 8 bytes unused����Ϊ���
}; 
========================================================
1���׽�������
A����ʽ�׽���
TCP:�������ӵĿɿ�ͨѶ

B�����ݱ��׽���
UDP�������Ӳ��ɿ�ͨѶ

C��ԭʼ�׽���
����������㣬�õ�����IP��


#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

2�������׽���

/*
 * ���ܣ������׽���
	int domain - Э����/��ַ�壺AF_INET IPv4��ַ�塢PF_INET IPv4Э����
	int type   - �׽�������
			SOCK_STREAM - ��ʽ�׽���	--->TCPЭ��
			SOCK_DGRAM  - ���ݱ��׽���	--->UDPЭ��
			SOCK_RAW    - ԭʼ�׽���	--->��Ӧ���������
	int protocol - Э����

 * ����ֵ��
	�ɹ����׽��֣�һ�������ļ���������
		��ʽ�׽��֣����ڵ�����
		���ݰ��׽�������ͨѶ
	ʧ�ܣ�-1
 */
int socket(int domain, int type, int protocol);


3����
A��ϵͳ�Զ���

B������Ա�Լ���
�׽��֣�������IP+�˿ڣ� ��-----���׽��֣�������IP+�˿ڣ�

/*
 * ���ܣ���
 * ������
	int sockfd - ���󶨵��׽���
	const struct sockaddr *addr - �󶨵���Ԫ����Ϣ
				Э����
				IP
				�˿�
	socklen_t addrlen - ��Ԫ����Ϣ�ĳ���
 * ����ֵ��
	ʧ�ܣ�-1
 */
int bind(int sockfd, const struct sockaddr *addr,
                socklen_t addrlen);

4������
TCP����������
A���׽���-����-ԴIP+Դ�˿ڣ�Ŀ��IP+Ŀ��˿ڣ� ��-----���׽���-����-ԴIP+Դ�˿ڣ�Ŀ��IP+Ŀ��˿ڣ�
B���ɿ�
��1���������ӵȴ�����
/*
 * ���ܣ��������ȴ��ļ�����
 * ������
	int sockfd - �������׽��֣�ǰ̨�Ӵ���
	int backlog- ���ȴ�������
 * ����ֵ��
	ʧ�ܣ�-1
 */
int listen(int sockfd, int backlog);

��2��������
/*
 * ���ܣ�����
 * ������
	int sockfd - �׽���
	const struct sockaddr *addr - Ҫ����˭�����ӶԷ�����Ԫ����Ϣ
			Э����
			IP
			�˿�
	socklen_t addrlen - ��Ԫ����Ϣ����
 * ����ֵ��
	ʧ�ܣ�-1
 */
int connect(int sockfd, const struct sockaddr *addr,
                   socklen_t addrlen);
��3����������
/*
 * ���ܣ�������
 * ������
	int sockfd - ���ڼ����׽��֣�ǰ̨�Ӵ���
	const struct sockaddr *addr - �����������˭���������ӷ�����Ԫ����Ϣ
			Э����
			IP
			�˿�
	socklen_t *addrlen - �õ���Ԫ����Ϣ����
 * ����ֵ��
	ʧ�ܣ�-1
	�ɹ����õ����ڶ�д���׽��֣��������ӣ�
 */
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
---------------------------------------------------------------------
������				�ͻ���
1.�����������׽���		1.�����׽���
int s = socket()		int s = socket()

2.����Ԫ����Ϣ
bind(s, IP+PORT,...)		...

3.�������ȴ�������	
listen(s, MAX);			...

4.������			2.׼���÷�������IP�Ͷ˿ڲ�����
int rws = accept(s, ...) <--->	connect(s,����������Ԫ��)

5. ��/������		<--->	3.��/������

read/write(rws, ...)	<--->	write/read(s, ...)

6. �رն�д�׽���(�Ͽ����ӣ�	4.�ر��׽���

7. �ر������׽���
=============================================================
ע������
1������
2��������
3�����ڷ��͵�ʱ�򣬽��ն��쳣�Ͽ�
4��ͬ��
read() <--������һ��--> write();

/* ==================================================== */

//netinet/in.h
typedef uint32_t in_addr_t;
struct in_addr
{
	in_addr_t s_addr;
};

//ͨ�õ�ַ�ṹ
struct sockaddr
{    
	u_short  sa_family;    // ��ַ��, AF_xxx
	char  sa_data[14];     // 14�ֽ�Э���ַ
};

//InternetЭ���ַ�ṹ
struct sockaddr_in
{           
	u_short sin_family;      // ��ַ��, AF_INET��2 bytes
	u_short sin_port;      // �˿ڣ�2 bytes
	struct in_addr sin_addr;  // IPV4��ַ��4 bytes 	
	char sin_zero[8];        // 8 bytes unused����Ϊ���
}; 


socket(port+ip) <-----> socket(port+ip) 

/* =================================================== */

AF ��ʾADDRESS FAMILY ��ַ��
PF ��ʾPROTOCOL FAMILY Э����
���������궨����һ����
����ʹ���ĸ���û�й�ϵ
Winsock2.h��
#define AF_INET 2
#define PF_INET AF_INET
������windows��AF_INET��PF_INET��ȫһ��
����Unix/Linuxϵͳ�У��ڲ�ͬ�İ汾����������΢С���
����BSD,��AF,����POSIX��PF
UNIXϵͳ֧��AF_INET��AF_UNIX��AF_NS�ȣ���DOS,Windows�н�֧��AF_INET����������������
�ں���socketpair��socket��domain��������AF_UNIX,AF_LOCAL,AF_INET,PF_UNIX,PF_LOCAL,PF_INET.
�⼸��������AF_UNIX=AF_LOCAL, PF_UNIX=PF_LOCAL, AF_LOCAL=PF_LOCAL, AF_INET=PF_INET.
����:����socketpair��socket��domain����,ʹ��PF_LOCALϵ��,
���ڳ�ʼ���׽ӿڵ�ַ�ṹʱ,��ʹ��AF_LOCAL.

����:
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
	/*inet_addr֮��õ��Ľ��������һ����
	 *Ӧ��ֻ�ǽ������˴����෴�������˵㡣
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
