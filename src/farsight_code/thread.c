{
#include <pthread.h>
/*
 * 功能：创建子线程
 * 参数：
	pthread_t *thread - 输出参数，如果线程创建成功则指向得到的线程ID
	const pthread_attr_t *attr - 线程属性
				用于指定创建的线程栈区、优先级、分离属性，等等。
	void *(*start_routine) (void *) - 函数指针，线程函数
	void *arg - 线程函数的参数
 * 返回值：
	0成功
	失败 错误码
 */
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
		  void *(*start_routine) (void *), void *arg);


注册回调

typedef int (*PFUN)(void *arg);

int registe_pfun(PFUN pfun, void *arg)
{
	return pfun(arg);
}

//----------------------------------------
int test(void *arg)
{
	return printf("test: %s\n", (char *)arg);
}

int main()
{
	registe_pfun(test, "hello");
}
---------------------------------------------
指针？
指针是一个特殊的整型变量，默认是存储别的变量的地址
地址是一个整型量，

==================================================
/*
 * 功能：等子线程退出
 * 参数：
	pthread_t thread - 等谁？ 线程ID指定
	void **retval - 得到线程的返回值
 * 返回值：
	0成功
	失败 错误码
 */
int pthread_join(pthread_t thread, void **retval);
================================================
竞态：多个执行序列同时竞争一个资源就是竞态

竞态产生条件
A、并行
多个执行序列一起运行

B、抢占

解决竞态的办法
A、同步
让执行序列串行，先执行一个序列，才执行一个序列

B、互斥
排他，一个序列使用资源，另一个序列就不能使用

临界区：被保护的区域

粒度：临界区大小

函数重入：
	当函数被一个线程调用的时候，另一个线程同时在调用该函数，则函数重入

线程安全：
	函数重入可能引起线程运行产生竞态，则叫线程不安全函数
	库函数是线程安全的吗？
	printf();

=================================================
信号量：系统一个特殊的值，其操作规则为：可以进行加减，当信号量值为0时不能进行减。
P操作：减1
V操作：加1

1、信号量类型
sem_t

2、初始化函数
/*
 * 功能：初始化信号量
 * 参数：
	sem_t *__sem - 指向被初始化的信号量
	int __pshared - 0在线程内，
	unsigned int __value - 初值（1、0）
 * 返回值：
	0 成功
	失败 - 错误码
 */
int sem_init (sem_t *__sem, int __pshared, unsigned int __value);

/*
 * 功能：销毁信号量
 * 参数：
	sem_t *__sem - 指向被销毁的信号量
 * 返回值：
	0 成功
	失败 - 错误码
 */
int sem_destroy (sem_t *__sem);

/*
 * 功能：P操作（try版本）
 * 参数：
	sem_t *__sem - 指向被操作的信号量
 * 返回值：
	0 成功
	失败-错误码（当信号量已经为0是，一定不能进行P操作，该函数立即报错返回）
 */
int sem_trywait (sem_t *__sem);
/*
 * 功能：P操作（阻塞版本，当信号量已经为0，则休眠等，直到信号量>=1，再进行P操作）
 * 参数：
	sem_t *__sem - 指向被操作的信号量
 * 返回值：
	0 成功
	失败-错误码
 */
int sem_wait (sem_t *__sem);

/*
 * 功能：V操作
 * 参数：
	sem_t *__sem - 指向被操作的信号量
 * 返回值：
	0 成功
	失败-错误码
 */
int sem_post (sem_t *__sem);
--------------------------------------------------
序列1			序列2
P操作			P操作
临界区			临界区
V操作			V操作
--------------------------------------------------
使用范围：用于同步
--------------------------------------------------
范例：生产者消费者问题：
	sem_a(0) sem_b(1)
序列1			序列2
Pa操作			Pb操作
临界区			临界区
Vb操作			Va操作
--------------------------------------------------
阻塞：当某种执行的条件不满足时，则休眠等，直到条件满足完成事件返回。
=================================================
互斥锁：一种锁机制（加锁/解锁），用于互斥
---------------------------------
序列1		序列2
加锁		加锁
临界区		临界区
解锁		解锁
---------------------------------
1、互斥锁类型
pthread_mutex_t

