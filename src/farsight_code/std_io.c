{
文件操作
1、打开
/*
功能：打开或创建打开文件
参数：
const char *path - 带路径的文件名（不写路径的话，目前当前路径）
const char *mode - 操作方法
	r  - 读打开
	r+ - Open for reading and writing.
	...
返回值：
	成功-得到一个文件指针
*/
FILE *fopen(const char *path, const char *mode);
2、关闭
A、主动关闭文件
int fclose(FILE *stream);
B、当进程结束时
如果没有主动关闭文件
系统会自动关闭

3、读文件
/*
 *读文件
 void *ptr - 缓存首地址
 size_t size - 单位
 size_t nmemb- 数量
 FILE *stream - 文件指针（打开的文件）
 返回值：
	成功-实际读取的数量
 */
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);

4、写文件
size_t fwrite(const void *ptr, size_t size, size_t nmemb,
                     FILE *stream);

5、定位文件
/*
 文件定位
 FILE *stream - 文件指针
long offset - 偏移位置
int whence - 开始位置
	SEEK_SET - 文件起始位置
	SEEK_CUR - 文件指针当前位置
	SEEK_END - 文件尾

	失败 - -1 
 */
int fseek(FILE *stream, long offset, int whence);

-----------------------
如何描述文件
1、字符串
文件名
路径

2、系统管理描述文件
A、文件描述符
特殊的整型-用于表征打开的文件对象
B、文件指针
typedef struct FILE{
	int fd;	//文件描述符-描述打开的文件对象
	void *buf;//缓存的首地址
	int len;//缓存的长度
	...
}FILE;

	补充：
	缓存的作用：解决速度不匹配
	
	刷新规则：
		A、全缓存：当库内建缓存满了，系统自动回写
		B、行缓存：当库内建缓存满了或遇到换行，系统自动回写
		C、无缓存：一般的，设备控制不能缓存，需要及时响应
		D、强制刷新：

=======================================================
标准报错
#include <stdio.h>
/*
 *标准报错（在标准输出显示错误信息）
 参数：
	const char *s - 报错时附加的输出信息
 返回值：
	
 */
void perror(const char *s)

#include <errno.h>
extern int errno;	//保存错误码的全局变量

#include <string.h>
/*
 *标准报错
 参数：
	int errnum 错误码
 返回值：
	指向错误信息的字符串首地址
 */
char *strerror(int errnum);
=======================================================
标准输入输出
文件指针
FILE* stdin; 输入
FILE* stdout;输出

/*
格式化字符串
参数：
	char *str - 指向格式化后的字符串存储首地址
	const char *format - 指定格式（%d、%s）
	...	- 不定参数

返回值：
	失败 - 负数
 */
int sprintf(char *str, const char *format, ...);
int snprintf(char *str, size_t size, const char *format, ...);
}

/* =================================================== */

{
1、系统调用
操作系统提供给用户层的功能，C编程方式，看到的就是一个个函数（API）

2、C库
A、与系统无关的标准接口函数
int add(int x, int y)
{
return x+y;
}
B、与系统有关的标准接口函数
fopen
3、一切既是文件
在linux系统把所有操作对象都当成“文件”
7类“文件”对象：普通文件、目录、符号链接（软/硬链接）、管道、字符设备、块设备、套接字
标准化：只需要一套函数就可以操作所以对象了，实际情况有不标准的接口
}

/* =================================================== */

{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <errno.h>
//extern int errno;

int main()
{
	char filename[] = "txt";
	FILE *fp = fopen(filename, "r");
	if(NULL == fp){
		perror("1111fopen");
		return -1;
	}

#define MAX 1024
#define DW 4
	char buf[MAX];
	memset(buf, 0, MAX);
	int num = fread(buf, DW, 1, fp);	
	printf("-------read:[%d]:%s", num, buf);

	perror("fseek");

	if(0 > fseek(fp, 0, SEEK_SET)){
		perror("fseek");
		printf("err code :%d\n", errno);
		printf("%s\n", strerror(errno));

		goto ERR_STEP;
	}

	memset(buf, 0, MAX);
	num = fread(buf, DW, 1, fp);
	printf("-------read:[%d]:%s", num, buf);
#if 0
	if(0 > fseek(fp, 0, SEEK_SET)){
		//printf("err code :%d\n", errno);
		goto ERR_STEP;
	}
#endif
	num = fwrite(buf, DW, num, fp);
	printf("-------WR:[%d]\n", num);

ERR_STEP:
	fclose(fp);
	return 0;
}
}

/* =================================================== */

