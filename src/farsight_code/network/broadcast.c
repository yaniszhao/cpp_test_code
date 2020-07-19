
同步：拆包或黏包都会造成收发不一致

snd 端（小端）

len(2bytes)+图片正文,len(2bytes)+程序正文,len(2bytes)+文本正文
-------------------------------
recv端（大端）

while(1){
	recv(2)
	int len ;
	char *p = buf;
	while(len){
		int num = recv(s, p, len);
		p   += num;
		len -= num;
	}
}
======================================
广播:在一个网络内，通过网络广播地址进行广播数据包
组播：通过特定的IP地址向一个组进行数据包的广播
=============================================================
udp实现可靠通讯

1、应答重发逻辑演示
	int err_num = 0;
START：
	sendto();						recvfrom()

	fd_set rfd;
	struct timeval timeout;

	while(1){
	timeout.tv_sec = 1000;
		timeout.tv_usec= 0;

		FD_ZERO(&rfd);
		FD_SET(s, &rfd);

		ret=select(s+1, &rfd, NULL, NULL, &timeout);
		if(0 > ret){
			perror("select");
			return -1;
		} else if(0==ret){
			printf("timeout\n");
			err_num++;
			if(err_num == 10){
				break;
			}
			goto START;
		} else {	
			int num = recvfrom(s, \
					buf, MAX, \
					MSG_NOSIGNAL, \
					(struct sockaddr*)&from_addr, &len);
			}
		}
	}

2、如果单次数据包过大，拆包，并顺序可靠发包
========================================================
#include <netdb.h>

 struct protoent {
               char  *p_name;       /* official protocol name */
               char **p_aliases;    /* alias list */
               int    p_proto;      /* protocol number */
           }
/*
 * 功能：得到协议编号
 * 参数：
	const char *name - 协议名称
		tcp
		udp
 * 返回值：
	失败：NULL
 */
struct protoent *getprotobyname(const char *name);


struct hostent {
               char  *h_name;            /* official name of host */
               char **h_aliases;         /* alias list */
               int    h_addrtype;        /* host address type */
               int    h_length;          /* length of address */
               char **h_addr_list;       /* list of addresses */
           }
#define h_addr h_addr_list[0] /* for backward compatibility */

/*
 * 功能：得到指定主机的IP地址
 * 参数：
	const char *name - 主机名
 * 返回值
	失败:NULL
 */
struct hostent *gethostbyname(const char *name);

/* =================================================== */

单播、多播、广播、组播、泛播、冲突域、广播域 
假设X代表所有的机器，Y代表X中的一部分机器，Z代表一组机器，1代表一台机器，那么
1：1 那就是单播；
1：Y 那就是多播；
1：X 那就是广播；
1：Z 那就是组播；
Y=X时，多播就是广播；Y=Z多播就是组播；
泛播也叫任意播，是指某组中任意发送方对应拓朴结构中几个最接近的接收方之间的通信。
而组播是指单个发送方对应一组选定接收方的一种通信。

一、什么是组播
1、组播的特点
1）什么是组播？
组播是一种数据包传输方式，当有多台主机同时成为一个数据包的接受者时，出于对带宽和CPU负担的考虑，组播成为了一种最佳选择。
2）组播如何进行工作？
    组播通过把224.0.0.0-239.255.255.255的D类地址作为目的地址，有一台源主机发出目的地址是以上范围组播地址的报文，在网络中，如果有其他主机对于这个组的报文有兴趣的，可以申请加入这个组，并可以接受这个组，而其他不是这个组的成员是无法接受到这个组的报文的。
3）组播和单播的区别？
   为了让网络中的多个主机可以同时接受到相同的报文，如果采用单播的方式，那么源主机必须不停的产生多个相同的报文来进行发送，对于一些对时延很敏感的数据，在源主机要产生多个相同的数据报文后，在产生第二个数据报文，这通常是无法容忍的。而且对于一台主机来说，同时不停的产生一个报文来说也是一个很大的负担。如果采用组播的方式，源主机可以只需要发送一个报文就可以到达每个需要接受的主机上，这中间还要取决于路由器对组员和组关系的维护和选择。
4）组播和广播的区别？
   如同上个例子，当有多台主机想要接收相同的报文，广播采用的方式是把报文传送到局域网内每个主机上，不管这个主机是否对报文感兴趣。这样做就会造成了带宽的浪费和主机的资源浪费。而组播有一套对组员和组之间关系维护的机制，可以明确的知道在某个子网中，是否有主机对这类组播报文感兴趣，如果没有就不会把报文进行转发，并会通知上游路由器不要再转发这类报文到下游路由器上。