2、初始化函数
/*
 * 功能：初始化互斥锁
 * 参数：
	pthread_mutex_t *__mutex - 指向被初始化的互斥锁
	__const pthread_mutexattr_t *__mutexattr - 指向互斥的属性（一般使用缺省属性NULL）
 * 返回值：
	0 成功
	失败-错误码
 */
int pthread_mutex_init (pthread_mutex_t *__mutex, __const pthread_mutexattr_t *__mutexattr);
3、销毁函数
/*
 * 功能：销毁互斥锁
 * 参数：
	pthread_mutex_t *__mutex - 指向被销毁的互斥锁
 * 返回值：
	0 成功
	失败-错误码
 */
int pthread_mutex_destroy (pthread_mutex_t *__mutex);

4、加锁函数
/*
 * 功能：try加锁（如果加锁不成功，立即报错返回）
 * 参数：
	pthread_mutex_t *__mutex - 指向互斥锁
 * 返回值：
	0 成功
	失败-错误码
 */
int pthread_mutex_trylock (pthread_mutex_t *__mutex);
/*
 * 功能：加锁（如果加锁不成功，则休眠等，直到加锁成功返回）
 * 参数：
	pthread_mutex_t *__mutex - 指向互斥锁
 * 返回值：
	0 成功
	失败-错误码
 */
int pthread_mutex_lock (pthread_mutex_t *__mutex);

5、解锁函数
/*
 * 功能：解锁
 * 参数：
	pthread_mutex_t *__mutex - 指向互斥锁
 * 返回值：
	0 成功
	失败-错误码
 */
int pthread_mutex_unlock (pthread_mutex_t *__mutex);
====================================================
死锁：逻辑上造成P/V操作或锁操作

=======================================
1、条件变量类型：pthread_cond_t
2、初始化函数
/*
 * 功能：初始化条件变量
 * 参数：
	pthread_cond_t *cond - 指向被初始化的条件变量
	__const pthread_condattr_t *attr - 指向被初始化的条件变量属性（NULL缺省属性）
 * 返回值：
	0 成功
	失败-错误码
 */
int pthread_cond_init (pthread_cond_t *cond, __const pthread_condattr_t *attr)

/*
 * 功能：销毁条件变量
 * 参数：
	pthread_cond_t *cond - 指向被销毁的条件变量

 * 返回值：
	0 成功
	失败-错误码
 */
int pthread_cond_destroy (pthread_cond_t *__cond);


/*
 * 功能：挂起线程（在指定的条件变量上休眠）
 * 参数：
	pthread_cond_t *cond - 指向条件变量
	pthread_mutex_t * __mutex - 互斥锁
 * 返回值：
	0 成功
	失败-错误码
 */
int pthread_cond_wait (pthread_cond_t *__cond,
			      pthread_mutex_t * __mutex);
/*
 * 功能：唤醒线程（在指定的条件变量上休眠的线程被唤醒）
 * 参数：
	pthread_cond_t *cond - 指向条件变量
 * 返回值：
	0 成功
	失败-错误码
 */
int pthread_cond_signal (pthread_cond_t *__cond);
}

/* ================================================= */

1.    线程属性：
使用pthread_attr_t类型表示，我们需要对此结构体进行初始化，

初始化后使用，使用后还要进行去除初始化！
 pthread_attr_init:初始化
 pthread_attr_destory:去除初始化       

#include <pthread.h>
int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destroy(pthread_attr_t *attr);   
若成功返回0，若失败返回-1。

pthread_attr_init之后，pthread_t结构所包含的内容就是操作系统实现
支持的线程所有属性的默认值。

如果pthread_attr_init实现时为属性对象分配了动态内存空间，
pthread_attr_destroy还会用无效的值初始化属性对象，因此如果经
pthread_attr_destroy去除初始化之后的pthread_attr_t结构被
pthread_create函数调用，将会导致其返回错误。

