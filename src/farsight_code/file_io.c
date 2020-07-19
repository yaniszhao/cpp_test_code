{
1、打开
（存储数据的普通文件存在创建）

A、文件i/o
int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);

B、标准I/O
#include <stdio.h>
FILE *fopen(const char *path, const char *mode);
FILE *fdopen(int fd, const char *mode);
FILE *freopen(const char *path, const char *mode, FILE *stream);

2、关闭文件
A、文件I/O
int close(int fd);

B、标准I/O
int fclose(FILE*);


3、创建数据文件
int creat(const char *pathname, mode_t mode);
4、删除数据文件
#include <unistd.h>
int unlink(const char *pathname);

5、读写方法
A、文件I/O
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);

B、标准I/O
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);


文件I/O：一般用于设备读写，一般的，设备读写要求实时性好
标准I/O：一般用于数据文件读写

6、刷新数据
强制回写数据到外部存储器（硬盘、U盘）
int fflush(FILE *stream);

7、定位文件
A、文件I/O
off_t lseek(int fd, off_t offset, int whence);

B、标准I/O
int fseek(FILE *stream, long offset, int whence);
long ftell(FILE *stream);
void rewind(FILE *stream);
int fgetpos(FILE *stream, fpos_t *pos);
int fsetpos(FILE *stream, fpos_t *pos);
========================================================================
#include <stdio.h>
int fgetc(FILE *stream);
char *fgets(char *s, int size, FILE *stream);
int getc(FILE *stream);
int getchar(void);

int fputc(int c, FILE *stream);
int fputs(const char *s, FILE *stream);
int putc(int c, FILE *stream);
int putchar(int c);
int puts(const char *s);
========================================================================

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
/*
 * 功能：查看文件属性信息
 * 参数：
	输入参数：const char *path - 带路径的文件名
	输出参数：struct stat *buf - 指向得到文件属性信息结构的首地址
 */
int stat(const char *path, struct stat *buf);

struct stat {
       dev_t     st_dev;     /* ID of device containing file */
       ino_t     st_ino;     /* inode number */
       mode_t    st_mode;    /* protection */
       nlink_t   st_nlink;   /* number of hard links */
       uid_t     st_uid;     /* user ID of owner */
       gid_t     st_gid;     /* group ID of owner */
       dev_t     st_rdev;    /* device ID (if special file) */
       off_t     st_size;    /* total size, in bytes */
       blksize_t st_blksize; /* blocksize for file system I/O */
       blkcnt_t  st_blocks;  /* number of 512B blocks allocated */
       time_t    st_atime;   /* time of last access */
       time_t    st_mtime;   /* time of last modification */
       time_t    st_ctime;   /* time of last status change */
};

时间值转换
char *ctime(const time_t *timep);
===========================================
打开目录
#include <sys/types.h>
#include <dirent.h>
/*
 *打开目录
 * const char *name - 带路径的目录
 * 返回值：
	成功- 目录指针DIR
 */
DIR *opendir(const char *name);
DIR *fdopendir(int fd);

读目录
#include <dirent.h>
/*
 * 读目录
  参数：
	DIR *dirp - 目录指针（表征已经打开的目录）
 返回值：
	成功 -得到目录下的”文件“信息
 */
struct dirent *readdir(DIR *dirp);
struct dirent {
               ino_t          d_ino;       /* inode number */
               off_t          d_off;       /* offset to the next dirent */
               unsigned short d_reclen;    /* length of this record */
               unsigned char  d_type;      /* type of file; not supported
                                              by all file system types */
               char           d_name[256]; /* filename */
};

关闭目录
int closedir(DIR *dirp);
=====================================================
库：就是很多供程序使用的函数的可执行代码

主要为了复用代码
把功能相同的代码预先编译好，不需要跟使用代码一起编译

静态库：在和调用者一起链接时，把代码直接加进调用者代码中
动态库：运行时加载的库
A、编译器在链接时，在调用者代码中添加特定代码
(1)、怎么加载库
（2）、怎么找到函数
B、编译时不依赖共享库，自己编写代码
(1)、怎么加载库
（2）、怎么找到函数
====================================================
共享库操作方法

