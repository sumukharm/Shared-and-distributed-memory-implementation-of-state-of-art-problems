//R-WAY FW-APSP : Author Sumukha R M
#include<stdio.h>
#define MAX 1000

int graph[MAX][MAX];
int path[MAX][MAX];
int n;

void FW_APSP(int start_row,int end_row,int start_col,int end_col)
{
	int i,j;

	int k;
	int start=start_row<start_col?start_row:start_col,end=end_col>end_row?end_col:end_row;

	for(k=start; k<=end; k++)		//execute in serial, case graph 0-3, 0-2, 2-3, sync
	{
		for(i=start_row; i<=end_row; i++)
		{
			for(j=start_col; j<=end_col; j++)
				path[i][j] = (path[i][j]<path[i][k]+path[k][j] ? path[i][j] : path[i][k]+path[k][j]); 
		}
	}
}

void r-way(int start_row, int end_row,int start_col, int end_col, int r)
{
	if((end_row-start_row+1<=r) && (end_col-start_col+1<=r))
	{
		FW_APSP(start_row,end_row,start_col, end_col);		
		int i,j;
		cilk spawn
			for(i=end_col+1; i<n; i=i+r)
				FW_APSP(start_row,end_row,i,i+r-1,r);

		cilk spawn
			for(j=end_row+1; j<n; j=j+r)
				FW_APSP(j,j+r-1,start_col,end_col,r);
		cilk sync;
	}
	
	else
	{
		int val=(end_col-start_col+1)/r;	//no padding as n and r are powers of 2, r always divides diff, except in base case
		r-way(start_row,start_row+val-1,start_col,start_col+val-1,r);
		int k;
		for(k=1; ((start_row+(k+1)*val<=n) && (start_col+(k+1)*val<=n)); k++)
		{ 	
			FW_APSP(start_row+k*val,start_row+(k+1)*val-1,start_col+k*val,start_col+(k+1)*val-1,r);
			int i,j;		
			cilk spawn
				for(i=start_col+(k+1)*val; i<n; i=i+val)
					FW_APSP(start_row+k*val,start_row+(k+1)*val-1,i,i+val-1,r);
			cilk spawn
				for(j=start_row+(k+1)*val; j<n; j=j+val)
					FW_APSP(j,j+val-1,start_col+k*val,start_col+(k+1)*val-1,r);
			cilk sync;
		}
	}
}

int main(int argc, char *argv[])
{
	printf("Nunber of vertices : \n");
	scanf("%d",&n);
	printf("enter the weights : \n");
	int i,j;
	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
			scanf("%d",&graph[i][j]);
	}	

	for(i=0; i<n; i++)
		for(j=0; j<n; j++)
			path[i][j]=graph[i][j];			

	printf("enter r\n");
	int r;
	scanf("%d",&r);
	r-way(0,n-1,0,n-1,r);
	return 0;	
}
