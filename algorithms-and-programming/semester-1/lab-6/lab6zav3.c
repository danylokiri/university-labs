#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void print_arr(int N, int arr[N][N]);
void create_random_arr(int N, int arr[N][N]);
void search_max_min(int N, int arr[N][N]);


int main()
{
	int N, menu;
	printf("Zavdannya 3 Lab 6\n");
	while (1)
	{
		printf("\nchoose option: \n1 - Start\n2 - Exit\n\n");
		scanf("%d", &menu);

		if (menu == 1)
		{
			printf("\nEnter a size: ");
			scanf("%d", &N);
			
			int arr[N][N];
			create_random_arr(N, arr);
			print_arr(N, arr);
			printf("\nfinish massive\n\n");
			
			search_max_min(N, arr);
			
			printf("\n");
			
		}
		else if (menu == 2)	break;
		else printf("\nerror (write a value from the list)\n");
	}
}

void create_random_arr(int N, int arr[N][N])
{
	int n;
	printf("\nenter the maximum allowable value: ");
	scanf("%d", &n);
	printf("\nstart massive: \n");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++){
			arr[i][j] = rand() % n;
		}
	}
}

void print_arr(int N, int arr[N][N])
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			printf("%d ", arr[i][j]);
		printf("\n");
	}
}

void search_max_min(int N, int arr[N][N])
{

	int i, j;
	int new_arr[N][N];
	
	int max = 0, min = N, max_ind, min_ind;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			new_arr[i][j] = arr[i][j];
			if(arr[i][j] > max)
			{
				max = arr[i][j];
				max_ind = j;
			}
			if(arr[i][j] < min)
			{
				min = arr[i][j];
				min_ind = j;
			}
		}
	}	
	new_arr[N][N] = arr[N][N];
	for (i = 0; i < N; i++)
	{
		new_arr[i][min_ind] = arr[i][max_ind];
		new_arr[i][max_ind] = arr[i][min_ind];
	}
		for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++)
			printf("%d ", new_arr[i][j]);
		printf("\n");
	}
}
