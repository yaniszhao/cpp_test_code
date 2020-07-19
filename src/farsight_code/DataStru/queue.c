/* insert queue(inqueue) must into tail(don't shift),
 * delete queue(outqueue) must out of head(don't shift),
 * so more simple than list. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


/*			  _____	      _______	    ______	   _____
	     next|node4| next |node3| next |node2|next|node1|
	NULL <---|_____|<---  |_____| <--- |_____|<---|_____|
		        ^			             ^	         ^		
                |                        |           |	
				|___________    _________|_ _ _ _ _ _|     
				Q->tail    \  /Q->head      *tmp
				             Q
 *insert: (e.g:node5)create node5, node5->next = tail(node4)->next, tail(node4)->next = node5.
 *delete: (e.g:node1)create tmp_node = head, head = tmp_node->next, free tmp_node.
 *select: can't(or: delete and compare).
 *change: can't(or: delete and compare and change).							 
 */
typedef int data_t;
struct node {
	data_t data;
	struct node *next;
};
typedef struct node NODE;

typedef struct que{
	NODE *head;
	NODE *tail;
}QUE;
int init_que(QUE *Q)
{
	Q->head = Q->tail = NULL;
	
	return 0;
}

int inque(QUE *Q, data_t data)
{
	NODE *node = malloc(sizeof(NODE));
	node->data = data;
	node->next = NULL;

	if (NULL == Q->head) {//first one
		Q->head = Q->tail = node;
	} else {
		Q->tail->next = node;
		Q->tail = node;
	}
	
	return 0;
}

data_t outque(QUE *Q)
{	
	if (NULL == Q->head) {//no member
		fprintf(stderr, "outque:no member\n");
		exit(-1);
	}
	
	data_t data;
	if (Q->head == Q->tail) {//last one
		data = Q->head->data;
		Q->head = Q->tail = NULL;
	} else {
		NODE *tem = Q->head;
		Q->head = tem->next;
		data = tem->data;
		free(tem);
	}

	return data;
}

int main()
{
	QUE Q;
	init_que(&Q);

	int i = 10;
	while(i--)
		inque(&Q, i);

	i = 5;
	while (i--) {
		printf("%d\n", outque(&Q));
		printf("%p\t%p\n", Q.head, Q.tail);
	}

	i = 5;
	while(i--)
		inque(&Q, i);
	
	i = 10;
	while (i--) {
		printf("%d\n", outque(&Q));
		printf("%p\t%p\n", Q.head, Q.tail);
	}
	
	return 0;
}
