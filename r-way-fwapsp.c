#include<stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <pthread.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <math.h>
#include <immintrin.h>
#define MAX 1000

int adj[MAX][MAX];
int path1[MAX][MAX],path2[MAX][MAX];
int offset;

void serial_fw(int flag,int start_row,int end_row,int start_col,int end_col)
{
	int i,j,k;
	for(k=start_row; k<=end_row; k++)
	{
		for(i=start_row; i<=end_row; i++)
		{
			for(j=start_col; j<=end_col; j++)
			{
				if(flag==1)
					path1[i][j]= (path1[i][j]<(path1[i][k]+path1[k][j]) ? path1[i][j]: (path1[i][k]+path1[k][j]));
				else if(flag==2)
					path2[i][j]= (path2[i][j]<(path2[i][k]+path2[k][j]) ? path2[i][j]: (path2[i][k]+path2[k][j]));
			}
		}
	}
}

void A_fw(int start_row,int end_row,int start_col, int end_col)
{
	int i,j,k;
	serial_fw(2,start_row,end_row,start_col,end_col);
}

void B_fw(int start_row,int end_row,int start_col,int end_col)
{
	int i,j,k;
	for(k=start_col; k<=end_col; k++)
	{
		if(k>end_row)
			break;
		else
		{
			for(i=start_row; i<=end_row; i++)
			{
				for(j=start_col; j<=end_col; j++)
					path2[i][j]=(path2[i][j]<(path2[i][k]+path2[k][j])? path2[i][j]:(path2[i][k]+path2[k][j]));
			}
		}
	}
}

void C_fw(int start_row,int end_row, int start_col,int end_col)
{
	int i,j,k;
	for(k=start_row; k<=end_row; k++)
	{
		if(k>end_col)
			break;
		else
		{
			for(i=start_row; i<=end_row; i++)
			{
				for(j=start_col; j<=end_col; j++)
					path2[i][j]=(path2[i][j]<(path2[i][k]+path2[k][j])? path2[i][j]:(path2[i][k]+path2[k][j]));
			}
		}
	}
}

void D_fw(int start_row, int end_row, int start_col,int end_col)
{
	int i,j,k;
	for(k=0; k<start_row; k++)
	{
		for(i=start_row; i<=end_row; i++)
		{	
			for(j=start_col; j<=end_col; j++)
				path2[i][j]=(path2[i][j]<(path2[i][k]+path2[k][j])? path2[i][j]:(path2[i][k]+path2[k][j]));
		}
	}
}

void A_bw(int start_row,int end_row,int start_col, int end_col)
{
	int i,j,k;
	for(k=start_row; k<=end_row; k++)
	{
		for(i=start_row; i<=end_row; i++)
		{
			for(j=start_col; j<=end_col; j++)
				path2[i][j]=(path2[i][j]<(path2[i][k]+path2[k][j])? path2[i][j]:(path2[i][k]+path2[k][j]));
		}
	}
}

void B_bw(int start_row, int end_row, int start_col, int end_col)
{
	int i,j,k;
	for(k=start_row; k<=end_row; k++)
	{
		for(i=start_row; i<=end_row; i++)
		{
			for(j=start_col; j<=end_col; j++)
				path2[i][j]=(path2[i][j]<(path2[i][k]+path2[k][j])? path2[i][j]:(path2[i][k]+path2[k][j]));
		}
	}
}

void C_bw(int start_row, int end_row, int start_col, int end_col)
{
	int i,j,k;
	for(k=start_col; k<=end_col; k++)
	{
		for(i=start_row; i<=end_row; i++)
		{
			for(j=start_col; j<=end_col; j++)
				path2[i][j]=(path2[i][j]<(path2[i][k]+path2[k][j])? path2[i][j]:(path2[i][k]+path2[k][j]));
		}
	}
}

void D_bw(int start_row, int end_row, int start_col, int end_col)
{
	int i,j,k;
	for(k=end_col+1; k<=end_col+offset; k++)
	{
		for(i=start_row; i<=end_row; i++)
		{
			for(j=start_col; j<=end_col; j++)
				path2[i][j]=(path2[i][j]<(path2[i][k]+path2[k][j])? path2[i][j]:(path2[i][k]+path2[k][j]));
		}	
	}

	for(k=start_col; k<=end_col; k++)
	{
		for(i=end_row+1; i<=end_row+offset; i++)
		{
			for(j=end_col+1; j<=end_col+offset; j++)
				path2[i][j]=(path2[i][j]<(path2[i][k]+path2[k][j])? path2[i][j]:(path2[i][k]+path2[k][j]));	
		}
	}
}

void parallel_fw(int n,int start_row,int start_col)
{
	offset=2;
	while(start_row+2*offset<=n)
	{
		A_fw(start_row,start_row+offset-1,start_col,start_col+offset-1);
		cilk_spawn B_fw(start_row,start_row+offset-1,start_col,start_col+2*offset-1);
		cilk_spawn C_fw(start_row,start_row+2*offset-1,start_col,start_col+offset-1);
		cilk_sync;
		D_fw(start_row+offset,start_row+2*offset-1,start_col+offset,start_col+2*offset-1);
			
		A_bw(start_row+offset,start_row+2*offset-1,start_col+offset,start_col+2*offset-1);
		cilk_spawn B_bw(start_row+offset,start_row+2*offset-1,start_col,start_col+2*offset-1);
		cilk_spawn C_bw(start_row,start_row+2*offset-1,start_col+offset,start_col+2*offset-1);
		cilk_sync;
		D_bw(start_row,start_row+offset-1,start_col,start_col+offset-1);

		offset=2*offset;
	}
}

void print_graph(int n, int flag)
{
	int i,j;
	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
		{
			if(flag==1)
				printf("%d ",path1[i][j]);
			else if(flag==2)
				printf("%d ",path2[i][j]);
		}
		printf("\n");
	}
}

void compare(int n)
{
	int i,j;
	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
		{
			if(path1[i][j]!=path2[i][j])
			{
				printf("\n\nThey don't match\n\n");
				return;
			}
		}
	}
	printf("\n\nThey match\n\n");
}

int main()
{
	int n,r;
	printf("enter n and r\n");
	scanf("%d %d",&n,&r);
	printf("enter the graph\n");
	int i,j;
	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
			scanf("%d",&adj[i][j]);
	}

	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
		{
			path1[i][j]=adj[i][j];
			path2[i][j]=adj[i][j];
		}
	}
	

	serial_fw(1,0,n-1,0,n-1);
	clock_t before=clock();
	parallel_fw(n,0,0);
	clock_t difference=clock()-before;
	printf("\n\nSerial algorithms answer\n");
	print_graph(n,1);
	printf("\n\n");
	printf("parallel r-way answer\n");
	print_graph(n,2);
	compare(n);
	printf("time taken by parallel execution : %f\n", (float)((difference*1000)/CLOCKS_PER_SEC));
	return 0;
}

