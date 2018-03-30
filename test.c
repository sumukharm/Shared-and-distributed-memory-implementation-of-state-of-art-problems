#include<stdio.h>
#define param1 2,3,4, 186
#define param2 int a, int b, int c , int d
#define param3 a,3,2
#define param4 int arr[10], int n, int fac
#include<stdlib.h>

int add(param2)
{
	return a+b+c+d;
}

int add1(param4)
{
	int i;
	int sum=0;
	for(i=0; i<n; i++)
		sum+=(arr[i]+fac);
	return sum;
}

void print_block(int *x, int start_row, int start_col,int n,int N)
{
	int i,j;
	for(i=0; i<n; i++)
	{
		for(j=0; j<n; j++)
			printf("%d ",x[(i+start_row)*N + start_col+j]);

		//printf("%d ",x[i]);
		printf("\n");
	}
}

int main()
{
	printf("%d\n",add(param1));
	int a[3]={10,20,30};
	printf("%d\n",add1(param3));

	int n;
	scanf("%d",&n);
	int *x=malloc(sizeof(int)*n*n);
	int i;
	for(i=0; i<n*n; i++)
		x[i]=i;
	for(i=0; i<n*n; i++)
		printf("%d ",x[i]);

	printf("\n");
	int start_row,start_col;
	scanf("%d %d",&start_row,&start_col);

	print_block(x,start_row,start_col,n/2,n);

	return 0;
}
