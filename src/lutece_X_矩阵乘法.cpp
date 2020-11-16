#include <iostream>
#include <vector>
using namespace std;
const int N = 1e5;
vector<int> map(N, -1);
//这题没有AC，以后如果想做再说
int main(int argc, char *argv[])
{
    //for (int i=0; i<10; i++) cout<<map[i]<<endl;
    int n,a,b;
    cin>>n;
    for (int i=0; i<n; i++) {
        cin>>a>>b;
        map[a] = b;
    }
    int start=0, end;
    for ( ; (start<N)&&(map[start]==-1); start++);
    if (start==N) {
        cout<<-1;
        return -1;
    }
    end=map[start];
    for (int i=1; i<n; i++) {
        if (map[end]==-1) {
            cout<<-1;
            return -1;
        }
        end=map[end];
    }
    cout<<start*end;
    return 0;
}
