#include <stdio.h>
#include <stdlib.h>

void print_arr(int N, int arr[N]);
void handwrite_arr(int N, int arr[N]); 
void searchfordiv_arr(int N, int k, int *dobutok, int arr[N]);

int main(void)
{
	system("chcp 1251");
	system("cls");
	
	int menu, N, k, i, dobutok;
	
	printf("Практичне завдання 1\nБілет 12\n");
	while (1)
	{
		printf("\nВиберіть варіант: \n1 - Початок обчислень\n2 - Вихід\n\n");
		scanf("%d", &menu);

		if (menu == 1)
		{
			printf("\nВведіть кількість елементів: ");
			scanf("%d", &N);
			printf("\nВведіть k: ");
			scanf("%d", &k);

			int arr[N];
			
			handwrite_arr(N, arr);
			
			printf("\nСтартовий массив: \n");
			
			print_arr(N, arr);
			searchfordiv_arr(N, k, &dobutok, arr);
			printf("\nДобуток елементів, які закінчують на %d: %d\n", k, dobutok);

		}
		else if (menu == 2)	return 0;
		else printf("\nПомилка! Будь-ласка, беріть значення зі списку\n");
	}	
	return 0;
}

void print_arr(int N, int arr[N]){
	int i;
	for(i = 0; i < N; i++)
     {
             printf("%d ", arr[i]);
     }
}

void handwrite_arr(int N, int arr[N])
{
	int i;
	for(i = 0; i < N; i++)
       	{
            int val;
            printf("A[%d] = ", i);
            scanf("%d", &val);
           	arr[i] = val;
       	}	
}

void searchfordiv_arr(int N, int k, int *dobutok, int arr[N])
{
	int i, d;
	for(*dobutok = 1, i = 0; i < N; i++)
       	{
       		d = arr[i]-k;
			if(d%10==0){
				*dobutok*=arr[i];
			}
       	}
}
