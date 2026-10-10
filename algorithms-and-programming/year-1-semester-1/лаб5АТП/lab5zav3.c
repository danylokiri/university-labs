#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(void)
{	
int *A;
	int menu, n, randnum, en, du;
	
		printf("Zavdannya 3\n");
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
		
		for(int k = 0; k<n; k++)
		{
			A[k] = rand()%randnum;
			printf("%d ", A[k]);
		}
		printf("\n\nzavdannya:\n"); 
		for(int i = 0; i<n; i++)
		{
			du=A[i] % 10; 
			A[i]=A[i]/10;
			en=A[i] % 10;
			A[i]=A[i]/10;
			if(A[i]/10<=0)
			{
				A[i]= du + en;
				printf("%d ", A[i]);
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
		for (int k = 0; k<n; k++)
  		{
   		printf("A[%d] = ", k);
 		scanf("%d", &A[k]);
 		}
		printf("\n\nzavdannya:\n\n");
		for(int i = 0; i<n; i++)
		{
			du=A[i] % 10; 
			A[i]=A[i]/10;
			en=A[i] % 10;
			A[i]=A[i]/10;
			if(A[i]/10<=0)
			{
				A[i]= du + en;
				printf("%d ", A[i]);
			}
		}	
		printf("\n\n");
		free(A);
	}
	if(menu==3) break;		
	if(menu!=1 && menu!=2 && menu!=3) printf("\nerror(choose option from meny)\n\n");
}
}

