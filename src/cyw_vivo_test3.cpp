#include <iostream>
#include <vector>
using namespace std;

//间隔着种树的问题

int fun(vector<int> &v){
	int res=0;
	if (v[0]==0&&v[1]==0) {v[0]=1; res++;}
	for (int i=1; i<v.size()-1; i++)
	{
		if (v[i]==1) continue;
		if (v[i-1]==1) continue;
		if (v[i+1]==1) continue;
		v[i]=1;
		res++;
	}
	if (v[v.size()-2]==0&&v[v.size()-1]==0) {v[v.size()-1]=1; res++;}
		
    return res;
}

int main(int argc, char **argv)
{
	vector<int> v{1,0,0,0,0,0,1,0};
	cout<<fun(v)<<endl;
	return 0;

}
