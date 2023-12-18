#include <bits/stdc++.h>
#include<mpi.h>
using namespace std;
const int N = 8;
int final_path[N+1];
bool visited[N];
int adj[100][50] = {{ 0, 29, 82, 46, 68, 52, 72, 42}, { 29,0 , 55, 46, 42, 43,43, 23}, { 82, 55, 0, 68, 46, 55,23,43}, { 46,46,68,0,82,15,72,31},{68,42,46,82,0,74,23,52}, {52, 43, 55,15,74,0,61,23},{72,43,23,72,23,61,0,42},{42,23,43,31,52,23,42,0}};
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
  
int TSPRec(int curr_bound, int curr_weight, int level, int curr_path[],int local_res)
{
    if (level==N)
    {
        if (adj[curr_path[level-1]][curr_path[0]] != 0)
        {
            int curr_res = curr_weight + adj[curr_path[level-1]][curr_path[0]];
  
            if (curr_res < local_res)
            {
                copyToFinal(curr_path);	
                local_res = curr_res;
            }
        }
        //cout<<local_res<<" ";
        return local_res;
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
  
            if (curr_bound + curr_weight < local_res)
            {
                curr_path[level] = i;
                visited[i] = true;
                local_res=TSPRec(curr_bound, curr_weight, level+1,curr_path,local_res);
                //cout<<local_res<<" ";
            }
            curr_weight -= adj[curr_path[level-1]][i];
            curr_bound = temp;
            memset(visited, false, sizeof(visited));
            for (int j=0; j<=level-1; j++)
                visited[curr_path[j]] = true;
        }
    }
    return local_res;
}
int TSP(int i)
{
	int x;
	int curr_path[N+1];
    int curr_bound = 0;
    memset(curr_path, -1, sizeof(curr_path));
    for(int j=0;j<N;j++)
    		visited[j]=false;
    for (int j=0; j<N; j++)
        curr_bound += (firstMin(i) + secondMin(i));
    if(curr_bound%2==0)
    	curr_bound=(curr_bound/2) +1;
    else
    	curr_bound=curr_bound/2;

    visited[0] = true;
    curr_path[0] = 0;
    int curr_weight=0;
        if(adj[curr_path[0]][i] != 0 && visited[i] == false)
            {
            int temp = curr_bound;
            curr_weight += adj[curr_path[0]][i];
              curr_bound -= ((firstMin(curr_path[0]) + firstMin(i))/2);
  	    int flag=0;
		 curr_path[1] = i;
                visited[i] = true;
                for(int j=0;j<N;j++)
    			visited[j]=0;

                x=TSPRec(curr_bound, curr_weight,2,curr_path,INT_MAX);
                //cout<<i<<" "<<x<<endl;
            curr_weight -= adj[curr_path[0]][i];
            curr_bound = temp;
            for(int j=0;j<N;j++)
    		visited[j]=false;
            visited[curr_path[0]] = true;
       }
       //cout<<"l"<<i<<x<<endl;
       return x;
}
int main(int argc,char* argv[])
{
	int size,rank,final_res,local_res=INT_MAX,l;
	double start,end;
	MPI_Init(&argc, &argv);
 	MPI_Comm_size(MPI_COMM_WORLD, &size);
 	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 	start=MPI_Wtime();
 	for(int i=1;i<N;i++){
 		int j=i%size;
 		if(rank==j){
 			l=TSP(i);
 			if(l<local_res) 
 				local_res=l;
 		}
    	}
    	MPI_Barrier(MPI_COMM_WORLD);
       MPI_Reduce(&local_res,&final_res,1,MPI_INT,MPI_MIN,0,MPI_COMM_WORLD);
       end=MPI_Wtime();
       if(rank==0){
       	cout<<final_res<<endl;
       	cout<<end-start<<endl;
       }
       MPI_Finalize();
    return 0;
}