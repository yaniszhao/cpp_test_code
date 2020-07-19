#include <sys/types.h>
#include <sys/socket.h>
/*
 * ���ܣ������������ݰ�
 * ������
	int sockfd - �׽���
	const void *buf - ָ�������ݵ��׵�ַ
	size_t len - ����
	int flags - ��־
		MSG_DONTWAIT - ������
		MSG_NOSIGNAL - �Է��Ͽ�����ʱ��������ϢSIGPIPE
 * ����ֵ��
	�ɹ���ʵ�ʷ��͵��ֽ�����TCP��ʽ�׽��֣������뷢���ֽ�����
	ʧ��: -1
 */
ssize_t send(int sockfd, const void *buf, size_t len, int flags);

/*
 * ���ܣ������������ݰ�(ָ�����շ�)
 * ������
	int sockfd - �׽���
	const void *buf - ָ�������ݵ��׵�ַ
	size_t len - ����
	int flags - ��־
		MSG_DONTWAIT - ������
		MSG_NOSIGNAL - �Է��Ͽ�����ʱ��������ϢSIGPIPE
	const struct sockaddr *dest_addr - ָ����շ�����Ԫ����Ϣ��Э���塢IP���˿ڣ�
	socklen_t addrlen - ��Ԫ����Ϣ����
 * ����ֵ��
	�ɹ���ʵ�ʷ��͵��ֽ�����һ���������UDP�������뷢���ֽ�����
	ʧ��: -1
 */
ssize_t sendto(int sockfd, const void *buf, size_t len, int flags,
	      const struct sockaddr *dest_addr, socklen_t addrlen);
/*
 * ���ܣ��������ݰ�(unix�׽��������ڱ��ؽ��̼�ͨѶ)
 * ������
	int sockfd - �׽���
	struct msghdr *msg - ��Ϣ��
	int flags - ��־
 * ����ֵ��
	�ɹ���ʵ�ʷ��͵��ֽ���
	ʧ��: -1
 */
ssize_t sendmsg(int sockfd, const struct msghdr *msg, int flags);
=====================================================================
/*
 * ���ܣ������������ݰ�
 * ������
	int sockfd - �׽���
	const void *buf - ָ����ջ����׵�ַ
	size_t len - ���泤��
	int flags - ��־
		MSG_DONTWAIT - ������
 * ����ֵ��
	�ɹ���ʵ�ʽ��յ��ֽ���
	ʧ��: -1
 */
ssize_t recv(int sockfd, void *buf, size_t len, int flags);

/*
 * ���ܣ������������ݰ�
 * ������
	int sockfd - �׽���
	const void *buf - ָ����ջ����׵�ַ
	size_t len - ���泤��
	int flags - ��־
		MSG_DONTWAIT - ������
	struct sockaddr *src_addr - ָ�򱣴淢�ͷ�����Ԫ����Ϣ�Ľṹ�׵�ַ
	socklen_t *addrlen - ����Է���Ԫ����Ϣ�ṹ�ĳ���
 * ����ֵ��
	�ɹ���ʵ�ʽ��յ��ֽ���
	ʧ��: -1
 */
ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags,
		struct sockaddr *src_addr, socklen_t *addrlen);

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
============================================================
UDP
1��û������
2��ͨѶ���ɿ���������ȥ���ܣ�
3������Է�����Ч�����
4��û��ͬ�����⣨û������
============================================================
SERVER			CLIENT
while(1){		while(1){
	sento			recvfrom
	recvfrom		sento
}			}
============================================================
����0������������һ�������<--->eth0(linuxϵͳ������������ӿ�)<-����Ӳ������->����������0<--->VMnet0(��������)<-�Ž�->PCIe<--->internet
����1������������һ�������<--->eth1(linuxϵͳ������������ӿ�)<-����Ӳ������->����������1<--->VMnet1(��������)<-�Ž�->8139<--->����

