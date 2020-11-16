#include <iostream>
#include <fstream>
using namespace std;
const int N = 5001;
int len, p1, p2;
short f[N][N];
char str[N];

inline short smin(short a, short b)
{
    return a < b ? a : b;
}
//网上找的一个代码，方法是dp
int main()
{
    cin>>len>>str;
    for(int i = 0; i < len; i++)
        f[i][i] = 0;
    for(int width = 1; width < len; width++)
    {
        for(int i = 0; i < len-width; i++)
        {
            f[i][i+width] = smin(f[i+1][i+width]+1, f[i][i+width-1]+1);
            if(str[i] == str[i+width])
                f[i][i+width] = smin(f[i][i+width], f[i+1][i+width-1]);
        }
    }
    cout<<f[0][len-1]<<endl;
    return 0;
}