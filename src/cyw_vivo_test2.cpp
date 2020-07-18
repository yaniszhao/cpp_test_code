#include <iostream>
#include <vector>
using namespace std;

//dp解类似扔鸡蛋的问题

int fun(int K, int N){
    // K=1 or N<=2 : f(K,N) =N
    if( N <= 2 || K == 1 ) return N;
    vector<vector<int>> v(K);
    // 初始化一个K行， N+1列的二维数组（多一个0层方便计算）
    for( int i = 0; i < K; i++ ){
        v[i].resize(N + 1);
        // N<=2 : f(K,N) =N
        v[ i ][ 0 ] = 0;
        v[ i ][ 1 ] = 1;
        v[ i ][ 2 ] = 2;
    }
    // K=1 : f(K,N) =N
    for( int i = 3; i <= N; i++ ){
        v[ 0 ][ i ] = i;
    }
    // aux[e][f] 代表有e个鸡蛋，f层楼时，需要移动的次数
    for( int e = 1; e < K; e++ ){
        int x = 1;
        for( int f = 3; f <= N; f++ ){
            // x取交汇处
            if( v[ e - 1 ][ x - 1 ] < v[ e ][ f - x ] ){
                x++;
            }
            // f(K,N) = max(f(K-1,X-1), f(k,N-X)) + 1
            v[ e ][ f ] = v[ e - 1 ][ x - 1 ] + 1;
        }
    }
    return v[ K - 1 ][ N ];
}

int main(int argc, char **argv)
{
	cout<<fun(6,100)<<endl;
	return 0;

}
