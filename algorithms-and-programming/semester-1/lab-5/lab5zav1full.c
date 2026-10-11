#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(void)
{	
int *A;
int menu, i, n, a, k, randnum;

printf("Zavdannya 1\n");
while(1){
	printf("choose option: \n");
	printf("1 - random values\n");
	printf("2 - your values\n");
	printf("3 - Exit\n");
	scanf("%d", &menu);
	
	if(menu==1)
	{
		printf("enter size of arr: ");
		scanf("%d", &n);
		printf("enter a max value: ");
		scanf("%d", &randnum);
		A = (int*)malloc(n * sizeof(int));
		srand(time(NULL));
		printf("\nrandom values:\n");
		for(i = 0; i<n; i++)
		{
			A[i] = rand()%randnum;
			printf("%d ", A[i]);
		}
		printf("\n\nzavdannya:\n");
		for(i = 0; i<n; i++)
		{
			if (i % 2 == 0)
  			{
 		 		printf("%d,  ", A[i]);
			}
		}
		for(i = n; i>0; i--)
		{
			if (i % 2 == 0)
  			{
 		 		printf("%d, ", A[i-1]);
			}
		}
		printf("\n\n");

	free(A);
	}
	if(menu==2)
	{
		printf("enter size of arr: ");
		scanf("%d", &n);
		A = (int*)malloc(n * sizeof(int));
		for (i = 0; i<n; i++)
  		{
   		printf("A[%d] = ", i);
 		scanf("%d", &A[i]);
 		}
 	
	printf("\n\nzavdannya:\n\n");

	for(i = 0; i<n; i++)
	{
		if (i % 2 == 0)
  		{
 	 		printf("%d,  ", A[i]);
		}
	}
	for(i = n; i>0; i--)
	{
		if (i % 2 == 0)
  		{
 	 		printf("%d, ", A[i-1]);
		}
	}
	printf("\n\n");
	free(A);	
	}
	if(menu==3)
	{
		break;
	}	
	if(menu!=1 && menu!=2 && menu!=3)
	{
		printf("\nerror(choose option from meny)\n\n");
	}
}
}

