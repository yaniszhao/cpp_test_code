#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 9999
#define MULTIIP "224.0.0.0"
#define IPADDR  "10.0.0.111"

int main(int argc, char*argv[])
{
	int s = socket(AF_INET, SOCK_DGRAM, 0);
	if(s < 0){
		perror("socket");
		return -1;
	}

	/*禁止组播数据回送*/
	int loop = 1;
	int ret = setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
	if(ret < 0){
		perror("1-setsockopt");
		return -1;
	}


	struct ip_mreq mreq;
#if 1
	mreq.imr_multiaddr.s_addr = inet_addr(MULTIIP);		/*组播地址*/
	mreq.imr_interface.s_addr = inet_addr(IPADDR);		/*本地IP*/
	//mreq.imr_interface.s_addr = htonl(INADDR_ANY);	/*网络接口为默认*/
	/*将本机加入广播组*/
	ret = setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
	if(ret < 0){
		perror("2-setsockopt");
		return -1;
	}
#endif

	struct sockaddr_in addr;
	memset((void*)&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family     = AF_INET;
	//addr.sin_addr.s_addr= inet_addr("10.0.0.111");
	//addr.sin_addr.s_addr= inet_addr(MULTIIP);
	addr.sin_addr.s_addr= htonl(INADDR_ANY);
	addr.sin_port	    = htons(PORT);
	ret = bind(s, (struct sockaddr*)&addr, sizeof(addr));
	if(ret < 0){
		perror("bind");
		return -1;
	}

	fd_set rfd;
	struct timeval timeout;
	while(1){
		timeout.tv_sec = 2000;
		timeout.tv_usec= 0;

		FD_ZERO(&rfd);
		FD_SET(s, &rfd);

		ret=select(s+1, &rfd, NULL, NULL, &timeout);
		if(0 > ret){
			perror("select");
			return -1;
		} else if(0==ret){
			printf("timeout\n");
			continue;
		} else {	
			if(FD_ISSET(s, &rfd)){
				struct sockaddr_in from_addr;
				memset(&from_addr, 0, sizeof(struct sockaddr_in));
				int len=sizeof(struct sockaddr_in);

				#define MAX	1024
				char buf[MAX+1];
				bzero(buf, MAX+1);
				int num = recvfrom(s, \
						buf, MAX, \
						MSG_NOSIGNAL, \
						(struct sockaddr*)&from_addr, &len);
				if(0 >= num){
					perror("recvfrom");
					break;
				}
				printf("Client[%s:%d] MSG: %s\n",\
					inet_ntoa(from_addr.sin_addr), ntohs(from_addr.sin_port), buf);
			}
		}
	}

	/*退出组播*/
	memset(&mreq, 0, sizeof(mreq));
	mreq.imr_multiaddr.s_addr = inet_addr(MULTIIP);	/*组播地址*/
	mreq.imr_interface.s_addr = inet_addr(IPADDR);	/*网络接口为默认*/
	ret = setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP,&mreq, sizeof(mreq));

	return 0;
}


/* ======================================================= */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if.h>

#define PORT 9999
#define MULTIIP "224.0.0.0"
#define IPADDR  "10.0.0.111"

int main(int argc, char*argv[])
{
	int s=socket(AF_INET, SOCK_DGRAM, 0);
	if(s < 0){
		perror("socket");
		return -1;
	}

	struct in_addr addr;
	addr.s_addr = inet_addr(IPADDR);
	setsockopt(s, IPPROTO_IP, IP_MULTICAST_IF, &addr, sizeof(addr));

	struct sockaddr_in multicast_addr;
	memset(& multicast_addr, 0, sizeof(struct sockaddr_in));
	multicast_addr.sin_family= AF_INET;
	multicast_addr.sin_port  = htons(PORT);
	multicast_addr.sin_addr.s_addr=inet_addr(MULTIIP);

	int k = 0;
	while(1){
		#define MAX	1024
		char buf[MAX];
		sprintf(buf, "NO.%u: multicast ...", k++);
		int ret = sendto(s, \
			buf, strlen(buf),\
			MSG_NOSIGNAL,\
			(struct sockaddr*)&multicast_addr, sizeof(multicast_addr));
		if(0 > ret){
	 		perror("sendto");
			return -1;
		}

		printf("SND[%s:%d] MSG: %s\n",\
			inet_ntoa(multicast_addr.sin_addr),\
			ntohs(multicast_addr.sin_port), buf);

		sleep(1);
	}
	
	return 0;
}