线程属性结构如下：

typedef struct
{
	   int                           detachstate;     线程的分离状态
	   int                          schedpolicy;   线程调度策略
	   struct sched_param      schedparam;   线程的调度参数
	   int                          inheritsched;    线程的继承性
	   int                          scope;          线程的作用域
	   size_t                      guardsize; 线程栈末尾的警戒缓冲区大小
	   int                          stackaddr_set;
	   void *                     stackaddr;      线程栈的位置
	   size_t                      stacksize;       线程栈的大小
}pthread_attr_t;

下面主要讨论此结构体！！！

2.    分离状态:
线程的分离状态决定一个线程以什么样的方式来终止自己。

我们已经在前面已经知道，在默认情况下线程是非分离状态的，这种情况   
下，原有的线程等待创建的线程结束。只有当pthread_join() 函数返回       
时，创建的线程才算终止，才能释放自己占用的系统资源。   

分离线程没有被其他的线程所等待，自己运行结束了，线程也就终止了，
马上释放系统资源。

通俗的说也就是：我们知道一般我们要等待(pthread_join)一个线程的结束，
主要是想知道它的结束状态，否则等待一般是没有什么意义的！但是if有一
些线程的终止态我们压根就不想知道，那么就可以使用“分离”属性，那么我
们就无须等待管理，只要线程自己结束了，自己释放src就可以咯！这样更
方便！

#include <pthread.h>
int pthread_attr_getdetachstate(const pthread_attr_t * attr, int * detachstate);
int pthread_attr_setdetachstate(pthread_attr_t * attr, int detachstate);
参数：attr:线程属性变量
		detachstate:分离状态属性   
若成功返回0，若失败返回-1。

设置的时候可以有两种选择：
<1>.detachstate参数为：PTHREAD_CREATE_DETACHED     分离状态启动
<2>.detachstate参数为：PTHREAD_CREATE_JOINABLE    正常启动线程
           
3.    线程的继承性：
           
函数pthread_attr_setinheritsched和pthread_attr_getinheritsched分别用来设
置和得到线程的继承性!

#include <pthread.h>
int pthread_attr_getinheritsched(const pthread_attr_t *attr,int *inheritsched);
int pthread_attr_setinheritsched(pthread_attr_t *attr,int inheritsched);
参数：
attr                线程属性变量
inheritsched     线程的继承性
若成功返回0，若失败返回-1。

请注意：
继承性决定调度的参数是从创建的进程中继承还是使用在 
schedpolicy和schedparam属性中显式设置的调度信息。           
			   
线程没有默认的继承值设置，所以如果关心线程的调度策略和参数，
只能手动设置！

可设置参数：
PTHREAD_INHERIT_SCHED： 新的线程继承创建线程的策略和参数！
PTHREAD_EXPLICIT_SCHED：新的线程继承策略和参数来自于
							schedpolicy和schedparam属性中显式
							设置的调度信息！
						   
>>>>>:    下面补充线程调度策略和调度参数：
<1>.调度策略：

函数pthread_attr_setschedpolicy和pthread_attr_getschedpolicy分别用
来设置和得到线程的调度策略。

int pthread_attr_getschedpolicy(const pthread_attr_t *, int * policy)
int pthread_attr_setschedpolicy(pthread_attr_*, int policy)
参数：
		attr            线程属性变量
		policy        调度策略   
若成功返回0，若失败返回-1。

所谓调度策略也就是我们之前在OS中所学过的那些调度算法：
SCHED_FIFO    ：先进先出
SCHED_RR       ：轮转法
SCHED_OTHER    ：其他方法

SCHED_OTHER是不支持优先级使用的,而SCHED_FIFO和SCHED_RR
支持优先级的使用,他们分别为1和99,数值越大优先级越高.

