#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <queue>
#include <map>
using namespace std;
int main(int argc, char **argv)
{
	string needle("abadfababadg");
	int n2 = needle.size();
	int next[n2];
	next[0]=-1;
        //n2-1注意减1否则next会越界，就是为什么下面种方法开vector失败的原因
	int i,j;
        for (i=0,j=-1; i<n2-1&&j<n2;)
            if (j<0 || needle[i]==needle[j]) {i++; j++; next[i]=j;}
            else j=next[j];
	for (i=0; i<n2; i++)
		cout<<next[i]<<endl;
	return 0;
}




