/*
 * demo for raw socket
 * write by panzhh
 */

#include <stdio.h>
#include <string.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <netinet/tcp.h>
#include <netinet/udp.h> 
#include <arpa/inet.h>
#include <netdb.h>

#include <sys/ioctl.h>
#include <unistd.h>
#include <signal.h>

int getprotocol(const char *protocolname)
{
	struct protoent *protocol = getprotobyname(protocolname);
	if(NULL == protocol){ 
		perror("getprotobyname"); 
		return -1; 
	}

	return protocol->p_proto;
}

#if 0
struct ip
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
	unsigned int ip_hl:4;		/* header length */
	unsigned int ip_v:4;		/* version */
#endif
#if __BYTE_ORDER == __BIG_ENDIAN
	unsigned int ip_v:4;		/* version */
	unsigned int ip_hl:4;		/* header length */
#endif
	u_int8_t ip_tos;		/* type of service */
	u_short ip_len;			/* total length */
	u_short ip_id;			/* identification */
	u_short ip_off;			/* fragment offset field */
#define	IP_RF 0x8000			/* reserved fragment flag */
#define	IP_DF 0x4000			/* dont fragment flag */
#define	IP_MF 0x2000			/* more fragments flag */
#define	IP_OFFMASK 0x1fff		/* mask for fragmenting bits */
	u_int8_t ip_ttl;		/* time to live */
	u_int8_t ip_p;			/* protocol */
	u_short ip_sum;			/* checksum */
	struct in_addr ip_src, ip_dst;	/* source and dest address */
};
#endif

void unpack_iphead(struct ip *iphd)
{
	printf("\nip head: \n");
	printf("\theader length: %u\n", iphd->ip_hl<<2);
	printf("\tversion: %u\n", iphd->ip_v);
	printf("\ttype of service: %u\n", iphd->ip_tos);
	printf("\ttotal length: %u\n", iphd->ip_len);
	printf("\tidentification: %u\n", iphd->ip_id);
	printf("\tfragment offset field: %u\n", iphd->ip_off);
	printf("\ttime to live: %u\n", iphd->ip_ttl);
	if(IPPROTO_TCP == iphd->ip_p)
		printf("\tprotocol: tcp\n");
	else if(IPPROTO_UDP == iphd->ip_p)
		printf("\tprotocol: udp\n");
	else
		printf("\tunkonw protocol : %u\n", iphd->ip_p);
	printf("\tchecksum: %u\n", iphd->ip_sum);
	printf("\tsource address: %s\n", inet_ntoa(iphd->ip_src));
	printf("\tdest address: %s\n", inet_ntoa(iphd->ip_dst));
}

#if 0
struct tcphdr
  {
    u_int16_t source;
    u_int16_t dest;
    u_int32_t seq;
    u_int32_t ack_seq;
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    u_int16_t res1:4;
    u_int16_t doff:4;
    u_int16_t fin:1;
    u_int16_t syn:1;
    u_int16_t rst:1;
    u_int16_t psh:1;
    u_int16_t ack:1;
    u_int16_t urg:1;
    u_int16_t res2:2;
#  elif __BYTE_ORDER == __BIG_ENDIAN
    u_int16_t doff:4;
    u_int16_t res1:4;
    u_int16_t res2:2;
    u_int16_t urg:1;
    u_int16_t ack:1;
    u_int16_t psh:1;
    u_int16_t rst:1;
    u_int16_t syn:1;
    u_int16_t fin:1;
#  else
#   error "Adjust your <bits/endian.h> defines"
#  endif
    u_int16_t window;
    u_int16_t check;
    u_int16_t urg_ptr;
};
#endif

void unpack_tcphead(struct tcphdr *tcphd)
{
	printf("tcp head: \n");
	printf("\tsource port: %u\n", ntohs(tcphd->source));
	printf("\tdestination port: %u\n", ntohs(tcphd->dest));
	printf("\tsequence number: %u\n", ntohl(tcphd->seq));
	printf("\tacknowledgement number: %u\n", ntohl(tcphd->ack_seq));
}

#if 0
struct udphdr
{
  u_int16_t source;/* source port */
  u_int16_t dest;/* destination port */
  u_int16_t len;/* udp length */
  u_int16_t check;/* udp checksum */
};
#endif

void unpack_udphead(struct udphdr *udphd)
{
	printf("udp head: \n");
	printf("\tsource port: %u\n", ntohs(udphd->source));
	printf("\tdestination port: %u\n", ntohs(udphd->dest));
	printf("\tudp length: %u\n", ntohs(udphd->len));
	printf("\tudp checksum: %u\n", ntohs(udphd->check));
}

int transfer_pack(int protocol)
{
	int s = socket(PF_INET, SOCK_RAW, protocol);
	if(0 > s){
		perror("socket");
		return -1;
	}

	while(1){
		struct sockaddr_in addr;
		memset(&addr, 0, sizeof(struct sockaddr_in));
		socklen_t len = sizeof(struct sockaddr);

		#define MAX 65536
		unsigned char buf[MAX];
		memset(buf, 0, MAX);
		int num = recvfrom(s, buf, MAX, 0, (struct sockaddr*)&addr, &len);
		if(0 >= num){
			perror("recvfrom");
			close(s);
			return -1;
		}
		//printf("ip=%s, port=%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

		struct ip *iphd = (struct ip *)buf;
		unpack_iphead(iphd);
		if(IPPROTO_TCP == iphd->ip_p){
			struct tcphdr *tcphd = (struct tcphdr *)(buf+(iphd->ip_hl<<2));
			unpack_tcphead(tcphd);
		}else if(IPPROTO_UDP == iphd->ip_p){
			struct udphdr *udphd = (struct udphdr *)(buf+(iphd->ip_hl<<2));
			unpack_udphead(udphd);
		}else{
			printf("\tunkonw protocol : %u\n", iphd->ip_p);
		}
	}

	return 0;
}

int main(int num , char **arg)
{
#if 0
	signal(SIGCHLD, SIG_IGN);

	pid_t pid = fork();
	if(0 > pid){
	}else if(0 == pid){
		transfer_pack(IPPROTO_TCP);
	}else{
		transfer_pack(IPPROTO_UDP);
	}
#else
	if(2 != num){
		printf("Usage:\n");
		printf("\t %s tcp/udp\n", arg[0]);
		return -1;
	}

	struct protoent *protocol = getprotobyname(arg[1]);
	if(NULL == protocol){ 
		perror("getprotobyname"); 
		return -1; 
	}

	printf("protocol '%s' : %d\n", arg[1], protocol->p_proto);

	transfer_pack(protocol->p_proto);
#endif
	return 0;
}
