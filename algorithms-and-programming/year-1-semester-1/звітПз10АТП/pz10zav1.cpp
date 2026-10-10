#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(void)
{
		int *A;  		// A - назва массиву
			int menu, menu1, menu12, menu13, menu2, menu21, n, i, k, max, maxi, a, bubble;
			int sum=0;
			float avsum=0;

			
	while(1){		
	printf("menu\n");
	printf("choose option: \n");
	printf("1 - Zavdannya 1\n");
	printf("2 - Zavdannya 2\n");
	printf("3 - Exit\n");
	scanf("%d", &menu);
		if(menu==1)
		{
			printf("enter size of arr: ");
			scanf("%d", &n);
			A = (int*)malloc(n * sizeof(int));
			srand(time(NULL));
			while(1)					//меню вибору спосібу створення елементів
			{
			printf("\nchoose option: \n");
			printf("1 - enter elements massive\n");
			printf("2 - random elements massive\n");
			printf("3 - Exit\n");
			scanf("%d", &menu1);
	
				if(menu1==2){							//рандом елементи
			printf("\nrandom values\n"); 
				for(i = 0; i<n; i++)
				{
					A[i] = rand()%30;
					printf("%d ", A[i]);
				}		
			srand(time(NULL));
			while(1){
				printf("\nchoose option: \n");
				printf("1 - sum of elements\n");
				printf("2 - max element\n");
				printf("3 - max to min values\n");
				printf("4 - Exit\n");
				scanf("%d", &menu12);
				
				
				if(menu12==1)				// знаходження cумми
				{
					sum=0;
					for(i = 0; i<n; i++)
						{
						sum+=A[i];	
						} 
					printf("\nsum = %d\n", sum);
				}							//end знаходження cумми
				
				if(menu12==2)			//max
				{
					max=A[0];
					for(i=1;i<n;i++)
					{
						if(abs(A[i])>abs(max))
						{
							max = abs(A[i]);
						}
					}
					for(i = 0; i<n; i++)
					{
						if(abs(A[i]) > A[maxi])
						{
							maxi = i;
						}
					}
					printf("max = %d\nindex = %d\n", max, maxi);	//endmax
				}
				
				if(menu12==3)			//max to min
				{
					printf("max to min: \n");

					for (i = 0; i<n-1; i++)
					{
				    	for (k = 0; k < n - i - 1; k++)
						{
				           if (A[k] < A[k + 1])
 					        {
  				          	a = A[k];
 				           	A[k] = A[k + 1];
 		 		          	A[k + 1] = a;
	   		    			}		
	  		      		}
  					}
	
					for (i = 0; i<n; i++)
					{
						printf("%d ", A[i]);		
					}								//end max to min
				}
				
				if(menu12==4)
				{
					break;
				}
				
				if(menu12!=1 && menu12!=2 && menu12!=3 && menu12!=4)
				{
					printf("\nerror (invalid option)\n");
				}
			}
			free(A);
		}
		
		if(menu1==1)
		{
			for (i = 0; i<n; i++)
  			{
 			   printf("A[%d] = ", i);
 			   scanf("%d", &A[i]);
 			}
 			
 				while(1){
				printf("\nchoose option: \n");
				printf("1 - sum of elements\n");
				printf("2 - max element\n");
				printf("3 - max to min values\n");
				printf("4 - Exit\n");
				scanf("%d", &menu13);
				
				
				if(menu13==1)				// знаходження cумми
				{
					for(i = 0; i<n; i++)
						{
						sum+=A[i];	
						} 
					printf("\nsum = %d\n", sum);
				}							//end знаходження cумми
				
				if(menu13==2)			//max
				{
					max=A[0];
					for(i=1;i<n;i++)
					{
						if(abs(A[i])>abs(max))
						{
							max = abs(A[i]);
						}
					}
					for(i = 0; i<n; i++)
					{
						if(abs(A[i]) > A[maxi])
						{
							maxi = i;
						}
					}
					printf("max = %d\nindex = %d\n", max, maxi);	//endmax
				}
				
				if(menu13==3)			//max to min
				{
					printf("max to min: \n");

					for (i = 0; i<n-1; i++)
					{
				    	for (k = 0; k < n - i - 1; k++)
						{
				           if (A[k] < A[k + 1])
 					        {
  				          	a = A[k];
 				           	A[k] = A[k + 1];
 		 		          	A[k + 1] = a;
	   		    			}		
	  		      		}
  					}
	
					for (i = 0; i<n; i++)
					{
						printf("%d ", A[i]);		
					}								//end max to min
				}
				
				if(menu13==4)
				{
					break;
				}
				
				if(menu13!=1 && menu13!=2 && menu13!=3 && menu13!=4)
				{
					printf("\nerror (invalid option)\n");
				}
			}
			free(A);
		}
 			if(menu1==3)
				{
					break;
				}
			}	
		}
		
		if(menu==2)
		{
			printf("enter size of arr: ");
			scanf("%d", &n);
			A = (int*)malloc(n * sizeof(int));	
			while(1)
			{
				printf("\nchoose option: \n");
				printf("1 - enter elements massive\n");
				printf("2 - random elements massive\n");
				printf("3 - Exit\n");
				scanf("%d", &menu2);
				A = (int*)malloc(n * sizeof(int));
				if(menu2==1)
					{	
						for (i = 0; i<n; i++)
  							{
   							printf("A[%d] = ", i);
  							scanf("%d", &A[i]);
 							}
 							sum=0;
 						for(i = 0; i<n; i++)
						{
							sum+=A[i];	
						} 
						avsum = sum/n;
					printf("\navarage sum = %f", avsum);
					}	
				
					if(menu2==2)
					{
						sum=0;
						srand(time(NULL));
						for(i = 0; i<n; i++)
							{
							A[i] = rand()%30;
							printf("%d ", A[i]);
							sum+=A[i];
					}	
					avsum = sum/n;
					printf("\navarage sum = %f", avsum);
			}
			
			if(menu2==3)
			{
				break;
			}
			
			if(menu2!=1 && menu2!=2 && menu2!=3)
			{
				printf("\nerror (invalid option)\n");
			}
			free (A);
		}
		
			if(menu==3)
			{
				return 0;
			}
			
		}	
	if(menu!=1 && menu!=2 && menu!=3)
			{
				printf("\nerror (invalid option)\n");
			}
	}
}
