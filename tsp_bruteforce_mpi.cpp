#include <mpi.h>
#include <stdio.h>
#include <bits/stdc++.h>
#include<sys/time.h>
#include<iostream>
using namespace std;
#define MAX 100000
int global_min=MAX;
int dist[100][50] = {{ 0, 29, 82, 46, 68, 52, 72, 42}, { 29,0 , 55, 46, 42, 43,43, 23}, 
	{ 82, 55, 0, 68, 46, 55,23,43}, { 46,46,68,0,82,15,72,31},{68,42,46,82,0,74,23,52}, {52, 43, 55,15,74,0,61,23},{72,43,23,72,23,61,0,42},{42,23,43,31,52,23,42,0}};
int factorial(int n) {
  if (n == 0 || n == 1) return 1;
  return n * factorial(n -1 );
}

int find_kth_permutation( vector<char>& v, int k, int p, int s,int min_path,int rank) {
  if (v.empty()) {
    p+=dist[s][0];
    min_path=min(min_path,p);
    return min_path;
  }
  int n = (int)(v.size());
  int count = factorial(n - 1);
  int selected = (k - 1) / count;
  int x=(int)(v[selected])-48;
  p+=dist[s][x];
  s=x;
  v.erase(v.begin() + selected);

  k = k - (count * selected);
  min_path=find_kth_permutation(v, k,p,s,min_path,rank);
  return min_path;
}


int main(int argc, char* argv[]) {
	int rank,size;
	double start,end;	
	MPI_Init(&argc, &argv);
	start=MPI_Wtime();
 	MPI_Comm_size(MPI_COMM_WORLD, &size);
 	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 	int min_path=MAX;
 	int x=factorial(7)/size;
	for (int i = 1 + (x*rank); i <=x*(rank+1); ++i) {
        	vector<char> v;
    		for (char i = 1; i <= 7; ++i) {
        		v.push_back(i + '0');
    		}
    		min_path=find_kth_permutation(v, i,0,0,min_path,rank);
    	}
    	end=MPI_Wtime();
    	MPI_Reduce(&min_path, &global_min, 1, MPI_INT, MPI_MIN, 0, MPI_COMM_WORLD);
    if(rank==0){
	    cout<<global_min<<endl;
	    cout<<(end-start)*1000000<<endl;
	    }
    MPI_Finalize();
}
