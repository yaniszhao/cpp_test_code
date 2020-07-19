System V IPC对象
1、共享内存(share memory)
进程间共享的内存区

2、消息队列(message queue)
包通讯的队列流（管道通讯的单位是字节，消息队列通讯的单位是包
（包大小可以程序员设定））

3、信号灯(semaphore)
同步技术

=====================================================================
查看方式
# ipcs
------ Shared Memory Segments --------
key        shmid      owner      perms      bytes      nattch     status      
key值	   id号       所有者     权限       大小       使用计数   状态

------ Semaphore Arrays --------
key        semid      owner      perms      nsems     
key值	   id号       所有者     权限       

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages 
key值	   id号       所有者     权限       大小         消息数量

=====================================================================
单个查看
# ipcs -m	共享内存
# ipcs -q	消息队列
# ipcs -s	信号灯
=====================================================================
key值：IPC对象不是“文件”，不同进程间标识同一个IPC对象使用key值
1、如果key为0x00000000，系统创建一次产生一个IPC对象
2、如果key大于0x00000000，只能标识唯一的一个IPC对象，
其他进程只能打开


id号：IPC对象创建打开后，用id来操作IPC对象
（就像“文件”打开后的用文件描述符来标识操作“文件”对象）
=====================================================================

#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/types.h>
#include <sys/ipc.h>
key_t ftok(const char *pathname, int proj_id);

/*
 * 功能：创建或打开
 * 参数：
	key_t key - IPC对象的key值（相当于文件的文件名）
	size_t size - 创建的共享内存的大小
	int shmflg - 标志
		 IPC_CREAT - 创建共享内存
		 IPC_EXCL  - 文件存在
 * 返回值：
	成功 - id(相当于文件描述符)

 */
int shmget(key_t key, size_t size, int shmflg);

#include <sys/types.h>
#include <sys/shm.h>

/*
 * 功能：映射共享内存
 * 参数：
	int shmid - ID(打开的共享内存)
	const void *shmaddr - NULL（指定被映射到进程空间哪个位置？NULL是让系统指定）
	int shmflg - 标志
		 读写权限（限定本进程读写权限）

 * 返回值：
	成功 - 得到映射地址
	失败NULL

 */
void *shmat(int shmid, const void *shmaddr, int shmflg);

/*
 * 功能：解除映射
 * 参数：
	const void *shmaddr - 映射成功的地址值

 * 返回值：
	成功 - 0
	-1 失败

 */
int shmdt(const void *shmaddr);

#include <sys/ipc.h>
#include <sys/shm.h>
/*
 * 功能：共享内存通用操作方法
 * 参数：
	int shmid - id(打开的共享内存对象)
	int cmd - 命令码
		IPC_RMID 删除共享内存
		

 * 返回值：
	成功 - 0
	-1 失败

 */
int shmctl(int shmid, int cmd, struct shmid_ds *buf);
----------------------------------------------------------
共享内存操作

1.创建
2.if(失败){
	if(是否存在){
		否
		打印错误信息退出
	}
}

3.映射
4.操作地址完成对共享内存
5.解除映射

6.删除共享内存