出处：libdl.so
头文件：dlfcn.h
/*
 * 功能：打开共享库
 * 参数：
	const char *filename - 指向带路径的共享库文件名的字符串首地址
	int flag - 打开标志
 * 返回值：
	成功 - 句柄（表征打开的共享库）
	失败 - NULL
 */
void *dlopen(const char *filename, int flag);

/*
 * 功能：输出操作共享库的错误信息
 * 参数：
 * 返回值：
	指向错误信息字符串的首地址
 */
char *dlerror(void);

/*
 * 功能：通过符号（字符串）查找地址
 * 参数：
	void *handle - 句柄（表征打开的共享库）
	const char *symbol - 指向要查找的符号（字符串）的首地址
 * 返回值：
	成功 - 查找到的地址
	失败 - NULL
 */
void *dlsym(void *handle, const char *symbol);

/*
 * 功能：关闭共享库
 * 参数：
	void *handle - 句柄（表征打开的共享库）
 * 返回值：
	成功 - 0
	失败 - 非0
 */
int dlclose(void *handle);
}

/* =================================================== */

{
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int prninfo(const char *fname)
{
	struct stat info;
	if(0 > stat(fname, &info)){
		perror("stat");
		return -1;
	}
/*
	S_ISREG(m)  is it a regular file?
           S_ISDIR(m)  directory?
           S_ISCHR(m)  character device?
           S_ISBLK(m)  block device?
           S_ISFIFO(m) FIFO (named pipe)?
           S_ISLNK(m)  symbolic link? (Not in POSIX.1-1996.)
           S_ISSOCK(m) */

	if(S_ISREG(info.st_mode)){
		printf("R");
	}else if(S_ISDIR(info.st_mode)){
		printf("D");
	}else if(S_ISCHR(info.st_mode)){
		printf("C");
	}else if(S_ISBLK(info.st_mode)){
		printf("B");
	}else if(S_ISFIFO(info.st_mode)){
		printf("F");
	}else if(S_ISLNK(info.st_mode)){
		printf("L");
	}else if(S_ISSOCK(info.st_mode)){
		printf("S");
	}else{
		printf("-");
	}

	mode_t owner = info.st_mode & S_IRWXU;
	if(owner & S_IRUSR){
		printf("r");
	}else{
		printf("-");
	}
	if(owner & S_IWUSR){
		printf("w");
	}else{
		printf("-");
	}
	if(owner & S_IXUSR){
		printf("x");
	}else{
		printf("-");
	}

	mode_t group = info.st_mode & S_IRWXG;
	if(group & S_IRGRP){
		printf("r");
	}else{
		printf("-");
	}
	if(group & S_IWGRP){
		printf("w");
	}else{
		printf("-");
	}
	if(group & S_IXGRP){
		printf("x");
	}else{
		printf("-");
	}

	mode_t other = info.st_mode & S_IRWXO;
	if(other & S_IROTH){
		printf("r");
	}else{
		printf("-");
	}
	if(other & S_IWOTH){
		printf("w");
	}else{
		printf("-");
	}
	if(other & S_IXOTH){
		printf("x");
	}else{
		printf("-");
	}

	printf(" %d", info.st_size);

	char *ptm = (char *)ctime(&info.st_atime);
	int id = strlen(ptm)-1;
	ptm[id] = 0;
	printf(" %s", ptm);

	ptm = (char *)ctime(&info.st_mtime);
	id = strlen(ptm)-1;
	ptm[id] = 0;
	printf(" %s", ptm);
	
	ptm = (char *)ctime(&info.st_ctime);
	id = strlen(ptm)-1;
	ptm[id] = 0;
	printf(" %s", ptm);

	printf(" %s\n", fname);

	return 0;
}

int main(int num, char **arg)
{
	if(2 != num){
		printf("Usage: %s filename.\n", arg[0]);
		return -1;
	}

	prninfo(arg[1]);
}
}

/* ================================================ */

