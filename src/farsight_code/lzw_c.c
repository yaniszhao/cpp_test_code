#include <stdio.h>
int main()
{
	// 数组初始化
	// 1, 完全初始化
	int a[4] = {1, 2, 3, 4};
	int i;
	for(i=0; i<4; i++)
		printf("a[%d] = %d\n", i, a[i]);
	// 2, 部分初始化，没有初始化将会被初始化为0
	float b[6] = {1.0, 2.0};
	for(i=0; i<6; i++)
		printf("a[%d] = %f\n", i, b[i]);
	// 3, 将数组初始化为0
	long c[6] = {0};
	for(i=0; i<6; i++)
		printf("a[%d] = %ld\n", i, c[i]); // %ld, 打印long
	//4, 不指定数组元素个数，由编译器确定
	int d[] = {1, 2, 3, 8, 0, 3, 9};
	// int e[]; //语法错误
	printf("elelment of d: %d\n", sizeof(d)/sizeof(int));
	// 5,不能以一个数组名初始化另外一个数组
	// int b[4] = a;
	int b[4];
	// b = a;	
	// 6,数组越界
	// int b[4] = a[4];

	return 0;
}

/*===================================================*/

1、二进制   '0'  '1'
范围
1）位 ，
2）字节 —— 8位（一般）， 基本单元
3）多字节
2、存储程序思想
	1）指令 Instruction， 代表处理器的特定的操作。
	2）程序，由一些列有序组成的，可以实现特定功能的指令。
3、计算机结构
	CPU（运算器、控制）、内存、输入、输出	
4、CPU结构
	1）指令集
	2）运行机制 ——  取指、执行、重复
		速度快 2 * 1024 * 1024 * 1024		
5、内存
	1） 字节 1000 10.0 JPEG
	2） 程序、数据
	3）掉电丢失
	Cache		
6、进制转换
	到二进制转换—— 除基数（2,16,8）倒取余数	
7、ASCII
	ASCII编码0
	ASCII字符0
	数字0
8、预处理指令 —— 控制编译器

/*=============================================*/

常量
在程序执行过程中，其值不能被改变。
1、类型
	整型、浮点型、字符型、字符串
	1(long) 1UL 1ULL  (365 * 24 * 60 * 60)UL
	10.0(double) 0.5f 0.5
	'a' '\x41' 0x41 '\n' '\r' 'r' '\t'
	'\xffff'
	'\032'
	字符串:以'\0'结尾
	"hello world" + '\0' = 12(not 11)
	"\t\r\n\x41" (5)
// 左值与右值 
2, 宏常量
/** */
#define MAX 1024 // 
3, 常变量: 只读变量
定义时必须初始化
int i = 1;
const int i = 2;

/*==============================================*/

1、算术表达式
1) 表达式的值
	1) /, 整数/没有四舍五入,只能保留整数
	int a = 10;
	int b = 36;
	int c = b / a;
	float f = b / a;
	float f1 = 10;
	float f2 = 36;
	float f3 = f2 / f1;
	float f4 = (float)b / (float)a;
	float f5 = (float)b / a;
	float f6 = b / (float)a;
	// 小数除法四舍五入
	float f7 = (float)2 / 3;// 0.666666, 0.666667
	2) % 浮点数没有模运算
	int a = 2;
	int b = 3;
	int c = a % b;// c = 2
	float f1 = 1.2;
	float f2 = 2.6;
	//float f3 = f1 % f2; // f2 = 
	3) ++,--
	int a = 0;
	a++; // a =1
	a = 2;
	a--; //  a = 1
	a = 0;
	++a; // a = 1
	a = 2;
	--a; // a = 1
	int a = 2;
	int b = 3;
	int c = a / b;
	// 后置++,--，先取值，再++，--
	a = 0;
	b = a++; // 0
	// 前置++,--，先++,-- ，再取值
	a = 0;
	b = ++a; // 1
	// 代码规范性
	// 贪心算法
	++a+++b++;
	++a;
	a++;
	a+b;
	b++;
2)关系运算
	(1) == 
	a = 10;
	a == 0;// a = 0;
	if(a == 0) // if(a=0)
	if(0 == a) // if(0 = a)
	(2)关系运算表达式的值的类型是bool类型
	3)逻辑运算
	1)参与逻辑运算的类型是bool类型，
		int k = 8; // bool 类型1
		float f = 0.5; /// 
		float f1 = 0.000001;
		!f1; //0
	2）逻辑运算表达式的值的类型是bool类型
		!k ; // 0
		int a = 10;
		(a == 0) && a;// 0
		(a == 0) || a; // 1
	3)  10 < a <= 20;// (a >10) && (a <= 20)
		int = 9;
		10 < a <= 20; //  0 or 1
		(10 < a) // 0
			0 <= 20 // 1
	4)  短路法则
		int a = 10;
		(a == 0) && (a++);// 0, a == ?
		a = 10;
		a || (a--) // 1, a == ?

