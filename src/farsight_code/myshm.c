#include "myshm.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <signal.h>
#include <error.h>

/*	semid = semget(key_t key, int nsems, int semflg)
 *	semctl(int semid, int semnum, int cmd, union semun arg)
 *  int semop(int semid, struct sembuf *sops, size_t nsops)
 */

int init_sem(int sem_id, int init_value)
{
	union semun sem_union;
	sem_union.val = init_value;
	if (-1 == semctl(sem_id, 0, SETVAL, sem_union)) {
		perror("Initialize semaphore");
		return -1;		
	}
	
	return 0;	
}

int del_sem(int sem_id)
{
	union semun sem_union;
	if (-1 == semctl(sem_id, 0, IPC_RMID, sem_union)) {
		perror("Delete semaphore");
		return -1;
	}
	
	return 0;
}

int sem_p(int sem_id) 
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;//one semaphore
	sem_b.sem_op = -1;//P operation
	sem_b.sem_flg = SEM_UNDO;//auto release semaphore
	if (-1 == semop(sem_id, &sem_b, 1)) {
		perror("P operation");
		return -1;
	}
	
	return 0;
}

int sem_v(int sem_id) 
{
	struct sembuf sem_b;
	sem_b.sem_num = 0;
	sem_b.sem_op = 1;//V operation
	sem_b.sem_flg = SEM_UNDO;
	if (-1 == semop(sem_id, &sem_b, 1)) {
		perror("V operation");
		return -1;
	}
	
	return 0;
}

int ignore_signal(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGSTOP, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	
	return 0;
}

struct shm_id shm_create(void)
{
	int shmid, semid;
	struct shm_id s_shm_id;
	
	/* semaphore init */
	//ignore_signal();
	
	semid = semget(ftok(".", 'a'), 1, 0666|IPC_CREAT);
	init_sem(semid, 1);
	
	/* shared memory init */
	shmid = shmget(99999, sizeof(struct shm_buff), 0666|IPC_CREAT);
	if (-1 == shmid) {
		perror("shmget failed");
		del_sem(semid);
		exit(-1);
	}
	
	s_shm_id.shmid = shmid;
	s_shm_id.semid = semid;
	return s_shm_id;
}
