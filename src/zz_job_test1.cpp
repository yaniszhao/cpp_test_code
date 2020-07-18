#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <queue>
#include <map>
using namespace std;
int main(int argc, char **argv)
{
	string stName;
	int m;
	cin>>stName;
	cin>>m;
	getchar();

	vector<vector<string>> lines;
	map<string, int> hash;
	int person_num=0; 

	while (m--) {
		string line;
		vector<string> names;

		getline(cin, line);
		const char *line_ptr = line.c_str();
		char *token = strtok((char *)line_ptr, ",");
		while (token) {
			//cout<<token<<endl;
			string name(token);
			names.push_back(name);
			if (hash.find(name) == hash.end())
				hash[name] = person_num++;
			token = strtok(NULL, ",");
		}					
		lines.push_back(names);	
	}


	for (auto it = hash.begin(); it !=  hash.end(); it++)
		;//cout<<it->first<<":"<<it->second<<endl;

	int n = hash.size();
	int v[n][n];
	memset(v, 0, n*n*sizeof(int)); 

	for (int k=0; k<lines.size(); k++) {
		vector<string> names = lines[k];
		int nums = names.size();
		for (int i=0; i<nums-1; i++)
			for (int j=i+1; j<nums; j++) {
				int a = hash[names[i]];
				int b = hash[names[j]];
				v[a][b] = v[b][a] = 1;
			}
	}


#if 0
	for (int i=0; i<n; i++) {
		for (int j=0; j<n; j++)
			cout<<v[i][j]<<" ";
		cout<<endl;
	}
#endif

	int last = hash[stName];
	int vist[n];
	memset(vist, 0, n*sizeof(int));
	vist[last]=1;
	queue<int> q;
	q.push(last);
	int ans = 0;
	while(!q.empty()) {
		int i = q.front();
		q.pop();
		//ans++;
		vist[i]=1;
		for (int j=0; j<n; j++) {
			if (v[i][j] == 1 && !vist[j]) {
				q.push(j);
			}
		}
		if (i == last) last=q.back();
	}

	for (int i=0; i<n; i++)
		if (vist[i]==1) ans++;
	cout<<ans<<endl;
	return 0;
}




