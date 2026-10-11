#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>


void print_arr(int N, int arr[N][N]);
void input_random_arr(int N, int arr[N][N]);
void input_handwrite(int N, int arr[N][N]);
void change_values(int N, int arr[N][N], int num);
int searchmax(int N, int arr[N]);
int *searchmin_value(int N, int arr[N][N]);



int main()
{	
	srand(time(NULL));
	
	int menu, N, num;
	int arr_n[N][N];
	
	printf("Pz.11. Zavdannya 1\n");
		printf("\nchoose option:\n1 - Random massive\n2 - Handwritten massive\n3 - Exit\n");
		scanf("%d", &menu);
		printf("\n");
		
		if(menu == 1)
		{
			printf("enter an amount of lines: ");
			scanf("%d", &N);
			input_random_arr(N, arr_n);
			printf("\nfirst massive values: \n\n");
			print_arr(N, arr_n);
			
			int *arr_min = searchmin_value(N, arr_n);
			int max_val = searchmax(N, arr_min);
			change_values(N, arr_n, max_val);
			
			printf("\nfinal massive values: \n\n");
			print_arr(N, arr_n);
			
			return 0;
		}
		else if(menu == 2)
		{
			printf("enter an amount of lines: ");
			scanf("%d", &N);
			input_handwrite(N, arr_n);
			printf("\nfirst massive values: \n\n");
			print_arr(N, arr_n);
			
			int *arr_min = searchmin_value(N, arr_n);
			int max_val = searchmax(N, arr_min);
			change_values(N, arr_n, max_val);
			
			printf("\nfinal massive values: \n\n");
			print_arr(N, arr_n);
			return 0;
			
		}
		else if(menu == 3)	return 0;
		else	printf("\nplease, choose option from the list\n");
}

void print_arr(int N, int arr[N][N]){
	for(int k = 0; k < N; k++)
     {
         for(int j = 0; j < N; j++)
         {
             printf("%5d ", arr[k][j]);
         }
         printf("\n");
     }
}

void input_random_arr(int N, int arr[N][N]){
	for(int i = 0; i<N; i++)
	{
		for (int k = 0; k<N; k++)
			arr[i][k] = rand() % 60;
	}
}

void input_handwrite(int N, int arr[N][N]){
	for(int k = 0; k < N; k++)
        {
            for(int i = 0; i < N; i++)
            {
                int val;
                printf("A[%d][%d] = ", i,k);
                scanf("%d", &val);

                arr[i][k] = val;
            }
        }
}

int *searchmin_value(int N, int arr[N][N]){
    int *arr_res = (int*) malloc(sizeof(int) * N);

     for(int k = 0; k < N; k++)
     {
         int min = arr[k][0];
         for(int j = 0; j < N; j++)
         {
             if(min > arr[j][k])
                 min = arr[j][k];
         }

         arr_res[k] = min;
     }

     return arr_res;
 }


 int searchmax(int N, int arr[N]){
     int max = arr[0];
     for(int k = 0; k < N; k++)
     {
         if(arr[k] > max)
             max = arr[k];
     }

     return max;
 }


void change_values(int N, int arr[N][N], int num){
     for(int k = 0; k < N; k++)
         arr[k][k] = num;
 }


