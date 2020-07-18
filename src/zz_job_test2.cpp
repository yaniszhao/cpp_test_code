#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <queue>
#include <map>
using namespace std;
int main(int argc, char **argv)
{
	int name_map[26]={0};
	string line;
	cin>>line;
	for (auto c : line)
		map[c-'A']++;

	vector<int> ans;
	int cnt=0;
	int nums = line.size();
	int last=line[0]-'A';
	queue<int>  q;
	q.push(last);

	last=-1;
	int idx=0;
	while (!q.empty()) {
		int cur = q.front();
		q.pop();
		map[cur]--;
	
		if (map[cur] == 0) {
			if (q.empty()) {
				ans.push_back(idx-last);
				if (idx == nums-1) break;
				q.push(line[++idx] - 'A');
			}
			else {
				idx++;
				
			}
		}
		else {
			q.push(line[++idx] - 'A');
		
		}


	}


	cout<<ans<<endl;
	return 0;
}




