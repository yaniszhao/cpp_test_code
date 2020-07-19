#include "list.h"
#include "int.h"
#include "stu.h"

/*===============================myint====================================*/
typedef struct myint{
	void (*show)(void *);//这里和STU里的show虽然是void类型，但是特定类型的函数指针，
						//不像之前的data之类的表示任意类型。
	int  (*cmp)(void *, void *);
	int data;
}MYINT;

void show_int(void *data)
{
	printf("%d, ", ((MYINT*)data)->data);
}

void *creat_int(int i)
{
	MYINT *node = malloc(sizeof(MYINT));
	node->show = show_int;
	node->cmp = NULL;
	node->data = i;

	return node;	
}

void free_int()
{

	return;
}
/*===========================================================================*/

/*==============================mystu=====================================*/
typedef struct stu{
	void (*show)(void *);
	int (*cmp)(void *, void *);
	char name[20];
	int score;
	int sex;
}STU;

static void show_stu(void *data)//加了static只能本文件调用
{
	STU *t = (STU*)data;
	printf("\n%s, %d, %d\n", t->name, t->score, t->sex);
}

void *creat_stu(const char *name, int score, int sex)
{
	STU *node = malloc(sizeof(STU));
	node->show = show_stu;//函数名就是地址
	node->cmp = NULL;
	strcpy(node->name, name);//这里不要直接赋值
	node->score = score;
	node->sex = sex;

	return node;//有了函数定义的类型，虽然这是STU*类型，但还是会转换成void*型
}

void free_stu()
{

}
/*===========================================================================*/

/*==============================mylist=====================================*/
typedef struct node{
	void *data;
	struct node *next;
}LIST;

void init_list(LIST *L)
{
	L->next = NULL;
}

void insert_list(LIST *L, void *data)
{	//创建新节点
	LIST *node = malloc(sizeof(LIST));
	node->data = data;
	node->next = NULL;
	//插入到头链表
	node->next = L->next;
	L->next = node;
}

typedef struct func{
	void (*show)(void *);
	int (*cmp)(void*, void *);
}FUNC;/*这里的作用其实是让show强转成void （*）（void *）类型，
		这里FUNC结构体虽然不属于上面任意两种，但是void本来就不属于任何一种，
		只是让show达到了强转类型的目的而已（可以说是
		通过结构体里相同名字的成员找到偏移量，然后才找到类型，因为一旦改变了show
		和cmp的位置，结果就和期待的不一样了）。
		要达到回调目的，那在这的声明部分的函数类型就必须与实现部分的一样，
		而且每种类型的show函数原型必须要一样。*/

void show_list(LIST *L)
{
	while(L->next)
	{	/*下面这可以是因为MYINT类型下恰好有show这个成员，通过成员找到偏移量，
		所以即使不是MYINT类型的stu也能打印是因为偏移量一样。
		((MYINT *)(L->next->data))->show(L->next->data);*/
		
		//show通过FUNC类型找到了函数的类型，加上自己有特定的地址，就成功成了函数指针		
		((FUNC *)(L->next->data))->show(L->next->data);
		L = L->next;
	}
	printf("\n");
}
/*================================================================================*/

int main()
{
	LIST L;
	init_list(&L);

	int i = 10;
	while(i--)
		insert_list(&L, creat_int(i));
	show_list(&L);

	i = 10;
	while(i--)
		insert_list(&L, creat_stu("aaa", 1, 3));
	show_list(&L);
	
	
}

