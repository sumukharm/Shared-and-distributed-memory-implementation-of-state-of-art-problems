#include<stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <pthread.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <math.h>
#define min(a,b) (a<b)?(a):(b)
#define A_loop_fw func
#define B_loop_fw func
#define C_loop_fw func
#define D_loop_fw func
#define PARAMS int x[][100], int start_rowx, int end_rowx, int start_colx, int end_colx, int start_rowu, int end_rowu, int start_colu, int end_colu, int start_rowv, int end_rowv, int start_colv, int end_colv, int n, int r


void func(PARAMS)
{
	int i,j,k;
	for(k=start_rowx; k<=end_rowx; k++)
		for(i=start_rowx; i<=end_rowx; i++)
			for(j=start_colx; j<=end_colx; j++)
				x[i][j]=min(x[i][j],x[i][k]+x[k][j]);
}


void D_fw(PARAMS)
{
	int m=end_rowx-start_rowx+1;
	if(r>m)
		D_loop_fw(x,start_rowx,end_rowx,start_colx,end_colx,start_rowu,end_rowu,start_colu,end_colu,start_rowv,end_rowv, start_colv, end_colv,n,r);

	else
	{
		int i,j,k;
		int offset=(end_colx-start_colx+1)/r;
		for(k=1; k<=r; k++)
		{
			cilk_for(i=1; i<=r; i++)
			{
				cilk_for(j=1; j<=r; j++)
				{
				D_fw(x, start_rowx+(i-1)*offset, start_rowx+i*offset-1, start_colx+(j-1)*offset, start_colx+j*offset-1,
				start_rowu+(i-1)*offset, start_rowu+i*offset-1, start_colu+(k-1)*offset, start_colu+k*offset-1,
				start_rowv+(k-1)*offset,start_rowv+k*offset-1 , start_colv+(j-1)*offset,start_colv+j*offset-1,n,r);	
				}			
			}
		}
	}
}

void B_fw(PARAMS)
{
	int m=end_rowx-start_rowx+1;
	if(r>m)
		B_loop_fw(x,start_rowx,end_rowx,start_colx,end_colx,start_rowu,end_rowu,start_colu,end_colu,start_rowv,end_rowv, start_colv, end_colv,n,r);

	else
	{
		int i,j,k;
		int offset=(end_colx-start_colx+1)/r;
		for(k=1; k<=r; k++)
		{
			cilk_for(j=1; j<=r; j++)
			{
				B_fw(x, start_rowx+(k-1)*offset,start_rowx+k*offset-1,start_colx+(j-1)*offset,start_colx+j*offset-1,
				start_rowu+(k-1)*offset,start_rowu+k*offset-1 ,start_colu+(k-1)*offset, start_colu+k*offset-1,
				start_rowv+(k-1)*offset,start_rowv+k*offset-1 , start_colv+(j-1)*offset,start_colv+j*offset-1,n,r);
			}

			cilk_for(i=1; i<=r; i++)
			{
				cilk_for(j=1; j<=r; j++)
				{
					if(i!=k)
					{
				D_fw(x,start_rowx+(i-1)*offset, start_rowx+i*offset-1, start_colx+(j-1)*offset, start_colx+j*offset-1,
					start_rowu+(i-1)*offset, start_rowu+i*offset-1, start_colu+(k-1)*offset, start_colu+k*offset-1,
					start_rowv+(k-1)*offset,start_rowv+k*offset-1 , start_colv+(j-1)*offset,start_colv+j*offset-1,n,r);
					}
				}
			}
			
		}
	}
}

