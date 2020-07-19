#include <strings.h>
#include <stdio.h>

#define SIZE 100

#define HEAD 1
#define TAIL -1

#define UP   1
#define DOWN 0 

/*sequence list is made up of array and size of array.
 *        _________________________________           _____
 *data -->|___|___|___|___|___|___|___|___|   num-->  |___|
 *insert: cyclic shift and insert.
 *delete: -by offset:find num and be instead of next data, set last data zero.
 		  -by data:  find data and ...
 *select: traversal.
 *change: select and change.
 */
typedef int data_t;

typedef struct list{
	data_t data[SIZE];
	int num;
}LIST;


void init_list(LIST *L)
{
	bzero(L, sizeof(LIST));
//	memset(L, 0, sizeof(LIST));
}

void insert_list(LIST *L, data_t data, int offset)
{
	int n;
	switch (offset) 
	{
	case TAIL:		
		L->data[L->num] = data;
		L->num++;
		break;
	case HEAD:
		n = L->num;
		while(n--) { 
			L->data[n + 1] = L->data[n];
		}
		L->data[0] = data;
		L->num++;
		break;
	default:
		if ((offset < L->num) && (offset > HEAD)) {
			n = L->num  - offset + 1;
			while (n--) {
				L->data[n + offset] = L->data[n + offset - 1];
			}
			L->data[offset - 1] = data;
			L->num++;
		} else {
			printf("bad offset!\n");
		}
		break;		
	}
}

/* sort UP or DOWN */
void sort_list(LIST *L, int flag_l)
{	//bubble sort
	int i, j, tmp;
	for (i = 0; i < L->num - 1; i++) {
		for (j = 0; j < L->num - i - 1; j++ ) {
			if (L->data[j] > L->data[j + 1]) {
				tmp = L->data[j + 1];
				L->data[j + 1] = L->data[j];
				L->data[j] = tmp;
			}
		}
	}
	if (flag_l != UP) {
		int i = 0, tmp;
		for (; i < L->num / 2; i++) {
			tmp = L->data[i];
			L->data[i] = L->data[L->num - i -1];
			L->data[L->num - i -1] = tmp;
		}
	}
}

void delete_list(LIST *L, data_t data, int offset)
{
	if (offset >= 0) {//by offset	
		if (offset <= L->num) {
			int n = L->num - offset;
			while (0 <= n--) {
				L->data[offset - 1] = L->data[offset];
			}	
			L->data[L->num - 1] = 0;
			L->num--;
		}
	} else {//by data
		int i = select_list(L, data);//return offset
		if (-1 != i) {
			delete_list(L, -1, i);
		} else {
			printf("cannot delete data");
		}
	}
}

void change_list(LIST *L, data_t old_data, data_t new_data)
{
	int i = select_list(L, old_data);
	if(-1 != i)
		L->data[i] = new_data;
}

int select_list(LIST *L, data_t data)
{
	int i = 0;
	for (;i < L->num; i++) {
		if (data == L->data[i])
			return i;
	}

	return -1;
}

void show_list(LIST *L)
{
	int i = 0;
	for (;i < L->num; i++)
		printf("%2d, ", L->data[i]);
	printf("\n");	
}

/* sequence list */
void main(void)
{	
	LIST L;
	init_list(&L);
		
	srand((unsigned)time(NULL));//srand() for rand();
	int i = 10;
	while(i--)
		insert_list(&L, rand() % 100, HEAD);
	printf("the ten number is:\n");
	show_list(&L);
	
	insert_list(&L, rand() % 100, 4);	
	printf("after insert, the result is:\n");
	show_list(&L);

	delete_list(&L, 5, 6);
	printf("after delete, the result is:\n");
	show_list(&L);	

	sort_list(&L, UP);	 
	printf("after sort, the result is:\n");
	show_list(&L);	
}

/* ==================================================== */

#include <strings.h>
#include <stdio.h>
#include <malloc.h>

#define HEAD 1
#define TAIL -1

/*linked list is made up of node(head is node too), but don't need size(traversal).
 * ______ next______ next   ______ next  ______next
 * |head| --> |node1| -->   |node2| -->  |node3|-->  NULL(tail)
 *insert: (e.g:node2)create node2, node2->next = node1->next, node1->next = node2.
 *delete: -by offset:(e.g:node2) shift offset times to node1, create tmp_node(tmp_node = node1->next),
					node1->next = tmp_node->next, free tmp_node.
 		  -by data:  if data == node->next->data then ...
 *select: traversal(if data == node->next->data).
 *change: select and change.
 */
typedef int data_t;

typedef struct node {
	data_t data;
	struct node *next;
}LIST;

void init_list(LIST *L)
{
	L->data = -1;
	L->next = NULL;
}

int insert_list(LIST *L, data_t data, int offset)
{
	LIST *node = malloc(sizeof(LIST));
	node->data = data;
	node->next = NULL;

	switch (offset)
	{
	case HEAD:
		node->next = L->next;
		L->next = node;
		break;
	case TAIL:
		while (L->next) {
			L = L->next;
		}
		node->next = L->next;
		L->next = node;
		break;
	default:
		if (offset <= 0) {
			fprintf(stderr, "offset should gt 0\n");
			return -1;
		} else {
			while (--offset) {	
				if (NULL == L->next)// if offset overflow
					break;		
				L = L->next;
			}
			node->next = L->next;
			L->next = node;
		}
	}
	return 0;
}

//delete by offset if offset > 0, or by data
int delete_list(LIST *L, int offset, data_t data)
{	//by offset
	if (offset > 0)		
	{				
		while (--offset) {
			if (L->next == NULL)//if offset overflow
				break;
			L = L->next;
		}
		
		if (L->next != NULL) {//not at tail
			LIST *tem = L->next;			
			L->next = tem->next; 
			free(tem);
		}		
	} else {//by data
		while (L->next)	{
			if (L->next->data == data)
				break;
			L = L->next;
		}
	
		if (L->next) {//if have found
			LIST *tem = L->next;
			L->next = tem->next;
			free(tem);
		}
	}
}

void change_list(LIST *L, data_t old_data, data_t new_data)
{
	while (L->next) {
		if (L->next->data == old_data) {
			L->next->data = new_data;
			//break;
			return;
		}			
		L = L->next;
	}
	
	printf("change_list:can't find data\n");
	return;
}

/* return 1 if have found, or return 0 */
int select_list(LIST *L, data_t data)
{	
	while (L->next) {	
		if (L->next->data == data)
			return 1; 
		L = L->next;
	}
	return 0;
}

void show_list(LIST *L)
{
	while (L->next) {
		printf("%d, ", L->next->data);
		L = L->next;
	}
	printf("\n");
}

/* linked list */
int main(void)
{	
	LIST L;
	init_list(&L);

	int i = 10;
	int ret;
	while (i--) {
		if (0 != (ret = insert_list(&L, i, HEAD))) {
			fprintf(stderr,"insert error.\n");
			return -1;
		}
	}
	show_list(&L);
		
	if (0 != (ret = insert_list(&L, 888, 3))) {
		fprintf(stderr,"insert error.\n");
		return -1;
	}
	
	show_list(&L);
	
	delete_list(&L, 4, -1);
	show_list(&L);

	return 0;
}
