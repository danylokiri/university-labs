#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(void)
{	
	int *A;
	int menu, i, n, a, k, randnum;

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
		if (i % 2 != 0)
  		{
 	 		printf("%d,  ", A[i]);
		}
	}
	for(i = n; i>0; i--)
	{
		if (i % 2 != 0)
  		{
 	 		printf("%d, ", A[i-1]);
		}
	}

	free(A);
	return 0;
}
