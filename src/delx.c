#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	int data;
	struct node *next;
} Node;

void Del_X(Node **L, int x)
{
	Node *p = *L;
	if (p == NULL)
		return;
	if (p->data == x)
	{
		*L = p->next;
		//L = &(p->next);
		free(p);
		Del_X(L, x);
	}
	else
	{
		L = &(p->next);
		Del_X(L, x);
	}
}

Node* CreatList(int n)
{
	Node *p = NULL;
        Node *tmp;
        for (int i= 0; i < n; ++i)
        {
                tmp = malloc(sizeof(Node));
                tmp->data = i/3;
                tmp->next = p;
                p = tmp;
        }
	return p;
}

void ShowList(Node *tmp)
{
	for (; tmp != NULL; tmp=tmp->next)
        {
                printf("%d\n", tmp->data);
                printf("%p\n", tmp->next);
        }
}
//为了验证引用可以用二重指针代替
int main(int argc, char * argv[])
{
	Node *p = CreatList(10);
	ShowList(p);
	Del_X(&p, 2);
	ShowList(p);

}
