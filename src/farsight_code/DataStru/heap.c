#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define SIZE 1000
#define OCC   1
#define UNOCC 0

/*the size of heap is unchanged, what can be changed is node.
 *        __________________________________________
 *heap--> |node1|node2|node3(node3->next = NULL)   | node1，node2:occ, node3:unocc
 *molloc:(e.g:node4)create node4, node4->start = node3->start + size, node4->end = node3->end,
						node4->flag = UNOCC, node4->size = SIZE - size, 
						node3->end = node3->start, node3->flag = OCC, node3->size = size,
						node4->next = node3->next, node3->next = node4.
 *free: (e.g:node1)node1->flag = UNOCC.						
 */

typedef struct node LIST;
typedef struct heap HEAP;

struct node {//LIST
	void *start;
	void *end;
	int  size;
	int  flag;
	struct node *p;
};

struct heap {//HEAP
	char data[SIZE];
	LIST *head;
};

HEAP MYHEAP;
	
void init_heap(void)
{
	LIST *node = malloc(sizeof(LIST));
	node->start = MYHEAP.data;
	node->end   = MYHEAP.data + SIZE - 1;
	node->size  = SIZE;
	node->flag = UNOCC;
	node->p = NULL;

	MYHEAP.head = node;
}

void *mymalloc(int size)
{	
	LIST *tem = MYHEAP.head;
	while (tem)	{
		if (tem->flag == UNOCC && tem->size >= size) {
			LIST *node = malloc(sizeof(LIST));
			node->start = tem->start + size;
			node->end   = tem->end;
			node->size  = tem->size - size;
			node->flag = UNOCC;//unoccupied
			node->p = NULL;

			tem->end = tem->start + size;
			tem->size= size;
			tem->flag = OCC;//occupied

			node->p = tem->p; 
			tem->p = node;
			
			return tem->start;	
		}
		tem = tem->p;  
	}
	return NULL;
}

void myfree(void *p)
{
	LIST *tem = MYHEAP.head;
        while (tem) {
			if (tem->start == p) {
				tem->flag = UNOCC;//free
				break;
			}
			tem = tem->p;
		}
}

void show_heap(void)
{
	LIST *tem = MYHEAP.head;
    while (tem) {
		printf("[%p-%p]%d:%d\n",tem->start,
								tem->end,
								tem->size,
								tem->flag);
		tem = tem->p;
	}
	printf("###############################\n");
}

int main(void)
{	
	init_heap();
	show_heap();
		
	char *p = mymalloc(10);
	strcpy(p, "hed");
	printf("%s\n", p);
	show_heap();
	
	myfree(p);
	p = NULL;
	show_heap();

	return 0;
}

