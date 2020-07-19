#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include <pthread.h>

#define RWSSIZE 10
#define RMAX 1024
/*========================server=================================*/
int init_server(const char *ipstr, u_short port, int backlog);

int myaccept(int s);

void *wait_accept_first(void *s_socket);

void *wait_accept(void *s_socket);

//int get_rws_size(int *rws);

int get_index(int rws);

int recv_msg_send(int rws_s, int rws_d, int *rws);

/*========================client=================================*/
#define IPMAX sizeof("255.255.255.255")
int client_init_connect(unsigned short port, char *ipstr);

int recv_msg(int rws);

int snd_msg(int rws);