#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void random_arr(int M, int** d_arr);
void handwrite_arr(int M, int** d_arr);
void print_arr(int M, int* d_arr);
void add_element(int m, int *M, int** d_arr);
void delete_element(int m, int *M, int** d_arr);


int main()
{
	srand(time(0));
	system("chcp 1251");
	system("cls");
	
	int menu, menu1, M, m;
	
	printf("Практична 3\nТема: Динамічні одномірні масиви\n");
	while(1)
	{
		printf("\nВиберіть варіант:\n1 - Згенерувати массив рандомно\n2 - Ввести значення для генерації масиву\n3 - Вихід\n\n");
		scanf("%d", &menu);
		printf("\nВиберіть кількість елементів массиву: \n");
		scanf("%d", &M);
		printf("\n");
		
		int** d_arr = (int**) malloc(M * sizeof(int*));
		if(!d_arr)
		{
			printf("Недостатньо пам'яті\n");
			exit(1);
		}
		
		if(menu == 1)	random_arr(M, d_arr);
		else if(menu == 2)	handwrite_arr(M, d_arr);
		else if(menu == 3)	exit(1);
		else printf("\nПомилка! Виберіть пункт з меню\n\n");
		print_arr(M, d_arr);
		while(1)
		{
			printf("\nВиберіть варіант:\n1 - Додати 1 елемент массиву\n2 - Видалити 1 елемент массиву\n3 - Додати M елементів массиву\n4 - Видалити M елементів массиву\n5 - Надрукувати масив\n6 - Назад\n\n");
			scanf("%d", &menu1);
			printf("\n");
			if(menu1 == 1)
			{
				m = 1;
				add_element(m, &M, d_arr);
				m = 0;
			}
			
			else if(menu1 == 2)
			{
				m = 1;
				delete_element(m, &M, d_arr);
				m = 0;
			}
			
			else if(menu1 == 3)
			{
				printf("\nВведіть кількість елементів, яку хочете додати: \n");
				scanf("%d", &m);
				add_element(m, &M, d_arr);
				m = 0;
			}
			
			else if(menu1 == 4)
			{
				printf("\nВведіть кількість елементів, яку хочете додати: \n");
				scanf("%d", &m);
				delete_element(m, &M, d_arr);
				m = 0;
			}
			
			else if(menu1 == 5)	print_arr(M, d_arr);
			else if(menu1 == 6)	break;
			else printf("\nПомилка! Виберіть пункт з меню\n\n");
		}
	free(d_arr);
	}
	return 0;
}

void random_arr(int M, int** d_arr)
{
	int k;
	for (k = 0; k < M; k++)
		d_arr[k] = rand() % 10;
}

void handwrite_arr(int M, int** d_arr)
{
	int i, val;
    for(i = 0; i < M; i++)
    {
    	printf("Елемент [%d] = ", i);
        scanf("%d", &val);
        if(val > 0 && val < 10)
		{
           	d_arr[i] = val;
        }
        else
		{
			printf("\nПомилка! Потрібне число від 0 до 10\n\n");
			exit(1);
		}
    }	
}

void print_arr(int M, int* d_arr)
{
	int k;
		for (k = 0; k < M; k++)
			printf("%d ",d_arr[k]);
}

void add_element(int m, int* M, int** d_arr)
{
    int k, meny, val;
    *M = *M + m;
    *d_arr = (int*) realloc(*d_arr, *M * sizeof(int));
    if (!(*d_arr))
    {
        printf("Недостатньо пам'яті\n");
        exit(1);
    }
    printf("\nЕлементи додано. Як хочете заповнити додані елементи?\n1 - Рандомно\n2 - Вручну\n\n");
    scanf("%d", &meny);
    if (meny == 1)
    {
        for (k = *M-m; k < *M; k++)
            (*d_arr)[k] = rand() % 10;
    }
	else if (meny == 2)
    {
        for (k = *M-m; k < *M; k++)
        {
            printf("Елемент [%d] = ", k);
            scanf("%d", &val);
            if (val > 0 && val < 10)
            {
                (*d_arr)[k] = val;
            }
			else
            {
                printf("\nПомилка! Потрібне число від 0 до 10\n\n");
                exit(1);
            }
        }
    }
}

void delete_element(int m, int* M, int** d_arr)
{
	int k, i, temp;
	if (*M - m < 0) 
	{
		printf("\nПомилка! Неможливо видалити більше елементів, ніж міститься в масиві\n\n");
		return;
	}
	*M = *M - m;
	*d_arr = (int*) realloc(*d_arr, *M * sizeof(int));
	printf("\nЕлементи видалено\n");

	for (k = 0; k < *M; k++)
	{
		temp = (*d_arr)[k+m];
		(*d_arr)[k] = temp;
	}

	for (i = *M; i < *M + m; i++)
	(*d_arr)[i] = 0;
}
