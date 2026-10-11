#include<stdio.h>
#include<stdlib.h>
#include <time.h>

void print_arr(int N, int arr[N][N]);
void input_random_arr(int N, int arr[N][N]);
void input_handwrite(int N, int arr[N][N]);
void change_values(int N, int arr[N][N], int val);
int searchmax(int N, int arr[N]);
int *searchmin_value(int N, int arr[N][N]);



int main()
{	
	srand(time(NULL));
	
	int menu, N;
	int arr_n[N][N];
	
	printf("Pz.11. Zavdannya 1\n");
	while(1){
		printf("\nchoose option:\n1 - Random massive\n2 - Handwritten massive\n3 - Exit\n");
		scanf("%d", &menu);
		printf("\n");
		
		if(menu == 1)
		{
			printf("enter an amount of lines: ");
			scanf("%d", &N);
			input_random_arr(arr_n, N);
			printf("\nfirst massive values: \n\n");
			print_arr(arr_n, N);
			
			int *arr_min = searchmin_value(N, arr_n);
			int max_val = searchmax(N, arr_min);
			change_values(N, max_val, arr_n);
			
			printf("\nfinal massive values: \n\n");
			print_arr(N, arr_n);
			
			free(arr_n);
		}
		else if(menu == 2)
		{
			printf("enter an amount of lines: ");
			scanf("%d", &N);
			input_random_arr(arr_n, N);
			printf("\nfirst massive values: \n\n");
			print_arr(arr_n, N);
			
			int *arr_min = searchmin_value(N, arr_n);
			int max_val = searchmax(N, arr_min);
			change_values(N, max_val, arr_n);
			
			printf("\nfinal massive values: \n\n");
			print_arr(N, arr_n);
			
			free(arr_n);
			
		}
		else if(menu == 3)	break;
		else	printf("\nplease, choose option from the list\n");
	}
}

void print_arr(int N, int arr[N][N]){
	for(int k = 0; k < N; k++)
     {
         for(int i = 0; i < N; i++)
         {
             printf("%d\n", arr[k][i]);
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
                scanf("%d", &val);

                arr[k][i] = val;
            }
        }
}

int searchmax(int N, int arr[N]){
	int max;
	max = 0;
	for(int i = 0; i<N; ++i)
	{
		if(arr[i] > max)
		max = arr[i];	
	}
return max;
}

int *searchmin_value(int N, int arr[N][N]){
	int *arr_res = (int*) mallos(sizeof(int)*N);
	
	for(int i = 0; i<N; i++)
	{
		int min = arr[i][0];
		for(int k = 0; k<N; k++)
		{
			if(min>arr[k][i])
				min = arr [k][i];
		}
		arr_res[i] = min;
	}
	return arr_res;
}

void change_values(int N, int arr[N][N], int val){
	for(int i = 0; i<N; i++)
		arr[i][i] = val;
}
