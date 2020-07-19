#include <unistd.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>


int lock_set(int fd, int type)
{
	struct flock old_lock,lock;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
  	lock.l_len = 0;
	lock.l_type = type;
	lock.l_pid;

	fcntl(fd, F_GETLK, &lock);
	if (F_UNLCK != lock.l_type) {
		if (F_RDLCK == lock.l_type) {
			printf("Read lock already set by %d\n", lock.l_pid);
		} else if (F_WRLCK == lock.l_type) {
			printf("Write lock already set by %d\n", lock.l_pid);
		}
	}

	lock.l_type = type;
	if (0 > fcntl(fd, F_SETLKW, &lock)) {
		printf("Lock failed:type = %d\n", lock.l_type);
		return -1;
	}

	switch (lock.l_type)
	{
	case F_RDLCK:
		printf("Read lock set by %d\n", getpid());
		break;
	case F_WRLCK:
		printf("Write lock set by %d\n", getpid());
		break;
	case F_UNLCK:
		printf("Release lock by %d\n", getpid());
		break;
	}

	return 0;
}

int main(void)
{
	int fd;
	
	if (0 > (fd = open("hello", O_RDWR|O_CREAT))) {
		perror("fail to open");
		return -1;
	}

	lock_set(fd, F_WRLCK);
	getchar();
	lock_set(fd, F_UNLCK);
	getchar();

	close(fd);

	return 0;
}