2、组播的缺点：
1) 与单播协议相比没有纠错机制，发生丢包错包后难以弥补，但可以通过一定的容错机制和QOS加以弥补。
2) 现行网络虽然都支持组播的传输，但在客户认证、QOS等方面还需要完善，这些缺点在理论上都有成熟的解决方案，只是需要逐步推广应用到现存网络当中。


二、单播：

1、单播的定义
    主机之间“一对一”的通讯模式，网络中的交换机和路由器对数据只进行转发不进行复制。如果10个客户机需要相同的数据，则服务器需要逐一传送，重复10次相同的工作。但由于其能够针对每个客户的及时响应，所以现在的网页浏览全部都是采用IP单播协议。网络中的路由器和交换机根据其目标地址选择传输路径，将 IP单播数据传送到其指定的目的地。
2、单播的优点：
1）服务器及时响应客户机的请求
2）服务器针对每个客户不通的请求发送不通的数据，容易实现个性化服务。
3、单播的缺点：
1）服务器针对每个客户机发送数据流，服务器流量＝客户机数量×客户机流量；在客户数量大、每个客户机流量大的流媒体应用中服务器不堪重负。
2）现有的网络带宽是金字塔结构，城际省际主干带宽仅仅相当于其所有用户带宽之和的5％。如果全部使用单播协议，将造成网络主干不堪重负。现在的P2P应用就已经使主干经常阻塞，只要有5％的客户在全速使用网络，其他人就不要玩了。而将主干扩展20倍几乎是不可能。

三、 广播

1、广播的定义
主机之间“一对所有”的通讯模式，网络对其中每一台主机发出的信号都进行无条件复制并转发，所有主机都可以接收到所有信息（不管你是否需要），由于其不用路径选择，所以其网络成本可以很低廉。有线电视网就是典型的广播型网络，我们的电视机实际上是接受到所有频道的信号，但只将一个频道的信号还原成画面。在数据网络中也允许广播的存在，但其被限制在二层交换机的局域网范围内，禁止广播数据穿过路由器，防止广播数据影响大面积的主机。
2、广播的优点：
1）网络设备简单，维护简单，布网成本低廉
2）由于服务器不用向每个客户机单独发送数据，所以服务器流量负载极低。
3、广播的缺点：
1）无法针对每个客户的要求和时间及时提供个性化服务。
2）网络允许服务器提供数据的带宽有限，客户端的最大带宽＝服务总带宽。例如有线电视的客户端的线路支持100个频道（如果采用数字压缩技术，理论上可以提供 500个频道），即使服务商有更大的财力配置更多的发送设备、改成光纤主干，也无法超过此极限。也就是说无法向众多客户提供更多样化、更加个性化的服务。
3） 广播禁止在Internet宽带网上传输。


四、多播

1、多播的定义
“多播”可以理解为一个人向多个人（但不是在场的所有人）说话，这样能够提高通话的效率。如果你要通知特定的某些人同一件事情，但是又不想让其他人知道，使用电话一个一个地通知就非常麻烦，而使用日常生活的大喇叭进行广播通知，就达不到只通知个别人的目的了，此时使用“多播” 来实现就会非常方便快捷，但是现实生活中多播设备非常少。多播包括组播和广播，组播是多播的一种表现形式。

2、多播的特点
    广播和多播仅应用于UDP，它们对需将报文同时传往多个接收者的应用来说十分重要。TCP是一个面向连接的协议，它意味着分别运行于两主机（由IP地址确定）内的两进程（由端口号确定）间存在一条连接。
   考虑包含多个主机的共享信道网络如以太网。每个以太网帧包含源主机和目的主机的以太网地址（48 bit）。通常每个以太网帧仅发往单个目的主机，目的地址指明单个接收接口，因而称为单播(unicast)。在这种方式下，任意两个主机的通信不会干扰网内其他主机（可能引起争夺共享信道的情况除外）。然而，有时一个主机要向网上的所有其他主机发送帧，这就是广播。通过ARP和RARP可以看到这一过程。多播(multicast) 处于单播和广播之间：帧仅传送给属于多播组的多个主机。


五、泛洪

1、泛洪的定义