注意：
		> 此处的SCHED_FIFO是允许被高优先级抢占的！
		> 也就是有高优先级的必须先运行
		> SCHED_RR是设置一个时间片
		> 当有SCHED_FIFO或SCHED_RR策赂的线程在一个条件变量
		上等持或等持加锁同一个互斥量时，它们将以优先级顺序被唤
		醒。即，如果一个低优先级的SCHED_FIFO线程和一个高优先
		织的SCHED_FIFO线程都在等待锁相同的互斥且，则当互斥量
		被解锁时，高优先级线程将总是被首先解除阻塞。
	   
<2>.调度参数：

函数pthread_attr_getschedparam 和pthread_attr_setschedparam分别
用来设置和得到线程的调度参数。

   

int pthread_attr_getschedparam(const pthread_attr_t *,struct
sched_param *);
int pthread_attr_setschedparam(pthread_attr_t *,const struct
sched_param *);
参数：
		attr            线程变量属性
		param        sched_parm 结构体
若成功返回0，若失败返回-1。

/usr/include /bits/sched.h
struct sched_param
{
	   int sched_priority;    //!> 参数的本质就是优先级
};
注意：大的权值对应高的优先级!
系统支持的最大和最小的优先级值可以用函数：
sched_get_priority_max和sched_get_priority_min得到！

#include <pthread.h>
int sched_get_priority_max( int policy );
int sched_get_priority_min( int policy );
参数：max_：    系统支持的优先级的最小值
		min_ ：    系统支持的优先级的最大值

使用：max_ = sched_get_priority_max( policy );
		min_ = sched_get_priority_min( policy );
		注意参数是policy调用策略，也就是说对于不同的策略的值是不
		一样的！

附录：来自
http://www.yuanma.org/data/2006/0823/article_1392.htm
policy = SCHED_OTHER
max_priority = 0
min_priority = 0

Show SCHED_FIFO of priority
max_priority = 99
min_priority = 1

Show SCHED_RR of priority
max_priority = 99
min_priority = 1

Show priority of current thread
priority = 0

3.    线程的作用域：
                               
函数pthread_attr_setscope和pthread_attr_getscope分别
用来设置和得到线程的作用域。       
#include <pthread.h>   
int    pthread_attr_getscope( const pthread_attr_t * attr, int * scope );
int pthread_attr_setscope( pthread_attr_t*, int scope );
参数：
		attr               线程属性变量
		scope         线程的作用域       
若成功返回0，若失败返回-1。

作用域控制线程是否在进程内或在系统级上竞争资源，可能的值是
PTHREAD_SCOPE_PROCESS（进程内竞争资源）
PTHREAD_SCOPE_SYSTEM   （系统级竞争资源）。
                   
4.    线程堆栈的大小
           
函数pthread_attr_setstackaddr和pthread_attr_getstackaddr分别用来设置和得
到线程堆栈的位置。

int pthread_attr_getstacksize(const pthread_attr_t *,size_t * stacksize);
int pthread_attr_setstacksize(pthread_attr_t *attr ,size_t *stacksize);
参数：attr                线程属性变量
		stacksize        堆栈大小
若成功返回0，若失败返回-1。           
       
5.    线程堆栈的地址           
           
#include <pthread.h>
int pthread_attr_getstackaddr(const pthread_attr_t *attr,void **stackaddf);
int pthread_attr_setstackaddr(pthread_attr_t *attr,void *stackaddr);
参数：attr               线程属性变量
		stackaddr     堆栈地址           
若成功返回0，若失败返回-1。

注意：pthread_attr_getstackaddr已经过期，现在使用的是：pthread_attr_getstack

6.    警戒缓冲区
           
函数pthread_attr_getguardsize和pthread_attr_setguardsize分别用来设置和得
到线程栈末尾的警戒缓冲区大小。

#include <pthread.h>                   
int pthread_attr_getguardsize(const pthread_attr_t *restrict attr,size_t *restrict
guardsize);
int pthread_attr_setguardsize(pthread_attr_t *attr ,size_t *guardsize);
若成功返回0，若失败返回-1。

