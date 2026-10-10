#include <stdio.h>
#include <stdlib.h>

void print_arr(int N, int M, int arr[N][M]);
void create_random_arr(int N, int M, int arr[N][M]);
void arr_chot(int N, int M, int arr[N][M]);

int main()
{
	int N, M, L, menu;
	printf("Zavdannya 2 Lab 6\n");
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
			printf("\n");
			
			arr_chot( N, M, arr);
			printf("\n");
			
		}
		else if (menu == 2)	break;
		else printf("\nerror (write a value from the list)\n");
	}
}

void create_random_arr(int N, int M, int arr[N][M])
{
	int n;
	printf("\nenter the maximum allowable value: ");
	scanf("%d", &n);
	printf("\nstart massive: \n");
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++){
			arr[i][j] = rand() % n;
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

void arr_chot(int N, int M, int arr[N][M])
{
	int	L = 0;
	int arr_c[N][L];
	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			if(j % 2 == 0){
				arr_c[i][L] = arr[i][j];
				printf("%d ", arr_c[i][L]);
				L++;
			}
		}
		printf("\n");
	}	
}
