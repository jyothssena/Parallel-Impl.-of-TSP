#include <stdio.h>
#include <bits/stdc++.h>
#include<sys/time.h>
using namespace std;
#define MAX 100000
int n=4,min_path=MAX;
int dist[100][50];

int factorial(int n) {
  if (n == 0 || n == 1) return 1;
  return n * factorial(n -1 );
}

void find_kth_permutation( vector<char>& v, int k, int p, int s) {
  if (v.empty()) {
    p+=dist[s][0];
    min_path=min(min_path,p);
    return;
  }
  
  int n = (int)(v.size());
  int count = factorial(n - 1);
  int selected = (k - 1) / count;
  int x=(int)(v[selected])-48;
  p+=dist[s][x];
  s=x;
  v.erase(v.begin() + selected);

  k = k - (count * selected);
  find_kth_permutation(v, k,p,s);
}


int main(int argc, char* argv[]) {
	for(int i=0;i<8;i++)
		for(int j=0;j<8;j++)
		cin>>dist[i][j];
	struct timeval start,end;
	gettimeofday(&start,NULL);
	int n=7;
    	for (int i = 1; i <= factorial(5); ++i) {
        	vector<char> v;
    		for (char i = 1; i <= 5; ++i) {
        		v.push_back(i + '0');
    		}
    		find_kth_permutation(v, i,0,0);
    		//path+=dist[s][0];
    	}
    gettimeofday(&end,NULL);
    cout<<min_path<<endl;
cout<<(end.tv_sec-start.tv_sec)*1000000L+(end.tv_usec-start.tv_usec)<<endl;
}