值得注意：
			线程属性guardsize控制着线程栈末尾之后以避免栈溢出的扩展内存
			大小。这个属性默认设置为PAGESIZE个字节。可以把guardsize线
			程属性设为0，从而不允许属性的这种特征行为发生：在这种情况
			下不会提供警戒缓存区。同样地，如果对线程属性stackaddr作了
			修改，系统就会认为我们会自己管理栈，并使警戒栈缓冲区机制无
			效，等同于把guardsize线程属性设为0。
一.函数：
1.线程属性的初始化与销毁：
#include <pthread.h>
int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destroy(pthread_attr_t   *attr);
Both return: 0 if OK, error number on failure
2.设置线程属性--detackstate(分离状态):
#include <pthread.h>
int pthread_attr_getdetachstate(const pthread_attr_t *restrict attr, int *detachstate);
int pthread_attr_setdetachstate(pthread_attr_t *attr,  int detachstate);
Both return: 0 if OK, error number on failure
detachstate有两个选项：PTHREAD_CREATE_DETACHED 分离状态启动线程
                       PTHREAD_CREATE_JOINABLE 正常状态启动线程
3.设置线程属性--stackaddr(线程栈的最低地址),stacksize(线程栈的大小)：
#include <pthread.h>
int pthread_attr_getstack(const pthread_attr_t *restrict attr,
                                                  void **restrict stackaddr,
                                                  size_t *restrict stacksize);
int pthread_attr_setstack(const pthread_attr_t *attr,
                                                  void *stackaddr,  
                                                  size_t *stacksize);
Both return: 0 if OK, error number on failure
4.设置线程属性--stacksize(线程栈的大小)：
#include <pthread.h>
int pthread_attr_getstacksize(const pthread_attr_t *restrict attr,
                                                          size_t *restrict stacksize);
int pthread_attr_setstacksize(pthread_attr_t *attr,
                                                          size_t stacksize);
Both return: 0 if OK, error number on failure
5.设置线程属性--guardsize(线程栈末尾的警戒缓冲区大小)
#include <pthread.h>
int pthread_attr_getguardsize(const pthread_attr_t *restrict attr,
                                                          size_t *restrict guardsize);
int pthread_attr_setguardsize(pthread_attr_t *attr, 
                                                          size_t guardsize); 
Both return: 0 if OK, error number on failure
二.重点：
三.例子：
以分离状态创建线程
[cpp] view plaincopy
#include <stdio.h>   
#include <pthread.h>   
#include <string.h>   

void * thr_fn()   
{   
	printf("thread run\n");   
	pthread_exit((void *)0);   
	}   

	int main()   
	{   
	pthread_t tid;   
	pthread_attr_t attr;   
	int ret;   

	ret = pthread_attr_init(&attr);   
	if(ret!=0)   
		printf("init attr error:%s\n",strerror(ret));   
	ret = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);   
	if(ret==0)   
	{   
		ret = pthread_create(&tid,&attr,thr_fn,NULL);   
		if(ret!=0)   
		printf("create thread error:%s\n",strerror(ret));   
	}   
	pthread_attr_destroy(&attr);   
	sleep(1);   
	return 0;  
}

/* ===================================================== */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
/*演示竞态*/

#define MAX 12
int arr[MAX];

int initarr(int value)
{
	for(int i = 0; i < MAX; i++){
		arr[i] = value;
		sleep(1);
	}

	for(int i = 0; i < MAX; i++){
		printf("1:value=%d, arr[%d]=%d\n", value, i, arr[i]);
	}
	
	return 0;
}

void *thr_proc(void *arg)
{
	int value = 100;
	initarr(value);

	return (void *)0;
}

int main()
{
	pthread_t tid;
	pthread_create(&tid, NULL, thr_proc, NULL);

	int value = 1;
	for(int i = 0; i < MAX; i++){
		arr[i] = value;
		sleep(1);
	}

	for(int i = 0; i < MAX; i++){
		printf("2:value=%d, arr[%d]=%d\n", value, i, arr[i]);
	}

	pthread_join(tid, NULL);
}

