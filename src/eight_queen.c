#include <stdio.h>
#include <stdlib.h>

int y[8];
int cnt=0;

void queen(int u)
{
    for (int v=0; v<8; v++) {
        int flag=1;
        for (int i=0; i<u; i++)
            if (y[i]==v || (u-i)==abs(v-y[i]))
                {flag=0; break;};
        if (!flag) continue;
        if (u==7) {cnt++; continue;}
        y[u]=v;
        queen(u+1);
    }
}

int main(int argc, char *argv[])
{
    queen(0);
    printf("%d\n", cnt);
}
