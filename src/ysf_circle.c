#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    int data;
    struct node *next;
} Node, *List; 

void CreateCircle(List head, int n)
{
    Node *p;
    for (int i=n; i>=1; i--) {//逆序头插
        p = (Node *)malloc(sizeof(*p));
        p->data=i;
        p->next=head->next;
        head->next=p;
    }        
    for (p=head; p->next!=NULL; p=p->next);
    p->next=head;//构成循环
}

void ShowCircle(List head)
{
    List p=head->next;
    while (p!=head) {
        printf("%3d ", p->data);
        p=p->next;
    }
    printf("\n");
}

void DelNode(List head, int n, int m)
{
    if (!head->next||m<1) return;
    List pre=head, p=head->next;
    for (; n>0; n--) {
        for (int i=1; i<m; i++) {//只移动m-1次，还有一次再进来之前就移好
            p=p->next;
            pre=pre->next;
            if (p==head) i--;
        }
        printf("delete: %3d-->", p->data);
        pre->next=p->next;//本来想交换数值的但是有头没办法
        free(p);
        p=pre->next;
        if (p==head) {
            p=p->next;
            pre=pre->next;//别忘了更新pre
        }
        ShowCircle(head);
    }
}

int ysf3(int n, int m)
{//用链表是真的麻烦
    Node head;//设个头
    head.next=NULL;
    CreateCircle(&head, n);
    ShowCircle(&head);
    DelNode(&head, n, m);
    return 0;
}

int ysf2(int n, int m)
{
    int *arr, i, num=n;
    arr=(int *)malloc(n*sizeof(*arr));
    for (i=0; i<n; i++) arr[i]=i+1;
    i=-1;
    for (; num>1; num--) {
        for (int t=0; t<m; ) {
            i=(i+1)%n;
            if (arr[i]!=0) t++;
        }
        arr[i]=0;
    }
    for (i=0; i<n&&arr[i]==0; i++);
    printf("ysf2:%d\n", i+1);
    free(arr);
    return 0;
}

int ysf1(int n, int m)
{//用的数学方法
    int ans=0;//只有一个人的时候下标为0
    for (int i=2; i<=n; i++)
        ans=(ans+m)%i;
    printf("ysf1:%d\n", ans+1);
    return ans+1;
}

int main(int argc, char *argv[])
{
    ysf1(14,3);
    ysf2(14,3);
    ysf3(14,3);
    return 0;
}