/* ===================================================== */
{
#include <pthread.h>
#include <stdio.h>

void * thr_proc(void *arg)
{
	int i = 10;

	while(i--){
		printf("-NO.%d-------2---------\n", i);
		sleep(1);
	}

	pthread_exit((void *)123);

	return (void *)123;
}

void * thr_proc1(void *arg)
{
	while(1);

	return (void *)123;
}

int main()
{
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	pthread_t tid = 0;
	if(pthread_create(&tid, &attr, thr_proc, "hello world")){
		printf("pthread_create fail.\n");
		return -1;
	}

	pthread_t tid1 = 0;
	if(pthread_create(&tid1, NULL, thr_proc1, NULL)){
		printf("pthread_create fail.\n");
		return -1;
	}

	void* ptr = NULL;
	pthread_join(tid, &ptr);
	printf("ret : %d\n", (int)(ptr));

	pthread_attr_destroy(&attr);

#if 0
	int i = 10;

	while(i--){
		printf("-NO.%d------1---------\n", i);
		sleep(1);
	}
#endif
}
}

/* ===================================================== */

{
#include <pthread.h>
#include <stdio.h>

void * thr_proc(void *arg)
{
	int i = 10;

	while(i--){
		printf("-NO.%d-------2---------\n", i);
		sleep(1);
	}

	pthread_exit((void *)123);

	return (void *)123;
}

void * thr_proc1(void *arg)
{
	while(1);

	return (void *)123;
}

int main()
{
	pthread_t tid = 0;
	if(pthread_create(&tid, NULL, thr_proc, "hello world")){
		printf("pthread_create fail.\n");
		return -1;
	}

	pthread_t tid1 = 0;
	if(pthread_create(&tid1, NULL, thr_proc1, NULL)){
		printf("pthread_create fail.\n");
		return -1;
	}

	sleep(1);

	void* ptr = NULL;
	pthread_join(tid1, &ptr);
	printf("ret : %d\n", (int)(ptr));

#if 0
	int i = 10;

	while(i--){
		printf("-NO.%d------1---------\n", i);
		sleep(1);
	}
#endif
}
}

/* ====================================================== */

#if 0
#include <pthread.h>

int pthread_mutex_init (pthread_mutex_t *__mutex, __const pthread_mutexattr_t *__mutexattr);
int pthread_mutex_destroy (pthread_mutex_t *__mutex);

int pthread_mutex_trylock (pthread_mutex_t *__mutex);
int pthread_mutex_lock (pthread_mutex_t *__mutex);
int pthread_mutex_unlock (pthread_mutex_t *__mutex);

#endif
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 6
int arr[MAX];

pthread_mutex_t mutex;


void init_arr(int value)
{
	for(int i = 0; i < MAX; i++){
		arr[i] = value;
		sleep(1);
	}

	for(int i = 0; i < MAX; i++){
		printf("value=%d, arr[%d]=%d\n", value, i, arr[i]);
	}
}

void *thr_proc(void *arg)
{
	sleep(1);
#if 0
	pthread_mutex_lock(&mutex);
#else
	int ret = pthread_mutex_trylock(&mutex);
	if(ret){
		printf("lock fail: %d\n", ret);
		return (void *)0;
	}
#endif
	init_arr(100);
	pthread_mutex_unlock(&mutex);

	return (void *)0;
}

int main()
{
	pthread_mutex_init(&mutex, NULL);

	pthread_t tid;
	pthread_create(&tid, NULL, thr_proc, NULL);

	pthread_mutex_lock(&mutex);
	init_arr(1);
	pthread_mutex_unlock(&mutex);

	pthread_join(tid, NULL);
	return 0;
}

/* ================================================ */

#if 0
#include <pthread.h>
int pthread_cond_init (pthread_cond_t *__restrict __cond,
			      __const pthread_condattr_t *__restrict
			      __cond_attr)

int pthread_cond_destroy (pthread_cond_t *__cond);


int pthread_cond_wait (pthread_cond_t *__restrict __cond,
			      pthread_mutex_t *__restrict __mutex);