在CISCO设备上有个概念叫泛洪。如果有学设备的，我这里顺便对比讲下，比如现在有个信息包进来，从我们交换机某个端口进来，他要寻质如果事先没有地址那么他就会给每个端口发信息，包括自己，来求证是否是对方。所以交换机不能屏蔽广播而路由器可以，因为他有记忆功能,可以形成路由表。而设备上的泛洪呢他的意思是给所有端口发除了自己，大家对记下这两个概念.而我们IP地址上的广播地址的概念：IP地址是由2进制组成，当全为一时，表示广播地址。广播地址主机都为1.主机全为0的话是网段表示一个网。

2、网络泛洪

从定义上说，攻击者对网络资源发送过量数据时就发生了洪水攻击，这个网络资源可以是router，switch，host，application等。常见的洪水攻击包含MAC泛洪，网络泛洪，TCP SYN泛洪和应用程序泛洪。接下来简单的分别解释一下以上这些：
    1)MAC泛洪发生在OSI第二层，攻击者进入LAN内，将假冒源MAC地址和目的MAC地址将数据帧发送到以太网上导致交换机的内容可寻址存储器（CAM）满掉，然后交换机失去转发功能，导致攻击者可以像在共享式以太网上对某些帧进行嗅探，这种攻击可以通过端口安全技术方式，比如端口和MAC地址绑定。
    2)网络泛洪包括Smurf和DDos：
    smurf发生在OSI第三层，就是假冒ICMP广播ping，如果路由器没有关闭定向广播，那攻击者就可以在某个网络内对其它网络发送定向广播 ping，那个网络中的主机越是多，造成的结果越是严重，因为每个主机默认都会响应这个ping，导致链路流量过大而拒绝服务，所以属于增幅泛洪攻击，当然也可以对本            网络发送广播ping。
    3)DDos发生在OSI第三、四层，攻击侵入许多因特网上的系统，将DDos控制软件安装进去，然后这些系统再去感染其它系统，通过这些代理，攻击者将攻击指令发送给DDos控制软件，然后这个系统就去控制下面的代理系统去对某个IP地址发送大量假冒的网络流量，然后受攻击者的网络将被这些假的流量所占据就无法为他们的正常用户提供服务了。
    4)TCP SYN泛洪发生在OSI第四层，这种方式利用TCP协议的特性，就是三次握手。攻击者发送TCP SYN，SYN是TCP三次握手中的第一个数据包，而当服务器返回ACK后，改攻击者就不对之进行再确认，那这个TCP连接就处于挂起状态，也就是所谓的半连接状态，服务器收不到再确认的话，还会重复发送ACK给攻击者。这样更加会浪费服务器的资源。攻击者就对服务器发送非常大量的这种TCP连接，由于每一个都没法完成三次握手，所以在服务器上，这些TCP连接会因为挂起状态而消耗CPU和内存，最后服务器可能死机，就无法为正常用户提供服务了。
    5）最后应用程序泛洪发生在OSI第七层，目的是消耗应用程序或系统资源，比较常见的应用程序泛洪是什么呢？没错，就是垃圾邮件，但一般无法产生严重的结果。其它类型的应用程序泛洪可能是在服务器上持续运行高CPU消耗的程序或者用持续不断的认证请求对服务器进行泛洪攻击，意思就是当TCP连接完成后，在服务器提示输入密码的时候停止响应。
冲突域（物理分段）：连接在同一导线上的所有工作站的集合，或者说是同一物理网段上所有节点的集合或以太网上竞争同一带宽的节点集合。也就是说，用Hub或者Repeater连接的所有节点可以被认为是在同一个冲突域内，它不会划分冲突域。由于广播域被认为是OSI中的第二层概念，所以象 Hub，交换机等第一，第二层设备连接的节点被认为都是在同一个广播域。

冲突域（Collision Domain）：一组与同一条物理介质相连的设备，其中任何两台设备同时访问该介质都将导致冲突，冲突域中同一时间内只有一台机器能够发送数据。
广播域（Broadcast Domain）：网络中一组相互接收广播消息的设备。

第一层设备如集线器，与之连接的所有设备都属于同一个冲突域和同一个广播域；
第二层设备如交换机和网桥，将网络划分成多个网段，每个网段是一个独立的冲突域，但是相连的所有设备是一个广播域，交换机的每个端口就是一个冲突域；
第三层设备如路由器，将网络划分为多个冲突域和广播域。
以太网使用载波侦听多路访问/冲突检测（Carrier Sense Multi-Access/Collision Detection）技术以减少冲突的发生。
即，二层广播帧覆盖的范围就是广播域；二层单播帧覆盖的范围就是冲突域。

/* ============================================================ */

多播的概念

