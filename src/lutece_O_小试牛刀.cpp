#include <iostream>
#include <string.h>
#include <string>
using namespace std;
//TLE
int main1(int argc, char *argv[])
{
    string s1,s2;	
	cin>>s1>>s2;
	const char *p1=s1.c_str();
	const char *p2=s2.c_str();
	const char *p3=p1;
	const char *p=strstr(p3, p2);
	size_t last=0;
	while (p) {
		size_t pos = p-p3+1;
		size_t abs_pos=last+pos;
		last = abs_pos;
		cout<<abs_pos<<' ';		
		p3 = p1 + abs_pos;
		p = strstr(p3, p2);
	}
    return 0;
}

#include <vector>
//改成KMP算法还是超时
int main(int argc, char *argv[])
{
    string haystack,needle;	
	cin>>haystack>>needle;
	int n1=haystack.size(), n2=needle.size();
    vector<int> next(n2);
    int i,j;
    //获得next数组
    next[0]=-1;
    for (i=0,j=-1; i<n2-1&&j<n2;)
        if (j<0 || needle[i]==needle[j]) {i++; j++; next[i]=j;}
        else j=next[j];
	//
    for (i=0,j=0; i<n1;) {
		if (j<0 || haystack[i]==needle[j]) {i++; j++;}
		else j=next[j];
		//
		if (j==n2) {
			cout<<i-j+1<<' ';
			i=i-j+1;
			j=0;
		}
	}

    return 0;
}
