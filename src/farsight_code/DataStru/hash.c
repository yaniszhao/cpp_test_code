/*hash table的常用方法：
 *1、直接寻址法
 *2、数字分析法
 *3、平方取中法
 *4、折叠法
 *5、随机数法
 *6、除留余数法（最常用）
 */
#include <strings.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

/*除留余数法
 *      _________   _______ next _______ next _______next
 *HASH->|data[0]| ->|node3| -->  |node2| -->  |node1|-->NULL
 *      |data[1]| ->...
 *      |data[2]| ->...
 *          .
 *			.
 *			.
 *insert: (e.g:node4)if node4.data % size == 0, node4->next = H->data[0],
						H->data[0] = node4.(result: data[0]-->node4-->node3)
 *select:according to data % size, find data[i], then traversal and compare.						
 */ 
typedef struct node NODE;
typedef struct hash{
	NODE **data;
	int size;
}HASH;
typedef int data_t;
struct node{
	data_t data;
	struct node *next;
};

void init_hash(HASH *H, int size)
{   
	//H->data[i] filled by address of NODE
	H->data = malloc(size * sizeof(NODE*));
	
	//H->data[i] = NULL, H->size = 0;
	bzero(H->data, size * sizeof(NODE*));
	
	H->size = size;
}

void insert_hash(HASH *H, data_t data)
{
	NODE *node = malloc(sizeof(NODE));
	node->data = data;
	node->next = NULL;
	
	node->next = H->data[data % H->size];
	H->data[data % H->size] = node;
}

static void show_list(NODE *head)
{
	while (head) {
		printf("%d,", head->data);
		head = head->next;
	}
	printf("\n");
}

void show_hash(HASH *H)
{
	int i = 0;
	for (;i < H->size; i++) {
		show_list(H->data[i]);
	}
}

#if 0 //my code
void creat_hash(HASH *H[MOD])
{	//创建节点
	HASH *node = malloc(sizeof(HASH));
	node->data = rand() % 100; 
	node->next = NULL;
	int i = node->data % MOD;//可以不要这步，直接带入数组下标。但写上简单些
	//插入节点
	node->next = H[i];
	H[i] = node;
	
}

void show_hash(HASH *H[MOD])
{
	HASH **tmp = H;//这千万不能写成HASH *tmp[MOD]=H，因为数组的地址是不能变的，能变的只是里面的值。
					//其实也不用这句话，因为其实函数调用玩H里的地址也不会变（没有传H的地址）。
	int i = MOD;
	while (i--) {
		printf("%%[%d]:", MOD - i - 1);
		while (tmp[MOD - i - 1]) { 
			printf("%d,", tmp[MOD - i - 1]->data);
			tmp[MOD - i - 1] = tmp[MOD - i - 1]->next;
		}
		putchar(10);
	}

}
#endif

int main()
{
	HASH H;
	init_hash(&H, 13);
	
	srand((unsigned)time(NULL));
	
	int i = 100;
	while(i--)
		insert_hash(&H, rand()%100);
									
	show_hash(&H);

	return 0;
}