多播，也称为“组播”，将网络中同一业务类型主机进行了逻辑上的分组，进行数据收发的时候其数据
仅仅在同一分组中进行，其他的主机没有加入此分组不能收发对应的数据。

在广域网上广播的时候，其中的交换机和路由器只向需要获取数据的主机复制并转发数据。
主机可以向路由器请求加入或退出某个组，网络中的路由器和交换机有选择地复制并传输数据，
将数据仅仅传输给组内的主机。多播的这种功能，可以一次将数据发送到多个主机，
又能保证不影响其他不需要（未加入组）的主机的其他通信。

相对于传统的一对一的单播，多播具有如下的优点：
1、具有同种业务的主机加入同一数据流，共享同一通道，节省了带宽和服务器的优点，具有广播的优点
而又没有广播所需要的带宽。
2、服务器的总带宽不受客户端带宽的限制。由于组播协议由接收者的需求来确定是否进行数据流的转发，
所以服务器端的带宽是常量，与客户端的数量无关。
3、与单播一样，多播是允许在广域网即Internet上进行传输的，而广播仅仅在同一局域网上才能进行。

组播的缺点：
1、多播与单播相比没有纠错机制，当发生错误的时候难以弥补，但是可以在应用层来实现此种功能。
2、多播的网络支持存在缺陷，需要路由器及网络协议栈的支持。

多播的应用主要有网上视频、网上会议等。

=================================================================

例如本地计算机的的IP地址是：127.0.0.1，它的多播地址是： 224.0.0.1。这是由RCF 1390定义的。
为发送IP多播数据，发送者需要确定一个合适的多播地址，这个地址代表一个组。IPv4多播地址采用
D类IP地址确定多播的组。在Internet中，多播地址范围是从224.0.0.0到234.255.255.255。其中比较
重要的地址有：
224.0.0.1 －网段中所有支持多播的主机
224.0.0.2 －网段中所有支持多播的路由器
224.0.0.4 －网段中所有的DVMRP路由器
224.0.0.5 －所有的OSPF路由器
224.0.0.6 －所有的OSPF指派路由器
224.0.0.9 －所有RIPv2路由器

广域网的多播
多播的地址是特定的，D类地址用于多播。D类IP地址就是多播IP地址，即224.0.0.0至239.255.255.255
之间的IP地址，并被划分为局部连接多播地址、预留多播地址和管理权限多播地址3类：
局部多播地址：在224.0.0.0～224.0.0.255之间，这是为路由协议和其他用途保留的地址，路由器并不转发属于此范围的IP包。
预留多播地址：在224.0.1.0～238.255.255.255之间，可用于全球范围（如Internet）或网络协议。
管理权限多播地址：在239.0.0.0～239.255.255.255之间，可供组织内部使用，类似于私有IP地址，不能用于Internet，可限制多播范围。

=================================================================

getsockopt()/setsockopt()的选项		含义
IP_MULTICAST_TTL			设置多播组数据的TTL值
IP_ADD_MEMBERSHIP			在指定接口上加入组播组
IP_DROP_MEMBERSHIP			退出组播组
IP_MULTICAST_IF				获取默认接口或设置接口
IP_MULTICAST_LOOP			禁止组播数据回送

1、选项IP_MULTICASE_TTL
选项IP_MULTICAST_TTL允许设置超时TTL，范围为0～255之间的任何值，例如：
unsigned char ttl=255;
setsockopt(s,IPPROTO_IP,IP_MULTICAST_TTL,&ttl,sizeof(ttl));

2、选项IP_MULTICAST_IF
选项IP_MULTICAST_IF用于设置组播的默认默认网络接口，会从给定的网络接口发送，
另一个网络接口会忽略此数据。例如：
struct in_addr addr;
setsockopt(s,IPPROTO_IP,IP_MULTICAST_IF,&addr,sizeof(addr));
参数addr是希望多播输出接口的IP地址，使用INADDR_ANY地址回送到默认接口。

3、选项IP_MULTICAST_LOOP
默认情况下，当本机发送组播数据到某个网络接口时，在IP层，数据会回送到本地的回环接口，
选项IP_MULTICAST_LOOP用于控制数据是否回送到本地的回环接口。例如：
unsigned char loop;
setsockopt(s,IPPROTO_IP,IP_MULTICAST_LOOP,&loop,sizeof(loop));
参数loop设置为0禁止回送，设置为1允许回送。

