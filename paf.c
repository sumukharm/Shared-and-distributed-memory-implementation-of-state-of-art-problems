#include<stdio.h>


void A_paf(int *x, int x_start_row, int x_start_col, int n, int r, int N, int base)
{
	if(n<=base)
		A_loop_paf(x, x_start_row, x_start_col, n, r, N, base);
	
	else
	{
		int i,j,l,offset=n/r;
		for(i=r; i>=1; i--)
		{
			A_paf(x, x_start_row+(i-1)*offset, x_start_col+(i-1)*offset, n/r, r, N, base);
			cilk_for(l=1; l<=i-1; l++)
			{
				B_paf(x, x_start_row+(l-1)*offset, x_start_col+(i-1)*offset,
					x, x_start_row+(i-1)*offset, x_start_col+(i-1)*offset, n/r, r, N, base);
			
				cilk_for(j=l; j>=1; j--)
				{
					if(j==l)
						C_paf(x, x_start_row+(j-1)*offset, x_start_col+(l-1)*offset,
							x, x_start_row+(l-1)*offset, x_start_col+(i-1)*offset, n/r, r, N, base);
					else
						D_paf(x, x_start_row+(j-1)*offset, x_start_col+(l-1)*offset,
							x, x_start_row+(l-1)*offset, x_start_col+(i-1)*offset, n/r, r, N, base);
				}

			}
		}
	}
}

void B_paf(int *x, int x_start_row, int x_start_col, int *v, int v_start_row, int v_start_col, int n, int r, int N, int base)
{
	if(n<=base)
		B_loop_paf(x, x_start_row, x_start_col, v, v_start_row, v_start_col, n,r,N, base);
	
	else
	{
		int i,k,l, offset=n/r;
		for(k=1; k<=r; k++)
		{
			cilk_for(i=1; i<=r-k+1; i++)
			{
				int j=i+k-1;
				cilk_for(l=1; l<=r; l++)
				{
					if(k==1)
						B_paf(x, x_start_row+(l-1)*offset, x_start_col+(j-k)*offset,
							v, v_start_row+(i-1)*offset, v_start_col+(j-1)*offset, n/r, r , N, base);
					else
						D_paf(x, x_start_row+(l-1)*offset, x_start_col+(j-k)*offset,
							v, v_start_row+(i-1)*offset, v_start_col+(j-1)*offset, n/r, r, N, base);
				}
			}
		}
	}
}

void C_paf(int *x, int x_start_row, int x_start_col, int *u, int u_start_row, int u_start_col, int n, int r, int N, int base)
{
	if(n<=base)
		C_loop_paf(x, x_start_row, x_start_col, u, u_start_row, u_start_col, n,r, N, base);
	
	else
	{
		int k,i,l, offset=n/r;
		for(k=1; k<=r; k++)
		{
			cilk_for(i=1; i<=r; i++)
			{
				int j=k;
				cilk_for(l=1; l<=i; l++)
				{
					if(l==i)
						C_paf(x, x_start_row+(l-1)*offset, x_start_col+(i-1)*offset,
							u, u_start_row+(i-1)*offset, u_start_col+(j-1)*offset, n/r, r, N, base);
					else
						D_paf(x, x_start_row+(l-1)*offset, x_start_col+(i-1)*offset, 
							u, u_start_row+(i-1)*offset, u_start_col+(j-1)*offset, n/r, r, N, base);
				}
			}
		}
	}	
}

void D_paf(int *x, int x_start_row, int x_start_col, int *u, int u_start_row, int u_start_col, int n, int r, int N, int base)
{
	if(n<=base)
		D_loop_paf(x, x_start_row, x_start_col, u, u_start_row, u_start_col, n,r, N, base);

	else
	{
		int i,j,k;
		for(k=1; k<=r; k++)
		{
			cilk_for(i=1; i<=r; i++)
			{
				cilk_for(j=1; j<=r; j++)
					D_paf(x, x_start_row+(i-1)*offset, x_start_col+(j-1)*offset,
						u, u_start_row+(j-1)*offset, u_start_col+(k-1)*offset, n/r, r, N, base);
			}
		}
	}
}
