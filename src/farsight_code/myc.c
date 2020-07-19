/*实现将十进制换成二进制*/
#include <stdio.h>
//fun_d2b函数定义
void fun_d2b(char x)                  
{
	int i; 
	int a[8] = { 0 };
	for (i = 0; i < 8; i++) {
		a[7 - i] = x % 2;   //求余
		x = x / 2;        //将剩下的赋给x，以便下次求余
	}
	printf("num=");
	for (i = 0; i < 8; i++) {
		printf("%d", a[i]);
	}
	printf("\n");

}
int main(void)            
{	
	char ch = 10;
	fun_d2b(ch);
	
	return 0;
}

/*==========================================================*/

/*打印三位数的水仙花数*/
#include<stdio.h>
int main(void)
{
	int num, i, j, k;
	for (num = 100; num < 1000; num++) {
		i = num / 100;                 //得到百位数
		j = (num % 100) / 10;   		//得到十位数
		k = (num % 100) % 10;			//得到个位数
		if (num == (i*i*i + j*j*j + k*k*k)) {
			printf("%d\t", num);	
		}
	}	
	putchar(10);

	return 0;
}

/*=======================================================*/

/*找到数组内最大值*/
#include<stdio.h>
//函数定义
void findmax(int s[], int t, int *max)
{
	int i;
	*max = 0;
	for (i = 0; i < t; i++) {
		if (s[*max] < s[i]) {	
			*max = i;
		}
	}
}
int main(void)
{
	int k;
	int a[10] = {12,23,34,45,56,67,78,89,11,22};
	findmax(a, 10, &k);
	printf("max = %d\nk = %d\n", a[k], k);

	return 0;
}

/*===================================================*/