{
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int prninfo(const char *fname)
{
	struct stat info;
	if(0 > stat(fname, &info)){
		perror("stat");
		return -1;
	}

	if(S_ISREG(info.st_mode)){
		printf("R");
	}else if(S_ISDIR(info.st_mode)){
		printf("D");
	}else if(S_ISCHR(info.st_mode)){
		printf("C");
	}else if(S_ISBLK(info.st_mode)){
		printf("B");
	}else if(S_ISFIFO(info.st_mode)){
		printf("F");
	}else if(S_ISLNK(info.st_mode)){
		printf("L");
	}else if(S_ISSOCK(info.st_mode)){
		printf("S");
	}else{
		printf("-");
	}

	mode_t owner = info.st_mode & S_IRWXU;
	if(owner & S_IRUSR){
		printf("r");
	}else{
		printf("-");
	}
	if(owner & S_IWUSR){
		printf("w");
	}else{
		printf("-");
	}
	if(owner & S_IXUSR){
		printf("x");
	}else{
		printf("-");
	}

	mode_t group = info.st_mode & S_IRWXG;
	if(group & S_IRGRP){
		printf("r");
	}else{
		printf("-");
	}
	if(group & S_IWGRP){
		printf("w");
	}else{
		printf("-");
	}
	if(group & S_IXGRP){
		printf("x");
	}else{
		printf("-");
	}

	mode_t other = info.st_mode & S_IRWXO;
	if(other & S_IROTH){
		printf("r");
	}else{
		printf("-");
	}
	if(other & S_IWOTH){
		printf("w");
	}else{
		printf("-");
	}
	if(other & S_IXOTH){
		printf("x");
	}else{
		printf("-");
	}

	printf(" %d", info.st_size);

	char *ptm = (char *)ctime(&info.st_atime);
	int id = strlen(ptm)-1;
	ptm[id] = 0;
	printf(" %s", ptm);
#if 0
	ptm = (char *)ctime(&info.st_mtime);
	id = strlen(ptm)-1;
	ptm[id] = 0;
	printf(" %s", ptm);
	
	ptm = (char *)ctime(&info.st_ctime);
	id = strlen(ptm)-1;
	ptm[id] = 0;
	printf(" %s", ptm);

#endif
	printf(" %s\n", fname);
	return 0;
}

int rdirinfo(const char *path)
{
	DIR *dir = opendir(path);
	if(NULL == dir){
		perror("opendir");
		return -1;
	}

	while(1){
		struct dirent *info = NULL;
		info = readdir(dir);
		if(NULL == info){
			//perror("readdir");
			break;
		}

		char fname[PATH_MAX];
		snprintf(fname, PATH_MAX, "%s/%s", path, info->d_name);
		//printf("[TRACE]%s\n", fname);
		if(0 > prninfo(fname)){
			break;
		}
	}

	closedir(dir);
	
}

int main()
{
	rdirinfo("/etc");
}
}

/* ===================================================== */

{
#include <stdio.h>
#include <string.h>
#define _TRACE
#define _PRNERRMSG
#include <errmsg.h>
/*
 *功能：从指定目录开始查找指定文件
 *参数：
	const char *dirname - 指向目录名
	const char *filname - 指向文件名
 *返回值：
	-1没找到，0找到
 */
int findfile(const char *pathname, const char *filname)
{
	DIR *dp;
	struct dirent *d;
	dp = opendir(pathname);
	if(NULL == dp){
		PRNERRMSG("opendir");
		return -1;
	}

	char fullpathname[PATH_MAX];
	while(NULL != (d=readdir(dp))){
		snprintf(fullpathname, PATH_MAX, "%s/%s", pathname, d->d_name);
		if((DT_REG == d->d_type) || (DT_LNK == d->d_type)){
			if(!strcmp(filname, d->d_name)){
				printf("%s\n", fullpathname);
				return 0;
			}
		}else if(DT_DIR == d->d_type){
			if(!strcmp(".", d->d_name))
				continue;
			if(!strcmp("..", d->d_name))
				continue;

			if(0 > findfile(fullpathname, filname)){
				if(0 > closedir(dp)){
					TRACE("closedir");
				}

				return -1;
			}
		}
	}

	if(0 > closedir(dp)){
		TRACE("closedir");
		return -1;
	}

	return 0;
}
int main(int argc, char *argv[])
{
	if(3 != argc){
		printf("Usage: %s dir file\n", argv[0]);	
		return -1;
	}

	if(0 > findfile(argv[1], argv[2])){
		return -1;
	}

	return 0;
}
}

/* ===================================================== */