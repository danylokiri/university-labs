#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(void)
{
	int *A;
	int i, sum=0, n;
	float avsum=0;
	
	printf("enter size of arr: ");
	scanf("%d", &n);
	
	A = (int*)malloc(n * sizeof(int));		//перший і другий пункти
	srand(time(NULL));
	for(i = 0; i<n; i++)
	{
		A[i] = rand()%30;
		printf("%d ", A[i]);
		sum+=A[i];
	}	
	   
	printf("sum = %d", sum);
	avsum = sum/n;
	printf("avarage sum = %f", avsum);
	free (A);
	return 0;
}
