#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void print_arr(int N, int M, int arr[N][M]);
void create_random_arr(int N, int M, int arr[N][M]);
void swap_arr(int N, int M, int arr[N][M]);


int main()
{
	int N, M, menu;
	printf("Zavdannya 4 Lab 6\n");
	while (1)
	{
		printf("\nchoose option: \n1 - Start\n2 - Exit\n\n");
		scanf("%d", &menu);

		if (menu == 1)
		{
			printf("\nAmount of raws: ");
			scanf("%d", &N);
			printf("Amount of colums: ");
			scanf("%d", &M);
			int arr[N][M];

			create_random_arr(N, M, arr);
			print_arr(N, M, arr);
			printf("\nfinish massive\n");
			swap_arr(N, M, arr);

			printf("\n");

		}
		else if (menu == 2)	break;
		else printf("\nerror (write a value from the list)\n");
	}
}

void create_random_arr(int N, int M, int arr[N][M])
{
	int n, m, r;
	printf("\nenter the maximum allowable value: ");
	scanf("%d", &n);
	printf("\nenter the minimum allowable value: ");
	scanf("%d", &m);
	printf("\nstart massive: \n");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < N; j++){
			r = (n - m)+1;
			arr[i][j] = rand() % r + m;
		}
	}
}

void print_arr(int N, int M, int arr[N][M])
{
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
			printf("%d ", arr[i][j]);
		printf("\n");
	}
}

void swap_arr(int N, int M, int arr[N][M])
{
	int new_arr[N][M];
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		new_arr[i][j] = arr[i][j];
	}

	for(int j = 0; j < M; j++)
	{
		for(int i = 0; i < N; i++)
		{
			if(arr[i][j]>0) break;
 		else
			{
				new_arr[i][j] = arr[i][M];
				new_arr[i][M] = arr[i][j];

			}
		}
	}
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
			printf("%d ", new_arr[i][j]);
		printf("\n");
	}
}
