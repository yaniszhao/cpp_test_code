#include <malloc.h>
#include <stdio.h>

/*              A
			B        C
			
 *DLR:A->B->C   LDR:B->A->C     LRD:B->C->A 
 
 *SHOW:A(B,C)->B(B1,B2)->C(C1,C2)->B1(B11,B12)->B2(,)->C1(,)...
 
 *DLR:	data			LDR: LDR(node->l)	LRD: LRD(node->l)
		DLR(node->l)		 data				 LRD(node->r)
		DLR(node->r)		 LDR(node->r)   	 data
 */

typedef int data_t;

#if 1 //queue
typedef struct node NODE;
typedef struct que{
	NODE *head;
	NODE *tail;
}QUE;

struct node{
	data_t data;
	struct node *next;
};

void init_que(QUE *Q)
{
	Q->head = Q->tail = NULL;
}

int isnull_que(QUE *Q)
{
	return Q->head == NULL ? 1 : 0;
}

void enque(QUE *Q, data_t data)
{
	NODE *node = malloc(sizeof(NODE));
	node->data = data;
	node->next = NULL;

	if(NULL == Q->head)
	{
		Q->head = Q->tail = node;
		return;
	}

	Q->tail->next = node;
	Q->tail = node;
}

data_t outque(QUE *Q)
{
	NODE *tem = Q->head;
	Q->head = tem->next;

	data_t data = tem->data;
	free(tem);

	return data;
}
#endif

typedef struct tree_node{
	data_t data;
	struct tree_node *l;
	struct tree_node *r;
}TREE;

static TREE* creat_treenode(data_t data)
{
	TREE *node = malloc(sizeof(TREE));
	node->data = data;
	node->l = node->r = NULL;

	return node;
}

TREE *creat_tree(const int *a, int len)
{
	TREE *root = creat_treenode(a[0]);
	TREE *tem = root;//return tem
	TREE *node;
	int i = 1;
	for(; i<len; i++) {
		root = tem;
		node = creat_treenode(a[i]);

		while (1) {
			if (node->data < root->data)	{
				if (root->l == NULL)	{
					root->l = node;
					break;
				} else {
					root = root->l;
				}
			} else {
				if (root->r == NULL) {
					root->r = node;
					break;
				} else {
					root = root->r;
				}
				
			}
		}
	}
	return tem;
}

/* data leftchild rightchild, preorder traversal */
void DLR(TREE *root)
{
	if(NULL == root)
		return;
	printf("%d,", root->data);
	DLR(root->l);//function recursive
	DLR(root->r);
}

/* leftchild data right, inorder traversal */
void LDR(TREE *root)
{
	if(NULL == root)
		return;
	LDR(root->l);
	printf("%d,", root->data);
	LDR(root->r);
}

/* leftchild rightchild data, postorder traversal */
void LRD(TREE *root)
{
	if(NULL == root)
		return;
	LRD(root->l);
	LRD(root->r);
	printf("%d,", root->data);
}

void LEVE_SHOW(TREE *root)
{
	TREE *node;

	QUE Q;
	init_que(&Q);

	enque(&Q, (data_t)root);//root is a address and is changed to int
	
	while (!isnull_que(&Q)) {
		node = (TREE *)outque(&Q);
		printf("%d,", node->data);
		if(node->l)
			enque(&Q, (data_t)node->l);
		if(node->r)
			enque(&Q, (data_t)node->r);
	}
	printf("\n");
}

int main()
{
	int a[] = {3, 4, 1, 78, 3, 1, 56, 8, 2};

	TREE *root = creat_tree(a, sizeof(a)/sizeof *a);

	DLR(root);
	printf("\n");
	LDR(root);
	printf("\n");
	LRD(root);
	printf("\n");

	LEVE_SHOW(root);
	
	return 0;
}