/*===============================================*/

#include <stdio.h>
#include <stdlib.h>
// 函数参数 与const
/*
// int fun(const int i)
int fun(int i)
{
	const int j = i;
	// no write i
}
strcpy(char *dest, const char *src);
int *pi;
// 代表指针指向的变量不可变, 指针本身可变
const int *pi;
int const *pi;
// 指针不可变，指针指向的变量可变
int *const pi;
const int * const pi;
int const * const pi;
*/
// main函数参数
int main(int argc, char *argv[], char *env[])
{
	printf("argc = %d\n", argc);	
	int i;
	for(i=0; i<argc;  i++)
	{
		printf("argv[%d]: %s\n", i, argv[i]);
	}
	printf("PATH: %s\n", getenv("PAHT"));
	for(i=0; i<argc;  i++)
	{
		printf("argv[%d]: %s\n", i, argv[i]);
	}
	/*
	extern char **environ;
	char **env = NULL;
	env = environ;
	*/
	while(*env != NULL)
	{
		printf("env: %s\n", *env);
		
		env++;
	}

	return 0;
}
// 9, 函数运用
//     .h  .c
//     库

/*==================================================*/

#include <stdio.h>
// 4, 函数指针
// 1) callback ,HOOK, 回调 + 注册
//typedef PFUNC (void (void)) ;
typedef void (PFUNC)(void) ;
//--------------------------------
// driver
void regist_vga(PFUNC *pinit, PFUNC *pdisp);
void amd_vga_init()
{
	printf("AMD_VGA_Initialized ... \n");
}
void amd_vga_disp()
{
	printf("AMD_VGA_Displayed... \n");
}
//--------------------------------
// kernel
//void (*init)(void) = NULL;
//void (*disp)(void) = NULL;
PFUNC *init = NULL;
PFUNC *disp = NULL;
//void regist_init(void (*pinit)(void))
void regist_init(PFUNC *pinit)
{
	init = pinit;
}
void regist_disp(PFUNC *pdisp)
{
	disp = amd_vga_disp;
}
// void regist_vga(void (*pinit)(void), void (*pdisp)(void))
void regist_vga(PFUNC *pinit, PFUNC *pdisp)
{
	regist_init(pinit);
	regist_disp(pdisp);
}
void vga_disp(void)
{
	if((init != NULL) && (disp != NULL))
		disp();
	else
		printf("[Error]\tNo VGA\n");
}
//--------------------------------
// app
void vga_disp(void);
int my_printf(void)
{
	vga_disp();
	return 0;
}
int main()
{
	regist_vga(amd_vga_init, amd_vga_disp);
//-------------------------------- 
	if(init != NULL)
		init();
//--------------------------------
	// application
	my_printf();
	return 0;
}

/*===================================================*/

/** 函数调用机制
 * 1,  保护现场
 * 2,  函数传参
 *    (1)函数私有空间：栈
 *    (2) 单向、拷贝传递
 *    [3] 调用约定： __cdel
// 3, 作用域与生命周期, static extern auto register
//	 	进程地址空间: 栈, 堆
//   	函数定义在函数内部？
// 	 	1) static
// 	 	生命周期，可见性
// 		(1) 只会被初始化一次，如果不执行初始化，被初始化为0
// 		(2) 程序存在，变量存在
// 		(3) 局部可见
// 		(4) 不是栈区上分配的变量，
// 		堆区
//		void *malloc(size_t len);
//		void free(void *ptr);
		int *pi = malloc(sizeof(int));
//		如果mallo成功，返回地址，如果失败，返回NULL
// 		如果不进行判断, 可能会造成野指针
// 4, 函数指针
// 5, 指针函数: 返回值为地址/指针的函数
// 6, 递归函数, 循环
// 7, 函数参数与数组、指针
// 8, 宏函数
		#define MAX 512
//	 	宏替换
		#define ADD(a, b) ((a) + (b))
		#define MACRO(a) ((a) * (a))
		#define FUN(a) ({ \
			typeof(a) _t; \
			_t = (a); \
			_t * _t; \
	   	})
		inline int fun(int a)
		{
			return a*a;
		}
// 9, 函数运用
//     .h  .c
//     库
// 条件编译
#if 0
	free(pi);
	// 已经被free的地址不能再使用
	pi = NULL;	
#else
	my_free(&pi);
#endif
// 全局变量
// 间接访问
void swap(int a, int b)
{
	int t = a;
	a = b;
	b = t;
	printf("[file: %s, line: %d]\ta = %d, b = %d\n", \
			__FILE__, __LINE__, a, b);

}
int main()
{
	int i = 10, j = 20;
	swap(i, j);
	printf("[file: %s, line: %d]\ti = %d, j = %d\n", \
			__FILE__, __LINE__, i, j);
	return 0;
}
//test
#include <stdio.h>
__attribute__ ((constructor)) int fun()
{
	printf("__before main() ...\n");
}
__attribute__ ((destructor)) int fun1()
{
	printf("__after main() ...\n");
}
int main()
{
	return 0;
}
*/

