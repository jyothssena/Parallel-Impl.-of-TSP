#include <bits/stdc++.h>
#include<omp.h>
using namespace std;
const int N = 8;
int final_path[N+1];
bool visited[N];
int final_res = INT_MAX;
int adj[100][50];
void copyToFinal(int curr_path[])
{
    for (int i=0; i<N; i++)
        final_path[i] = curr_path[i];
    final_path[N] = curr_path[0];
}
int firstMin(int i)
{
    int min = INT_MAX;
    for (int k=0; k<N; k++)
        if (adj[i][k]<min && i != k)
            min = adj[i][k];
    return min;
}
  
int secondMin(int i)
{
    int first = INT_MAX, second = INT_MAX;
    for (int j=0; j<N; j++)
    {
        if (i == j)
            continue;
  
        if (adj[i][j] <= first)
        {
            second = first;
            first = adj[i][j];
        }
        else if (adj[i][j] <= second &&
                 adj[i][j] != first)
            second = adj[i][j];
    }
    return second;
}
  
void TSPRec(int curr_bound, int curr_weight, int level, int curr_path[])
{
    if (level==N)
    {
        if (adj[curr_path[level-1]][curr_path[0]] != 0)
        {
            int curr_res = curr_weight + adj[curr_path[level-1]][curr_path[0]];
            #pragma omp critical
  	    {
            if (curr_res < final_res)
            {
                copyToFinal(curr_path);	
                final_res = curr_res;
                cout<<final_res<<" ";
            }
            }
        }
        return;
    }
    for (int i=0; i<N; i++)
    {
        if (adj[curr_path[level-1]][i] != 0 &&
            visited[i] == false)
        {
            int temp = curr_bound;
            curr_weight += adj[curr_path[level-1]][i];

            if (level==1)
              curr_bound -= ((firstMin(curr_path[level-1]) +
                             firstMin(i))/2);
            else
              curr_bound -= ((secondMin(curr_path[level-1]) +
                             firstMin(i))/2);
  	    int flag=0;
            #pragma omp critical
            {
            	if(curr_bound + curr_weight < final_res)
            		flag=1;
            }
            if (flag==1)
            {
                curr_path[level] = i;
                visited[i] = true;
                TSPRec(curr_bound, curr_weight, level+1,curr_path);
            }
            curr_weight -= adj[curr_path[level-1]][i];
            curr_bound = temp;
            memset(visited, false, sizeof(visited));
            for (int j=0; j<=level-1; j++)
                visited[curr_path[j]] = true;
        }
    }
}
void TSP()
{
    int curr_path[N+1];
    int curr_bound = 0;
    memset(curr_path, -1, sizeof(curr_path));
    for(int j=0;j<N;j++)
    		visited[j]=false;
    for (int i=0; i<N; i++)
        curr_bound += (firstMin(i) + secondMin(i));
    if(curr_bound%2==0)
    	curr_bound=(curr_bound/2) +1;
    else
    	curr_bound=curr_bound/2;

    visited[0] = true;
    curr_path[0] = 0;
    int curr_weight=0;
    omp_set_num_threads(N);
    #pragma omp parallel for private(visited,curr_weight,curr_bound,curr_path)
    for (int i=0; i<N; i++)
    {
        if(adj[curr_path[0]][i] != 0 && visited[i] == false)
            {
            int temp = curr_bound;
            curr_weight += adj[curr_path[0]][i];
              curr_bound -= ((firstMin(curr_path[0]) + firstMin(i))/2);
  	    int flag=0;
            #pragma omp critical
            {
            	if(curr_bound + curr_weight < final_res)
            		flag=1;
            }
            if (flag==1)
            {
                curr_path[1] = i;
                visited[i] = true;
                for(int j=0;j<N;j++)
    			visited[j]=0;

                TSPRec(curr_bound, curr_weight,2,curr_path);
            }
            curr_weight -= adj[curr_path[0]][i];
            curr_bound = temp;
            for(int j=0;j<N;j++)
    		visited[j]=false;
            visited[curr_path[0]] = true;
       }
       }
}
int main()
{
    for(int i=0;i<N;i++){
    	for(int j=0;j<N;j++)
    		cin>>adj[i][j];
    }
    printf("Path Taken : \n");  
    TSP();
    for(int i=0;i<N;i++)
	printf("%d ",final_path[i]);
    printf("\nMinimum cost : %d\n", final_res);
    return 0;
}