/*使用三种排序法排序*/
#include<stdio.h>
#include<time.h>
//产生十个0到99的随机数，并打印出来
void creat_arr(int *arr)
{	
	srand((unsigned)time(NULL)); //以时间为种    
	int i;
	for (i = 0; i < 10; i++)         
		arr[i] = rand() % 100;
	printf("The ten random number is(0-99):\n");
	for (i = 0; i < 10; i++) 
		printf("arr[%d] = %d\n", i, a[i]);
}
//打印排序后结果
void print_arr(int *arr)
{
	for (i = 0; i < 10; i++)
		printf("arr[%d] = %d\n", i, arr[i]);
}
//冒泡排序函数的定义
void maopao(int *arr)      
{
	int i, j, k = 0;
	for (i = 0; i < 10; i++) {	
		for (j = i + 1; j < 10; j++) {	
			if (arr[i] > arr[j]) {
				k = arr[i];
				arr[i] = arr[j];
				arr[j] = k;
			} 
		}	
	}
}
//插入排序法定义
void insertsort(int *arr)        
{
	int i;	
	for (i = 1;i <= 10; i++) {	
		int x = arr[i], j = i - 1;
		while (x < arr[j] && j >= 0) {
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = x;
	}
}
//二元选择排序法定义
void selectsort(int *arr)        
{
	int i, j;	
	for (i = 0; i < 10 / 2; i++)
	{
		int *max = &arr[10 - i], *min = &arr[i], tmp = 0;
		for (j = i; j < 10 - i; j++) {
			if (arr[j] > *max) {
				tmp = *max;
				*max = arr[j];
				arr[j] = tmp;
			}
			if (arr[j] < *min) {
				tmp = *min;
				*min = arr[j];
				arr[j] = tmp;
			}
		}
	}
}
int main(void)
{	
	int a[10] = { 0 }; 
	
	creat_arr(a);	//产生十个随机数并用冒泡法序排序
	maopao(a);                   
	printf("After maopao,the result is:\n");
	print_arr(a);
	
	creat_arr(a);	//产生十个随机数并用插入法序排序
	insertsort(a);
	printf("After insertsort,the result is:\n");
	print_arr(a);

	creat_arr(a);	//产生十个随机数并用二元选择法排序
	selectsort(a);
	printf("After selectsort,the result is:\n");
	print_arr(a);		

	return 0;
}

/*====================================================*/

/*实现字符串的拷贝*/
#include<stdio.h>
//定义函数
char *str_cpy(char *str, const char *dest)
{
	char *pstr = str;
	//while ((*str++ = *dest++) != '\0');
	while ((*str = *dest) != '\0') {
		str++;
		dest++;
	}
	
	return pstr;
}
int main(void)
{
	char str[20] = { '\0' }, dest[12] = "i am yanis.";
	char *string = str_cpy(str, dest);
	printf("dest = %s\n", dest);
	printf("string = %s\n", string);
	
	return 0;
}

/*======================================================*/

/*完成在str1中搜索str2*/

#include<stdio.h>

//函数定义
char *str_search(const char *str1, const char *str2)
{
	if (*str2 == '\0') {
		printf("the arg is null!");
		return NULL;
	}
	const char *loop, *pstr1, *pstr2 ;   //这里必须有const
	for (loop = str1; *loop != '\0'; loop++)
	{
		pstr1 = loop;
		pstr2 = str2;
		//while (*pstr1++ == *pstr2++ && *pstr2 != '\0');
		while (*pstr1 == *pstr2 && *pstr2 != '\0') {
			pstr1++;
			pstr2++;
		}
		if (*pstr2 == '\0')   break;
	}
	
	if (*pstr2 == '\0')  
			 return (char *)pstr1;//找到返回所在地址
	else       
			 return NULL;       //找不到返回空指针
}
int main(void)  
{
	char str[20] = "i am yanis.",  dest[6] = "yanis";
	char *result = str_search(str, dest);
	if (result == NULL)  
		printf("did not find dest!\n");
	else  
		 printf("have found dest!\n");
	
	return 0;
}

/*=====================================================*/

/*打出杨辉三角的前十排*/
#include<stdio.h>
int main(void)
{
	int a[10][10] = { 0 };
	int i, j;
	//赋值
	for (i = 0; i < 10; i++) {
		a[i][0] = 1;	//把为1的地方填好
		a[i][i] = 1;
		for (j = 1; j < i; j++)
			a[i][j] = a[i - 1][j - 1] + a[i - 1][j];
	}
	//打印
	for (i = 0; i < 10; i++) {
		for (j = 0; j <= i; j++) {
			printf("%d\t", a[i][j]);
		}
		putchar(10);//每一排打完换行
	}

	return 0;
}

/*====================================================*/

/*利用zlib的函数完成一些功能*/
#include <stdio.h>
#include <string.h>
#include <zlib.h>
#include <stdlib.h>
//执行文件后 可不接参数 或一个参数 或重定向数据
int main(int argc, char *argv[])
{
	char *text = NULL;
	unsigned long tlen;
	//得到文本及长度
	if (argc > 2){  //操作系统给的参数不能大于1个        
		printf("Only one obeject can be compress!Please retry...\n");
		return -1;
	}	
	else if (argc == 2) { //接一个参数时，文本内容从参数获得   
		tlen = strlen(argv[1]) + 1;
		text = argv[1];
		text[tlen - 1]= '\0';
	}	
	else {			//不接参数时手动输入或接收重定向数据
		printf("Please input string which you want to compress:\n");
		char ch = getchar();   //吸收上一句的换行符
		char *straccept = &ch;
		straccept++;					//本来此处直接用一句gets就可以了
		text = straccept;				//，用getchar更麻烦了，不过修改while
		ch = getchar();					//的判断句可以接收换行符。也就是可以
		while (ch != 10 && ch != '\0') {	//更好的接收重定向。
			*straccept = ch;
			straccept++;
			ch = getchar();
		}
		*straccept = '\0';
		tlen = strlen(text) + 1;
	}
	//打印接收的数据
	printf("[The text is]:%s\n", text);   
	char *buff = NULL;
	unsigned long blen = compressBound(tlen);
	//判断分配内存是否成功
	if (buff = (char *)malloc(sizeof(char) * blen) == NULL) {
		printf("Failed to allocate memory!\n");
		return -1;
	}	
	//判断并压缩
	if (compress(buff, &blen, text, tlen) != Z_OK) {
		printf("Failed to compress!\n");
		return -1;
	}
	else {				//压缩成功后打印:压缩后内容，压缩前大小，
						//					压缩后大小，和压缩界限大小
		printf("[After compressed,text is] :%s\n", buff);	
		printf("[The original size was]:%d\n", strlen(buff) + 1);
		printf("[Compressed size is]:%lu\n", tlen);
		printf("[The compressBound is]:%lu\n", blen);
	}
        //解压后内容赋予文本2，用于比较是否失真
	char text2[tlen];
	if (uncompress(text2, &tlen, buff, blen) != Z_OK) {
		printf("Failed to uncompress!\n");
		return -1;
	}
	else {       //比较原始文件和解压文件，是否失真，并打印结果
		printf("[The original text is] :%s\n",text); 
		printf("[Uncompressed text is] :%s\n",text2); 
		if (strcmp(text, text2)) {
			printf("Uncompressed text is different from before!\n");
		}
		else {
			printf("Uncompressed text is same as before!\n");
		}
	}
	//释放内存
	if (buff != NULL) {
		free(buff);
		buff = NULL;
	}
	
	return 0;
}