int pthread_cond_signal (pthread_cond_t *__cond);

#endif
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


pthread_cond_t cond;
pthread_mutex_t mutex;

int flag = 1;

void *thr_proc(void *arg)
{
	printf("-------3---------\n");
	
	pthread_mutex_lock(&mutex);
	if(flag){
		pthread_cond_wait(&cond, &mutex);
#if 0
		{
			unlcok();
			...
			lock();
		};
#endif
	}
	pthread_mutex_unlock(&mutex);

	printf("-------4---------\n");

	return (void *)0;
}

int main()
{
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init (&cond, NULL);

	pthread_t tid;
	pthread_create(&tid, NULL, thr_proc, NULL);

	printf("-------1---------\n");
	//sleep(3);

	pthread_mutex_lock(&mutex);
	flag = 0;
	pthread_mutex_unlock(&mutex);
	pthread_cond_signal(&cond);

	pthread_join(tid, NULL);
}

/* ================================================= */

#if 0
#include <semaphore.h>

int sem_init (sem_t *__sem, int __pshared, unsigned int __value);
int sem_destroy (sem_t *__sem);

P OP
int sem_trywait (sem_t *__sem);
int sem_wait (sem_t *__sem);
v OP
int sem_post (sem_t *__sem);

#endif

#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 12
int arr[MAX];

sem_t sem;

void init_arr(int value)
{
	for(int i = 0; i < MAX; i++){
		arr[i] = value;
		sleep(1);
	}

	for(int i = 0; i < MAX; i++){
		printf("value=%d, arr[%d]=%d\n", value, i, arr[i]);
	}
}

void * thr_proc(void *arg)
{
	int value = 100;
	init_arr(value);

	sem_post (&sem);

	return (void *)0;
}

int main()
{
	sem_init (&sem, 0, 0);

	pthread_t tid;
	pthread_create(&tid, NULL, thr_proc, NULL);

	printf("--------1--------\n");
#ifdef BLOCK_SEM
	sem_wait (&sem);
#else
	if(sem_trywait (&sem)){
		printf("try lock fail.\n");
		return -1;
	};
#endif
	printf("--------2--------\n");

	int value = 1;
	init_arr(value);

	pthread_join(tid, NULL);
}

/* =================================================== */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/*函数重入引发竞态：*/
#define MAX 12
int arr[MAX];

/*不可重入函数，线程不安全*/
int initarr(int value)
{
	for(int i = 0; i < MAX; i++){
		arr[i] = value;
		sleep(1);
	}

	for(int i = 0; i < MAX; i++){
		printf("1:value=%d, arr[%d]=%d\n", value, i, arr[i]);
	}
	
	return 0;
}

void *thr_proc(void *arg)
{
	int value = 100;
	initarr(value);

	return (void *)0;
}

int main()
{
	pthread_t tid;
	pthread_create(&tid, NULL, thr_proc, NULL);

	int value = 1;
	initarr(value);

	pthread_join(tid, NULL);
}

/* =============================================== */

#include <stdio.h>

//模拟注册回调机制

#define MAX 12
typedef void *(*THRPROC)(void *arg);

//抽象管理代码对象
typedef struct __thr{
	THRPROC proc;
	int id;
	void *arg;
}THR;

//代码管理数据结构
THR thrarr[MAX];

//系统初始化代码对象
void sys_init()
{
	for(int i = 0; i < MAX; i++){
		thrarr[i].proc = NULL;
		thrarr[i].id = -1;
		thrarr[i].arg = NULL;
	}
}

//注册代码函数
int register_thr(int *id, THRPROC pfun, void *arg)
{
	for(int i = 0; i < MAX; i++){
		if(NULL == thrarr[i].proc){
			*id = i;
			thrarr[i].id = i;
			thrarr[i].proc = pfun;
			thrarr[i].arg = arg;
			return 0;
		}
	}

	return -1;
}

//调度代码函数
void *schedule()
{
	//
	for(int i = 0; i < MAX; i++){
		if(NULL != thrarr[i].proc){
			thrarr[i].proc(thrarr[i].arg);
		}
	}

	return (void *)0;
}

