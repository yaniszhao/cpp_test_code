#include <iostream>
#include <vector>
using namespace std;
const int N = 1e6;
vector<int> map;

int Find(int idx, int &n)
{
	int cnt=0;
	while (map[idx]!=idx) {
		idx = map[idx];
		cnt++;
	}
	n=cnt;
	return idx;
}

void Union(int x, int y)
{
	int topx,topy,nx,ny;
	topx = Find(x, nx);
	topy = Find(y, ny);
	if (topx!=topy) {
		if (nx<ny)
			map[topx] = topy;
		else
			map[topy] = topx;
	}
	return;
}

//用并查集做的，AC了
int main(int argc, char *argv[])
{
    //这题不优化输入会超时
	ios::sync_with_stdio(0);
	cin.tie(0);
    //
    int n,val;
    cin>>n;
	map.resize(n+1);
    //初始化
    for (int i=1; i<n+1; i++) {
        map[i]=i;
    }
    //更新
	for (int i=1; i<n+1; i++) {
		cin>>val;
        Union(i, val);
    }
    //计算
	int cnt=0;
    for(int i=1; i<n+1; i++)
		if (map[i]==i) cnt++;
    cout<<cnt;
    return 0;
}