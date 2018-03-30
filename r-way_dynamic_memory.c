#include<stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <pthread.h>
#include <cilk/cilk.h>
#include <cilk/cilk_api.h>
#include <math.h>
#define min(a,b) (a<b)?(a):(b)
#include<stdlib.h>
#define A_loop_fw func
#define B_loop_fw func
#define C_loop_fw func
#define D_loop_fw func

void serial(int *x, int n)
{
	int i,j,k;
	for(k=0; k<n; k++)
		for(i=0; i<n; i++)
			for(j=0; j<n; j++)
				x[i*n + j]=min(x[i*n+j],x[i*n+k]+x[k*n+j]);
}

void print(int *x, int n, int N)
{
	int i,j;
	printf("\n");
	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
			printf("%d ",x[i*n+j]);

		printf("\n");
	}
}

void func(int *x, int x_start_row, int x_start_col, int *u, int u_start_row, int u_start_col, int *v, int v_start_row, int v_start_col, int n,
        int N)
{
        int i,j,k;
        for(i=0; i<n; i++)
        {
                for(j=0; j<n; j++)
                {
                        for(k=0; k<n; k++)
                        {
                                int val= u[(u_start_row+i)*N+(u_start_col+k)] + v[(v_start_row+k)*N+(v_start_col+j)];               
				x[(x_start_row+i)*N+(x_start_col+j)]=min(x[(x_start_row+i)*N+(x_start_col+j)],val);
			}
		}
	}
}
		
void D_fw(int *x, int x_start_row, int x_start_col, int *u, int u_start_row, int u_start_col, int *v, int v_start_row, int v_start_col, int n,  int r, int N)
{
	if(r>n)
		D_loop_fw(x,x_start_row, x_start_col, u, u_start_row, u_start_col, v, v_start_row, v_start_col, n,N);

	else
	{
		int i,j,k, offset=n/r;
		for(k=1; k<=r; k++)
		{
			cilk_for(i=1; i<=r; i++)
			{
				cilk_for(j=1; j<=r; j++)
					D_fw(x, x_start_row+(i-1)*offset, x_start_col+(j-1)*offset, 
						u, u_start_row+(i-1)*offset, u_start_col+(k-1)*offset,
						v, v_start_row+(k-1)*offset, v_start_col+(j-1)*offset, n/r,r,N);
			}	
		}
	}
}

void C_fw(int *x, int x_start_row, int x_start_col, int *u, int u_start_row, int u_start_col, int *v, int v_start_row, int v_start_col, int n, int r, int N)
{
	if(r>n)
		C_loop_fw(x, x_start_row, x_start_col, u, u_start_row, u_start_col, v, v_start_row, v_start_col, n,N);	

	else
	{
		int k, offset=n/r;
		for(k=1; k<=r; k++)
		{
			int i,j;
			cilk_for(i=1; i<=r; i++)
				C_fw(x, x_start_row+(i-1)*offset, x_start_col+(k-1)*offset, u, u_start_row+(i-1)*offset, 
					u_start_col+(k-1)*offset, v, v_start_row+(k-1)*offset, v_start_col+(k-1)*offset, n/r, r,N);

			cilk_for(i=1; i<=r; i++)
			{
				cilk_for(j=1; j<=r; j++)
				{
					if(j!=k)
						D_fw(x,x_start_row+(i-1)*offset, x_start_col+(j-1)*offset, 
							u, u_start_row+(i-1)*offset, u_start_col+(k-1)*offset,
							v, v_start_row+(k-1)*offset, v_start_col+(j-1)*offset, n/r,r,N);
				}
			}
	
		}
	}
}


void B_fw(int *x, int x_start_row, int x_start_col, int *u, int u_start_row, int u_start_col, int *v, int v_start_row, int v_start_col, int n, int r, int N)
{
	if(r>n)
		B_loop_fw(x, x_start_row, x_start_col, u, u_start_row, u_start_col, v, v_start_row, v_start_col, n,N);

	else
	{
		int i,j,k;
		int offset=n/r;
		for(k=1; k<=r; k++)
		{
			cilk_for(j=1; j<=r; j++)
				B_fw(x, x_start_row+(k-1)*offset, x_start_col+(j-1)*offset, u, u_start_row+(k-1)*offset,
					u_start_col+(k-1)*offset, v, v_start_row+(k-1)*offset, v_start_col+(j-1)*offset, n/r, r,N);

			cilk_for(i=1; i<=r; i++)
			{
				cilk_for(j=1; j<=r; j++)
				{
					if(i!=k)
						D_fw(x, x_start_row+(i-1)*offset, x_start_col+(j-1)*offset, 
							u, u_start_row+(i-1)*offset, u_start_col+(k-1)*offset,
							v, v_start_row+(k-1)*offset, v_start_col+(j-1)*offset, n/r,r,N);
				}
			} 	
		}
	}	
}

void A_fw(int *x, int x_start_row, int x_start_col, int *u, int u_start_row, int u_start_col, int *v, int v_start_row, int v_start_col, int n, int r,int N)
{
	if(r>n)
                A_loop_fw(x, x_start_row, x_start_col, u, u_start_row, u_start_col, v, v_start_row, v_start_col, n,N);
        else
        {
                int offset=n/r;
                int i,j,k;
                cilk_for(k=1; k<=r; k++)
                {
                        A_fw(x,x_start_row+(k-1)*offset, x_start_col+(k-1)*offset, u, u_start_row+(k-1)*offset, u_start_col+(k-1)*offset, v,
                                v_start_row+(k-1)*offset, v_start_col+(k-1)*offset, n/r, r, N);

                        
                        for(i=1,j=1; i<=r && j<=r; i++, j++)
                        {
                                if(j!=k)    
                                        cilk_spawn B_fw(x,x_start_row+(k-1)*offset, x_start_col+(j-1)*offset, u, u_start_row+(k-1)*offset,
                                                   u_start_col+(k-1)*offset, v, v_start_row+(k-1)*offset, v_start_col+(j-1)*offset, n/r,r,N);

                                if(i!=k)
                                        cilk_spawn C_fw(x, x_start_row+(i-1)*offset, x_start_col+(k-1)*offset, u, u_start_row+(i-1)*offset,
                                                   u_start_col+(k-1)*offset, v, v_start_row+(k-1)*offset, v_start_col+(k-1)*offset, n/r,r, N);

                        }
                        cilk_sync;
                }

                cilk_for(i=1; i<=r; i++)
                {
			cilk_for(j=1; j<=r; j++)
                        {
                                if((i!=k) &&(j!=k))
                                        cilk_spawn D_fw(x, x_start_row+(i-1)*offset, x_start_col+(j-1)*offset, u, u_start_row+(i-1)*offset,
							u_start_col+(k-1)*offset, v, v_start_row+(k-1)*offset, v_start_col+(j-1)*offset,n/r,r,N);			       }
		}
		cilk_sync;
	}
}

int main(int argc, char *argv[])
{
	int n,r;
	scanf("%d %d",&n,&r);
	int *x=malloc(sizeof(int)*n*n);
	int i;
	for(i=0; i<n*n; i++)
	{
		//x[i]=(random()%20)+1;
		scanf("%d",&x[i]);
	}
	
	A_fw(x,0,0,x,0,0,x,0,0,n,r,n);
	print(x,n,n);
	
	printf("\n");
	return 0;
}