//////////////////////////////////////////////////////
//代码实例
void* demo1(void *arg)
{
	printf("%s:%s\n", __func__, (char *)arg);
	return (void *)0;
}

void* demo2(void *arg)
{
	printf("%s:%d\n", __func__, (int)arg);
	return (void *)0;
}

int main()
{
	//演示初始化
	sys_init();

	////////////////
	//演示注册
	int id;
	register_thr(&id, demo1, "hello world");
	register_thr(&id, demo2, (void *)123);
	////////////////
	
	//演示调度回调
	schedule();
	return 0;
}

/* ============================================== */

unsigned int k = 0;
static void *routine(void *arg)
{
	PMYTHR pthr=(PMYTHR)arg;

	pthread_mutex_init(&pthr->mutex, NULL);
	pthread_cond_init (&pthr->cont, NULL); 

	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	do{
		pthread_mutex_lock(&pthr->mutex);

		if(pthr->bthrsusend)
		{
			if(0 != pthread_cond_wait (&pthr->cont, &pthr->mutex))
			{
				perror("work:pthread_cond_wait");
				exit(-1);
			}
		}

		pthread_mutex_unlock(&pthr->mutex);
		//执行线程
		pthr->run(pthr->arg);
	}while(pthr->bthrun);

	pthread_cond_destroy(&pthr->cont);
	pthread_mutex_destroy(&pthr->mutex);

	return (void *)0;
}

int thr_start(PMYTHR pthr)
{
	if(NULL != pthr->run){
		return pthread_create(&pthr->tid, NULL, routine, (void *)pthr);
	}

	return -1;
}

int thr_wait(PMYTHR pthr)
{
	if(NULL != pthr->run){
		return pthread_join(pthr->tid, NULL);
	}

	return -1;
}

int thr_cancel(PMYTHR pthr)
{
	if(NULL != pthr->run){
		pthread_cancel(pthr->tid);

		pthread_cond_destroy(&pthr->cont);
		pthread_mutex_destroy(&pthr->mutex);

		return 0;
	}

	return -1;
}

void thr_exit(PMYTHR pthr)
{
	if(NULL != pthr->run){
		pthread_mutex_lock(&pthr->mutex);
		pthr->bthrun = false;
		pthread_mutex_unlock(&pthr->mutex);
	}
}

void thr_suspend(PMYTHR pthr)
{
	pthread_mutex_lock(&pthr->mutex);

	if(!pthr->bthrsusend)
	{
		pthr->bthrsusend = true;
	}

	pthread_mutex_unlock(&pthr->mutex);
}

void thr_resume(PMYTHR pthr)
{
	pthread_mutex_lock(&pthr->mutex); 
	if(pthr->bthrsusend)
	{
		pthr->bthrsusend = false;
		pthread_cond_signal(&pthr->cont);
	} 
	pthread_mutex_unlock (&pthr->mutex);
}
void run1(void *arg)
{
	char *str = (char *)arg;
	printf("1: Thr---test---%s\r\n", str);
	sleep(1);
}
MYTHR thr1={run1, "aaaaaa", true, false};

void run2(void *arg)
{
	char *str = (char *)arg;
	printf("2: Thr---test---%s\r\n", str);
	sleep(1);
}
MYTHR thr2={
	.run = run2,
	.arg = "hello world",
	.bthrun = true,
	.bthrsusend = true,
};
	
int main(int argc, char **argv)
{
	thr_start(&thr1);
	thr_start(&thr2);

	while(1){
		char ch = getchar();
		switch(ch){
		case 'r':
			thr_resume(&thr1);
			break;
		case 'R':
			thr_resume(&thr2);
			break;
		case 's':
			thr_suspend(&thr1);
			break;
		case 'S':
			thr_suspend(&thr2);
			break;
		case 'q':
		case 'Q':
			exit(0);
		};
	}

	thr_wait(&thr1);	
	thr_wait(&thr2);	

	return 0;
}