4、选项IP_ADD_MEMBERSHIP和IP_DROP_MEMBERSHIP
加入或者退出一个组播组，通过选项IP_ADD_MEMBERSHIP和IP_DROP_MEMBER- SHIP，
对一个结构struct ip_mreq类型的变量进行控制，struct ip_mreq原型如下：
struct ip_mreq
{
	struct in_addr imn_multiaddr; /*加入或者退出的广播组IP地址*/
	struct in_addr imr_interface; /*加入或者退出的网络接口IP地址*/
};

选项IP_ADD_MEMBERSHIP用于加入某个广播组，之后就可以向这个广播组发送数据或者
从广播组接收数据。此 选项的值为mreq结构，成员imn_multiaddr是需要加入的广播组IP地址，
成员imr_interface是本机需要加入广播组的网络接口 IP地址。例如：
struct ip_mreq mreq;
setsockopt(s,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
使用IP_ADD_MEMBERSHIP选项每次只能加入一个网络接口的IP地址到多播组，但并不是一个多播组
仅允许一 个主机IP地址加入，可以多次调用IP_ADD_MEMBERSHIP选项来实现多个IP地址加入同一个
广播组，或者同一个IP地址加入多个广播组。当 imr_ interface为INADDR_ANY时，选择的是默认
组播接口。
选项IP_DROP_MEMBERSHIP
选项IP_DROP_MEMBERSHIP用于从一个广播组中退出。例如：
struct ip_mreq mreq;
setsockopt(s,IPPROTP_IP,IP_DROP_MEMBERSHIP,&mreq,sizeof(sreq));
其中mreq包含了在IP_ADD_MEMBERSHIP中相同的值。

=================================================================

多播程序设计的框架
（1）建立一个socket。
（2）然后设置多播的参数，例如超时时间TTL、本地回环许可LOOP等。
（3）加入多播组。
（4）发送和接收数据。
（5）从多播组离开。

=================================================================

1、组播和广播需要在局域网内才能实现，另外得查看linux系统是否支持多播和广播：ifconfig
     UP BROADCAST MULTICAST MTU:1500  跃点数:1
     说明该网卡支持
2、发送多播包的主机需要设置网关，否则运行sendto()会出现"network is unreachable"，
网卡可以随便设置，但是一定要设。还要添加路由240.0.0.0，即：
      route add -net 224.0.0.0 netmask 240.0.0.0 dev eth0
      route add default gw "192.168.40.1 " dev eth0
3 、出现：“setsockopt:No such device”。
What does "IP_ADD_MEMBERSHIP: No such device" mean? 
It means that the tool is trying to use multicast but the network interface doesn't support it There are two likely causes: 
·Your machine doesn't have multicast support enabled. For example, on Linux and FreeBSD it is possible to compile a kernel which doesn't support multicast.  
 ·You don't have a route for multicast traffic. Some systems don't add this by default, and you need to run：
# route add -net 224.0.0.0 netmask 224.0.0.0 eth0(or similar). If you wish to use RAT in unicast mode only, it is possible to add the multicast route on the loopback interface.
我一般都这样写：
xxx.imr_interface.s_addr = htonl(localIP);

/* ======================================================= */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{	
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if(0 > s){
		perror("socket");
		return -1;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(9876);
	addr.sin_addr.s_addr = INADDR_ANY;
	//addr.sin_addr.s_addr = inet_addr("10.0.0.111");
	memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
	socklen_t len = sizeof(struct sockaddr_in);
	if(0 > bind(s, (struct sockaddr *)&addr, len)){
		perror("bind");
		return -1;
	}

	while(1){
		#define MAX 1024
		char buf[MAX];
		memset(buf, 0, MAX);
		if(0 >= recv(s, buf, MAX-1, 0)){
			perror("recv");
		}
		
		printf("REV:%s\n", buf);
	}
}

/* ======================================================= */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{	
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if(0 > s){
		perror("socket");
		return -1;
	}

	int on = 1;
 	if(0 > setsockopt(s, SOL_SOCKET, SO_BROADCAST, &on, sizeof(int))){
		perror("setsockopt");
		return -1;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port   = htons(9876);
	addr.sin_addr.s_addr = inet_addr("10.255.255.255");
	memset(addr.sin_zero, 0, sizeof(addr.sin_zero));
	socklen_t len = sizeof(struct sockaddr_in);

	int id = 0;
	while(1){
		#define MAX 1024
		char buf[MAX];
		snprintf(buf, MAX, "NO.%d: hello world", id++);
		if(0 >= sendto(s, buf, strlen(buf), 0, (struct sockaddr*)&addr, len)){
			perror("sendto");
		}
		
		printf("SND:%s\n", buf);
		//sleep(1);
	}
}
