#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

void print_arr(int N, int M, int arr[N][M]);
void random_arr(int N, int M, int arr[N][M]);
void handwrite_arr(int N, int M, int arr[N][M]);
void tranform_arr(int N, int M, int arr[N][M]);
int convert_colums(int N, int M, int arr[N][M]);
int convert_lines(int N, int M, int arr[N][M]);



int main()
{
	system("chcp 1251");
	system("cls");
	srand(time(NULL));
	
	int N, M, menu, menu1, menu0, menu01;
	
	printf("Практичне завдання 1\n""Статичний масив""\n");
	while (1)
	{
		printf("\nВиберіть варіант:\n1 - Згенерувати массив рандомно\n2 - Ввести значення для генерації масиву\n3 - Вихід\n\n");
		scanf("%d", &menu1);
		printf("\n");
		
		if (menu1 == 1 || menu1 == 2)
		{
			printf("Виберіть кількість рядків: ");
			scanf("%d", &N);
			printf("Виберіть кількість стовпців: ");
			scanf("%d", &M);
			printf("\n");
			
			int arr[N][M], res_arr[N];
			
			if(menu1 == 1)	random_arr( N, M, arr);	//згенерувати массив
			if(menu1 == 2)	handwrite_arr( N, M, arr);
			print_arr( N, M, arr);	//вивести массив
			while(1)
			{
				printf("\nВиберіть варіант: \n1 - Перетворити масив\n2 - Перевести рядки в десяткову систему\n3 - Перевести стовпці в десяткову систему\n4 - Назад\n\n");
				scanf("%d", &menu);
				printf("\n");

				if (menu == 1)
				{
					tranform_arr( N, M, arr);	//перетворення
					print_arr( N, M, arr);	//прінт
					while(1)
					{
						printf("\nВиберіть варіант: \n1 - Перетворити масив\n2 - Перевести рядки в десяткову систему і знайти найбільше\n3 - Перевести стовпці в десяткову систему і знайти найбільше\n4 - Назад\n5 - Вихід\n\n");
						scanf("%d", &menu0);
						printf("\n");
			
						if(menu0 == 1)
						{
							while(1)
							{
								printf("\nВаш масив вже перетворений!\nБажаєте згенерувати новий і перетворити його?\n1 - Ні\n2 - Так\n\n");
								scanf("%d", &menu01);
								printf("\n");
								
								if(menu01 == 1)		break;
								else if(menu01 == 2)
								{
									random_arr( N, M, arr);	//генерація нового
									print_arr( N, M, arr);	//прінт
									printf("\n");
									tranform_arr( N, M, arr);	//перетворення
									print_arr( N, M, arr);	//прінт
									break;	
								}
								else printf("\nПомилка! Будь-ласка, беріть значення зі списку\n");
							}
						}
						else if(menu0 == 2)
						{
							convert_lines( N, M, arr);	//перетворення рядка в десятичну
							printf("\n");
						}
						else if(menu0 == 3)
						{
							convert_colums( N, M, arr);
							printf("\n");
						}
						else if(menu0 == 4)		break;
						else if(menu0 == 5)		exit(1);
						else printf("\nПомилка! Будь-ласка, беріть значення зі списку\n");
					}
				}
				else if (menu == 2)
				{
					printf("\nПомилка! Для виповнення треба перетворити масив!\n");
				}
				else if (menu == 3)
				{
					printf("\nПомилка! Для виповнення треба перетворити масив!\n");
				}
				else if (menu == 4)		break;
				else printf("\nПомилка! Будь-ласка, беріть значення зі списку\n");
			}
		}
		else if (menu1 == 3) return 0;
		else printf("\nПомилка! Будь-ласка, беріть значення зі списку\n");	
	}
}

void random_arr(int N, int M, int arr[N][M])
{
	int i, k;
	for(i = 0; i < N; i++)
	{
		for (k = 0; k < M; k++)
			arr[i][k] = rand() % 10;
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
            	    printf("Елемент [%d][%d] = ", i,j);
            	    scanf("%d", &val);
            	    if(val > 0 && val < 10)
					{
           		    	arr[i][j] = val;
           			}
           			else
					{
						printf("\nПомилка! Потрібне число від 0 до 10\n\n");
						exit(1);
					}
            	}
        	}	
}

void print_arr(int N, int M, int arr[N][M])
{
	int i, k;
	for(i = 0; i < N; i++)
	{
		for (k = 0; k < M; k++)
			printf("%5d",arr[i][k]);
		printf("\n");
	}
}

void tranform_arr(int N, int M, int arr[N][M])
{
	int i, k;
	for(i = 0; i<N; i++)
	{
		for (k = 0; k<M; k++)
			if(arr[i][k] % 2 == 0)	arr[i][k] = 0;
			else	arr[i][k] = 1;
	}
}

int convert_lines(int N, int M, int arr[N][M])
{
    int i, k, res, max_ind = 0, max = 0, temp_arr[N], res_arr[N];
    for(i = 0; i < N; i++)
	{
        res = 0;
        for(k = 0; k < M; k++)
		{
            res = res * 10 + arr[i][k];
        }
        temp_arr[i] = res;   
    }
    
    for(i = 0; i < N; i++)
	{
		res = 0;
        for(k = 0; temp_arr[i] != 0; ++k)
		{
            int digit = temp_arr[i] % 10;
            res += digit * pow(2, k);
            temp_arr[i] /= 10;
        }
        res_arr[i] = res;
    }
    
    for(i = 0; i < N; i++)
			printf("%d-ий рядок: %d\n", i+1, res_arr[i]);
    
	for(i = 0; i<N; i++)
	{
		if(res_arr[i] > max)
		{
			max = res_arr[i];
			max_ind = i;
		}
	}
	printf("\nМаксимальне число у %d-у рядку\n", max_ind+1);
}

int convert_colums(int N, int M, int arr[N][M])
{
    int i, k, res, max_ind = 0, max = 0, temp_arr[M], res_arr[M];
    for(i = 0; i < M; i++)
	{
        res = 0;
        for(k = 0; k < N; k++)
		{
            res = res * 10 + arr[k][i];
        }
        temp_arr[i] = res;   
    }
    
    for(i = 0; i < M; i++)
	{
		res = 0;
        for(k = 0; temp_arr[i] != 0; ++k)
		{
            int digit = temp_arr[i] % 10;
            res += digit * pow(2, k);
            temp_arr[i] /= 10;
        }
        res_arr[i] = res;
    }
    
    for(i = 0; i < M; i++)
			printf("%d-ий стовпець: %d\n", i+1, res_arr[i]);
    
	for(i = 0; i<M; i++)
	{
		if(res_arr[i] > max)
		{
			max = res_arr[i];
			max_ind = i;
		}
	}
	printf("\nМаксимальне число у %d-у стовпці\n", max_ind+1);
}


