#include <stdio.h>
#include <stdlib.h>

void print_arr(int N, int M, int arr[N][M]);
void handwrite_arr(int N, int M, int arr[N][M]);
void search_max(int N, int M, int *max_x, int *max_y, int arr[N][M]);

int main()
{
	system("chcp 1251");
	system("cls");
	int N, M, L, menu, i, j, max_x, max_y;
	printf("Практичне завдання 2\nБілет 12\n");
	while (1)
	{
		printf("\nВиберіть варіант: \n1 - Початок обчислень\n2 - Вихід\n\n");
		scanf("%d", &menu);

		if (menu == 1)
		{
			printf("\nВведіть кількість рядків: ");
			scanf("%d", &N);
			printf("Введіть кількість стовбців: ");
			scanf("%d", &M);
			int arr[N][M];
			
			handwrite_arr(N, M, arr);
			
			printf("\nСтартовий массив: \n");
			
			print_arr(N, M, arr);

			search_max(N, M, &max_x, &max_y, arr);
			printf("\nНайбільший елемент знаходиться за координатами:\nСтрока: %d\nСтолбец: %d\n", max_x+1, max_y+1);

		}
		else if (menu == 2)	return 0;
		else printf("\nПомилка! Будь-ласка, беріть значення зі списку\n");
	}
}

void handwrite_arr(int N, int M, int arr[N][M])
{
	int i, j;
	for(i = 0; i < N; i++)
       		{
           		for(j = 0; j < M; j++)
           		{
              		int val;
            	    printf("A[%d][%d] = ", i,j);
            	    scanf("%d", &val);
           		     arr[i][j] = val;
            	}
        	}	
}

void print_arr(int N, int M, int arr[N][M])
{
	int i, j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
			printf("%d ", arr[i][j]);
		printf("\n");
	}
}

void search_max(int N, int M, int *max_x, int *max_y, int arr[N][M])
{
	int max, i, j;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < M; j++)
		{
			if(arr[i][j] > max)
			{
				max = arr[i][j];
				*max_y = j;
				*max_x = i;
			}
		}
	}
	
}
