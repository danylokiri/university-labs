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


return 0;
}