/*================================================*/

#include <stdio.h>
int main()
{
	// step1
	if(error1)
		goto error1;
	// step2
	if(error2)
		goto error2;
error2:
	// recover step1
error1:

	return 0;
}

/*===================================================*/

/*一维指针*/
#include <stdio.h>
#define SZ 4
int main()
{
	float a[SZ];
	float *ptr = a;// float *
	int i;
	for(i=0; i<SZ; i++)
		a[i] = i+1;
	float (*ptr1) [SZ] = &a; // (float [SZ]) *, float (*) [SZ];
	for(i=0; i<SZ; i++)
		printf("a[%d] = %f, %f\n", i, *(ptr+i), ptr[i]);
	for(i=0; i<SZ; i++)
		printf("a[%d] = %f, %f\n", i, *((*ptr1)+i), (*ptr1)[i]);

	return 0;
}
/*二维指针*/
#include <stdio.h>
int main()
{
	int a[3][4];
	int (*pa) [4] = a; // a ---> int (*)[4]
	// a[0] {[0-3]}
	(*pa)[0] = 1; // a[0][0]
	// a[1] {[0-3]}
	(* (pa+1) )[0] = 1; // a[1][0]
	(* (pa + 1)) [0] = 2;// a[1][0]
	*(*(pa+1) + 0) = 2;
	// *(*(pa + M) + N) ==== pa[M][N]
	printf("a[0][0] = %d\n", a[0][0]);
	printf("a[1][0] = %d\n", a[1][0]);
	int (*paa) [3][4] = &a; // int [3][4]
	(*paa)[0][0]  = 3;// a[0][0]
	printf("a[0][0] = %d\n", a[0][0]);
	// *(*( (*paa) + M) + N) ; (*paa)[M][N]

	return 0;
}
/*三维指针*/
#include <stdio.h>
int main()
{
	int a[2][3][4];
	int (*pa) [3][4] = a; //int (*)[3][4]
	// pa ---> a[0] 
	(*pa)[0][0] = 0;
	*( *( *(pa + 1) + 2 ) + 1 ) = 1; // ---> int (*)[4] a[0] [3][4]
	printf("a[0][0][0] = %d\n", a[0][0][0]);
	printf("a[0][0][1] = %d\n", a[0][0][1]);
	printf("a[1][2][1] = %d\n", a[1][2][1]);

	return 0;
}

/*=============================================================*/

1、指针变量
1）初始化
	int *ptr ; // 野指针, 非法指针
	(1) 类型强转(float)1
		(int *)100
	(2) (int *)0  NULL  空指针
	int *pi = (int *)0;
	if(pi != (int *)0)
		*pi = 1;
	float *pf = NULL;
	(3) void : 无
	void *空类型指针，无确切类型指针
	#define NULL (void *)0
2)指针运算
	(1)加减 : 偏移
		int a;
		int b;
		int *pa = &a;
		int *pb = &b;
		//pa + pb;
		pa - pb; 得到两个地址间元素个数
		<1> pa, pb 类型一致
	(2)关系
	
/*=====================================================*/

/*位运算*/
#include <stdio.h>
int main()
{
	//有符号、无符号数，左移位都补0
	// 有符号数，右移位，补符号位, 算术移位
	unsigned int a = 0x80000000;
	unsigned int c = 0xe0000000;
	unsigned int m = a % c;
	printf("a = %u, c = %u, m = %u\n", a, c, m);
	int b;
	b = a >> 2; // a = a >> 2;
	printf("a = 0x%08x, b = 0x%08x\n", a, b);
	// 无符号数，右移位，补0 , 逻辑移位
	unsigned int a1 = 0x80000000;
	unsigned int b1;
	b1 = a1 >> 2;
	printf("a1 = 0x%08x, b1 = 0x%08x\n", a1, b1);

	return 0;
}
#include <stdio.h>
int main()
{
	int a = 0x80000000;
	int b;
	b = a << 2;
	printf("a = 0x%08x, b = 0x%08x\n", a, b);

	return 0;
}
#include <stdio.h>
int main()
{
	unsigned int a = 0x01;
	int b;
	b = a << 31;
	printf("a = 0x%08x, b = 0x%08x\n", a, b);

	return 0;
}
#include <stdio.h>
int main()
{
	unsigned int a = 0x80;
	int b;
	b = a >> 2;
	printf("a = 0x%08x, b = 0x%08x\n", a, b);

	return 0;
}

