
/* list can change any position(by num(sequence) or traversal(linked)),
 * but stack can't, because it must be last in first out,
 * so it's more simple than list.
 */
#include <stdio.h>
#define SIZE 10

/*sequence stack is made up of array and size of array.
 *        _________________________________           _____
 *data -->|___|___|___|___|___|___|___|___|   top-->  |___|
 *insert: shift top and insert(or: insert and shift top).
 *delete: shift top and delete(or: delete and shift top).
 *select: can't(or: delete and compare).
 *change: can't(or: delete and compare and change).
 */ 
typedef int data_t;
typedef struct node{
	data_t data[SIZE];
	int top;
}STA;

void init_stack(STA *S)
{
#if 0
	S->top = SIZE; //FD:--top, insert data
	S->top = SIZE-1;//ED:insert data, --top
	S->top = 0;//EI:insert data, ++top
#endif
	S->top = -1;//FI:++top, insert data
}

void push(STA *S, data_t data)
{
	S->data[++S->top] = data;
}

data_t pop(STA *S)
{
	return S->data[S->top--];
}

/* sequence stack */
int main()
{
	STA S;
	init_stack(&S);

	int i = 10;
	while (i--)
		push(&S, i);

	printf("%d,", pop(&S));
	printf("%d,", pop(&S));
	printf("\n");

	return 0;
}

/* ======================================================= */

#include <stdio.h>
#include <malloc.h>

/*linked stack is made up of node(head is node too), but don't need size(traversal).
 * ______ next______ next   ______ next  ______next
 * |head| --> |node3| -->   |node2| -->  |node1|-->  NULL(tail)
 *insert: (e.g:node3)create node3, node3->next = head->next, head->next = node3.
 *delete: create tmp_node = head->next, head->next = tmp_node->next, free tmp_node.
 *select: can't(or: delete and compare).
 *change: can't(or: delete and compare and change).
 */
typedef int data_t;
typedef struct node{
	data_t data;
	struct node *next;
}STA;

void init_stack(STA *S)
{
	S->next = NULL;
}

void push(STA *S, data_t data)
{
	STA *node = malloc(sizeof(STA));
	node->data = data;
	node->next = NULL;

	node->next = S->next;
	S->next = node;
}

data_t pop(STA *S)
{
	STA *tem = S->next;
	S->next = tem->next;

	data_t data = tem->data;
	free(tem);

	return data;
}

/* linked stack */
int main()
{
	STA S;
	init_stack(&S);//get head

	int i = 10;
	while (i--)
		push(&S, i);

	printf("%d,", pop(&S));
	printf("%d,", pop(&S));
	printf("\n");

	return 0;
}
