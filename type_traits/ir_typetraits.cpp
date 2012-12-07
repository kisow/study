#include <queue>
#include <iostream>

using namespace std;

int main()
{
	queue<string> q;
	string E[26] = { "bcd", "acd", "abd", "abc" };
	char s = 'a';
	char t = 'd';

	q.push(string(1, s));
	while(!q.empty()) { 
		string path = q.front(); q.pop();
		for(char n : E[path.back() - 'a']) 
			if(n == t) { 
				for(char c : path) cout << c << " - ";
				cout << t << endl;
			} else if(string::npos == path.find(n)) 
				q.push(path + string(1, n));
	}

	return 0;
}