{
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <string.h>
#include <errno.h>

int main()
{
	int fd = open("demo.txt", O_CREAT|O_EXCL|O_RDWR, 0666);
	if(0 > fd){
		if(EEXIST != errno){
			perror("open");
			return -1;
		}

		printf("----------------\n");
		fd = open("demo.txt", O_RDWR|O_APPEND);
		if(0 > fd){
			perror("open");
			return -1;
		}
		printf("1: %d\n", fd);
	}

	//lseek(fd, 0, SEEK_END);

	char wbuf[] = "12345 world";
	int len = sizeof(wbuf)-1;
	int num = write(fd, wbuf, len);
	if(num != len){
		printf("2: %d\n", fd);
		perror("write");
		close(fd);
		return -1;
	}

	lseek(fd, 0, SEEK_SET);

	#define MAX 1024
	char rbuf[MAX];
	memset(rbuf, 0, MAX);
	num = read(fd, rbuf, MAX);
	if(0 > num){
		perror("read");
		close(fd);
		return -1;
	}

	printf("RD[%d]:%s\n", num, rbuf);

	close(fd);
	return 0;	
}
}

/* =================================================== */

{
#include <stdio.h>
#include <string.h>
#define MAX 10
/*linux的一切皆文件，可以让我们很轻松的对IO设备进行操作，
  文件IO应该不可以因为不能操作文件指针（FILE *）。*/
/*标准IO相比文件IO主要是多了缓存，执行的速度要快些，不过用fread()和fwrite()要注意*/
int main()
{
	char buf[MAX];
	memset(buf, 0, MAX);
	
	/*追加读写*/
	FILE *fp;						/*所以返回值最好用man查*/
	fp = fopen("./hello.txt", "a+");
									/*关于字符串的函数*/
	fputs("yanis", fp);				//fputs(char *, FILE *)
	fseek(fp, 0, SEEK_SET);			//fseek(FILE *, offset, whence)
	fgets(buf, MAX, fp);			//fgets(char *(就是缓冲区地址), 最大长度, FILE *)
	printf("RD:"); 
	printf("%s\n", buf);
	puts("input a string:");		//puts(char *) 结束后自动添加换行符
	gets(buf);						//gets(char *) 与fgets类似，只是默认FILE *为标准输入，因为没有大小限制，所以最好不用
	char str[10] = "RD:";
	puts(strcat(str, buf));			/*字符串的类似函数有无f的区别主要在于是否默认了流文件，接收处有无基本一致
									  而字符的类似函数有无f基本没有区别，除putchar和getchar外都需要指定流文件*/
	printf("========================================\n");
	int i = 10;						/*关于字符的函数*/
	while (i--)	
		fputc(i + 66, fp);			//fputc(int, FILE *) 这里是的参数是int不是char
	putc('\n',fp);					//putc(int, FILE *) 等价于fputc()
	fseek(fp, -4, 1);				//fseek()的第三个参数既可以是0，1，2也可以是SEEK_SET，SEEK_CUR，SEEK_END
	char ch = '\0';
	printf("RD:");
	i = 2;	//这里和上面的fseek不能太大，否则第一会乱码
	while (i--) {
		ch = fgetc(fp);				//fgetc(FILE *)
		putchar(ch);
	}
	ch = getc(fp);					//getc(FILE *) 等价于fgetc()
	putc(ch - 1, stdout);
	printf("\n========================================\n");
	printf("input a character:\n");
	ch = getchar();					//getchar()
	putc(ch, stdout);				
	putchar(10);					//putchar()
	
	fclose(fp);
	printf("========================================\n");
	
	/*重新以读写打开文件*/
	/*fread()貌似要等缓存满了才执行下面的操作，这点很不方便，
	所以用标准IO要注意fread()，而且fwrite()貌似也不好用，
	所以要注意这俩函数*/
	fp = fopen("./hello.txt", "w+");	//区别于上面的追加写入，这儿是覆盖写入
	bzero(buf, MAX);					//和memset()的功能差不多
	fwrite("yanisme",1 , 7, fp);		//第一个数字代表写入块的单位大小，第二个数字代表写入块的数量
	fseek(fp, 0, 0);
	fread(buf, 1, 7, fp);
	printf("buf:%s\n", buf);
	printf("========================================\n");
	fwrite("yanisme",1 , 7, stdout);   //fwrite()和fread()也可以直接操作设备文件
	puts("\ninput a string:");
	fread(buf, 1, 7, stdin);
	printf("buf:%s\n", buf);
	
	fclose(fp);
	return 0;	
}

	
}


