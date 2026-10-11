#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(void)
{	
	int *A;
	int menu, pos, i, c, n=0;
	void add_arr (int *A, int value, int pos, int i);
	void remove_arr (int *A, int pos, int i);
	printf("The list of inveted\n");
	while(1){		
		printf("choose option: \n");
		printf("1 - Show the list\n");
		printf("2 - Add\n");
		printf("3 - Remove\n");
		printf("4 - Exit\n\n");
		scanf("%d", &menu);
		printf("\n");
		
		if(menu==1)
		{
			for(int i = 0; i<n; i++)
			{
				printf("%d. %d\n", i+1, A[i]);
			}
			printf("\n");
		}
		if(menu==2)
		{
			printf("enter a number: \n");
			scanf("%d", &c);
			printf("enter a position: \n");
			scanf("%d", &pos);
			n++;
			if(pos<=n){
				add_arr(*A, value, pos, i);
			}
			
			else
			{
				printf("\nplease write correct value\n");
			}		
		}
		if(menu==3)
		{
			printf("enter a position: \n");
			scanf("%d", &pos);

			remove_arr(*A, pos, i);	
			n--;
		}
		if(menu==4) break;		
		if(menu!=1 && menu!=2 && menu!=3 && menu!=4) printf("\nerror(choose option from meny)\n\n");
	}
	
}

void add_arr(int *A, int value, int pos, int i)
{
	int *A = malloc((n+1)*sizeof(int));
	int value;
	for (i = n-1; i > pos; i--)
    *(A+i) = *(A+i-1); 
	*(A+pos-1) = value;
}

void remove_arr(int *A, int pos, int i)
{
	int *A = malloc(n*sizeof(int));
	int pos, i;
	for (i = pos; i < n-1; i++)
    *(A+i) = *(arr+i+1); 
	*(A+n-1) = 0;
}

