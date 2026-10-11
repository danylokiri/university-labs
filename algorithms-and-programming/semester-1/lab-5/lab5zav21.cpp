#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(void)
{	
		int *A;
		int menu, n, randnum, en, du;

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


return 0;
}