/*=============================================*/

struct TYPE {
	int a;
	float f;
	char name[32];
};
#define TYPE_T struct TYPE
typedef struct TYPE TYPE_t;
struct TYPE *ptp1;
tp1->a = 1;
TYPE_T tp2;
TYPE_t tp2;
int main()
{
	// MJPEG;
	fopen();
	fread();
	fwrite();
	fclose();

	return 0;
}

/*=======================================*/

变量
1、类型
	1）大小
	2）数据操作	
2、定义变量 —— 划空间
	1）类型
	2）取名—— 标识符
		（1）不能是C语言的关键字
		（2）组成：数字、字母、下划线
		（3）不能以数字开头
	变量声明 —— 不划空间
3、C语言支持的类型
	sizeof，关键字，	
	【范围】	
	基本数据类型	
	整型 signed unsigned 
		signed int, unsigned int, int, sizeof(int)
		short, long, long long		
		// char，与signed char对应关系不是必然的。
		char, signed char, unsigned char
	小数类型（浮点类型）IEEE754
		float(4), double(8)
	字符类型
		char
		
/*================================================*/

类型转换
1, 安全的数据转换, 类型提升
2，不安全数据类型转换
3, 隐式类型转换：
4，显式类型（强制性）类型转换
1)符号位扩展:针对有符号数
2)高位补0:针对无符号数
int main()
{
	char ch;		
	int i = 10;
	ch = i;
	ch = (char)i;

	return 0;
}
5, 补码
1)正数补码： 符号位0 + 正数原码
2)负数补码: 符号位1 + (原码取反 + 1)

/*==============================================*/

/*变量声明*/
#include <stdio.h>
// 声明： 不划空间，仅告诉编译器有标识符可用
// 定义：一定划空间
int main()
{
	extern int i;
	printf("i = %d\n", i);

	return 0;
}
/*变量使用*/
#include <stdio.h>
int main()
{
	// 先定义后使用
	// radious = 1;
	// 定义一个变量， 划空间
	int radious; // 定义语句
	long l;
	// 通过变量名直接访问
	radious = 10; // 赋值语句
	l = 20;
	l = radious;
	// %d打印整数
	printf("sizeof(int) = %d, sizeof(radious ) = %d\n", \
			sizeof(int), sizeof(radious));
	printf("sizeof(long) = %d, sizeof(l) = %d\n", \
			sizeof(long), sizeof(l));
	printf("hello world\n");
	printf("radious = %d\n", radious);
	printf("l = %d\n", l);

	return 0;
}
/*使用sizeof*/
#include <stdio.h>
// 用gdb查看内存存储方式
// x/4xb 
int main()
{
	float f1;
	double f2;
	int i = 1;
	f1 = 1;
	// %d打印整数
	printf("sizeof(float) = %d, sizeof(f1) = %d\n", \
			sizeof(float), sizeof(f1));
	printf("sizeof(double) = %d, sizeof(f2) = %d\n", \
			sizeof(double), sizeof(f2));

	return 0;
}
/*直接通过地址改值*/
#include <stdio.h>
int i;
int main()
{
	//i = 10;
	*((int *)0x804957c) = 20;
	printf("i = %d, &i = %p\n", i, &i);

	return 0;
}

/*===================================================*/

作用域与生命周期
1、作用域：可见性
1)复合语句 {} ： 栈 auto 
2) 可见性：
	(1)局部变量：从定义或声明开始，到相应的}结束
	(2)全局变量：从定义或声明开始，全局可见
	// 全局变量没有初始化，那么将会被初始化0
	// 局部变量没有初始化，将是随机值
	int i;   //全局变量i
	int main()
	{	
	// 隐藏全局i
	int i = 1;
	{
		int i = 2;
		printf("i = %d\n", i); // 使用本复合语句块内定义的i（2）
	}
	printf("i = %d\n", i);	// %d 以有符号形式打印正数
							// %x 以十六进制打印无符号数
							// %f 打印小数 %lf 打印double类型小数
							// %c 打印字符
							// %u 以十进制方式打印无符号数

	return 0;
	}
	(3)static
2、生命周期
	(1)局部变量：从定义开始， 到相应的}结束
	(2)全局变量：程序存在，变量存在
	(3)static
1、使用变量
extern int i;
i = 10;
2、定义变量：分配空间
int i;
3、代码组织
#include <stdio.h>
变量声明：存放在头文件，#include 

/*=======================================================*/