void C_fw(PARAMS)
{
	int m=end_rowx-start_rowx+1;
	if(r>m)
		C_loop_fw(x,start_rowx,end_rowx,start_colx,end_colx,start_rowu,end_rowu,start_colu,end_colu,start_rowv,end_rowv, start_colv, end_colv,n,r);

	else
	{
		int k;
		int offset=(end_colx-start_colx+1)/r;
		for(k=1; k<=r; k++)
		{
			int i,j;
			cilk_for(i=1; i<=r; i++)
			{
				C_fw(x,start_rowx+(i-1)*offset, start_rowx+i*offset-1, start_colx+(k-1)*offset, start_colx+k*offset-1,
				start_rowu+(i-1)*offset, start_rowu+i*offset-1, start_colu+(k-1)*offset, start_colu+k*offset-1,
				start_rowv+(k-1)*offset,start_rowv+k*offset-1 ,start_colv+(k-1)*offset, start_colv+k*offset-1, n,r);
			}
			cilk_for(i=1; i<=r; i++)
			{
				cilk_for(j=1; j<=r; j++)
				{
				if(j!=k)
				{
				D_fw(x, start_rowx+(i-1)*offset, start_rowx+i*offset-1, start_colx+(j-1)*offset, start_colx+j*offset-1,
				start_rowu+(i-1)*offset, start_rowu+i*offset-1, start_colu+(k-1)*offset, start_colu+k*offset-1,
				start_rowv+(k-1)*offset,start_rowv+k*offset-1 , start_colv+(j-1)*offset,start_colv+j*offset-1,n,r);
				}
				}
			}
		}
	}
}

void A_fw(PARAMS)
{
	int m=end_rowx-start_rowx+1;
	if(r>m)
		A_loop_fw(x,start_rowx,end_rowx,start_colx,end_colx,start_rowu,end_rowu,start_colu,end_colu,start_rowv,end_rowv, start_colv, end_colv,n,r);

	else
	{
		int k;
		int offset=(end_colx-start_colx+1)/r;
		for(k=1; k<=r; k++)
		{
			A_fw(x, start_rowx+(k-1)*offset,start_rowx+k*offset-1 ,start_colx+(k-1)*offset, start_colx+k*offset-1, 
			start_rowu+(k-1)*offset,start_rowu+k*offset-1 ,start_colu+(k-1)*offset, start_colu+k*offset-1, 
			start_rowv+(k-1)*offset,start_rowv+k*offset-1 ,start_colv+(k-1)*offset, start_colv+k*offset-1, n,r);
		
			int i,j;
			
				
			for(i=1, j=1; i<=r && j<=r; i++,j++)
                        {
                                if(j!=k)
                                {
                                cilk_spawn B_fw(x, start_rowx+(k-1)*offset,start_rowx+k*offset-1,start_colx+(j-1)*offset,start_colx+j*offset-1,
                                start_rowu+(k-1)*offset,start_rowu+k*offset-1 ,start_colu+(k-1)*offset, start_colu+k*offset-1,
                                start_rowv+(k-1)*offset,start_rowv+k*offset-1 , start_colv+(j-1)*offset,start_colv+j*offset-1,n,r);
                                }
                     
                                if(i!=k)
                                {
                                cilk_spawn C_fw(x, start_rowx+(i-1)*offset, start_rowx+i*offset-1, start_colx+(k-1)*offset, start_colx+k*offset-1,
                                start_rowu+(i-1)*offset, start_rowu+i*offset-1, start_colu+(k-1)*offset, start_colu+k*offset-1,
                                start_rowv+(k-1)*offset,start_rowv+k*offset-1 ,start_colv+(k-1)*offset, start_colv+k*offset-1, n,r);
                                }

				cilk_sync;
                        }
                        
                       	
			cilk_for(i=1; i<=r; i++)
			{
				cilk_for(j=1; j<=r; j++)
				{
				if((i!=k) && (j!=k))
				{
				D_fw(x, start_rowx+(i-1)*offset, start_rowx+i*offset-1, start_colx+(j-1)*offset, start_colx+j*offset-1,
				start_rowu+(i-1)*offset, start_rowu+i*offset-1, start_colu+(k-1)*offset, start_colu+k*offset-1,
				start_rowv+(k-1)*offset,start_rowv+k*offset-1 , start_colv+(j-1)*offset,start_colv+j*offset-1,n,r);
				}				
				}
			}
		}
	}
}

int main()
{ 
	int graph[100][100];
	int n,r;
	printf("r-way fwapsp\n");
	scanf("%d %d",&n,&r);
	int i,j;
	for(i=0; i<n; i++)
		for(j=0; j<n; j++)
			scanf("%d",&graph[i][j]);

	int x[100][100];
	for(i=0; i<n; i++)
		for(j=0; j<n; j++)
			x[i][j]=graph[i][j];

	 A_fw(x,0,n-1, 0,n-1, 0,n-1, 0,n-1, 0,n-1, 0,n-1,n,r);

	printf("\n\n");
	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
			printf("%d ",x[i][j]);
		printf("\n");
	}

	return 0;
}
