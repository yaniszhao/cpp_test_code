#ifndef _MYSHM_H_
#define _MYSHM_H_

#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>

#define IMG_NUM			1				//number of image
#define SHM_BUFF_SZ 	(1024 * 80)  	//80kb

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
};

/* shared memory size */
struct shm_buff {
	int pid;//shared memory pid
	int written;//real written number
	/*            ____________
	 * buffer -->|SHM_BUFF_SZ|
	 *           |___________| *IMG_NUM
	 */
	char buffer[IMG_NUM][SHM_BUFF_SZ];
};

/* for shared memory id and semaphore id */
struct shm_id {
	int shmid;
	int semid;
};

int init_sem(int sem_id, int init_value);

int del_sem(int sem_id);

int sem_p(int sem_id);

int sem_v(int sem_id) ;

struct shm_id shm_create(void);

#endif
