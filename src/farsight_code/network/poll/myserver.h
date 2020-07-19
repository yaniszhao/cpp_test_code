/*
 * Demo for epoll
 * writed by panzhh
 * 2013.11.15
 */

#ifndef MYSERVER_H
#define MYSERVER_H

#include <sys/types.h>

/*
 * ���ܣ���ʼ��tcp server
 * ������
	const char *ipstr - ָ��ip���ַ����׵�ַ�����ΪNULL��ϵͳ�Զ���IP
			ʾ����"192.168.1.111"
	u_short port - ָ��server�Ķ˿ں�
	int backlog  - ָ��server���ȴ�������
 * ����ֵ��
	ʧ�ܣ�-1��������Ϣ�鿴errno��
	�ɹ��������׽���	
 */
int init_server(const char *ipstr, u_short port, int backlog);

/*
 * ���ܣ�����ָ���ļ��������Ƿ�����
 * ������
	int fd - ָ���ļ�������
 * ����ֵ��
	ʧ�ܣ�-1��������Ϣ�鿴errno��
 */
int set_nonblock(int fd);

/*
 * ���ܣ������Ӳ����뵽poll����
 * ������
	int s - �����׽���
	struct pollfd *arr - ָ��poll���Ͻṹ�׵�ַ
	const int lenmax   - poll���Ͻṹ�������
 * ����ֵ��
	ʧ�ܣ�-1��������Ϣ�鿴errno��
 */
int newconn(int s, struct pollfd *arr, const int lenmax);

/*
 * ���ܣ����տͻ�������
 * ������
	int rws - ��д�׽���
 * ����ֵ��
	ʧ�ܣ�-1��������Ϣ�鿴errno��
 */
int recvdata(int rws);

#endif

