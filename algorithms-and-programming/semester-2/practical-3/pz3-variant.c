#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>

#define ERROR 2147483647
void clean_rows(int);
int num_safe_input(int *, int, int);
void error_check_menu(char [50], int *, int, int, int);

void write_arr(int *, int);
void handwrite_arr(int *, int);
void random_arr(int *, int);
void insert(int *, int *, int, int);
void insert_range(int *, int *, int, int, int);
void excise(int *, int *, int);
void excise_slice(int *, int *, int, int);


int main()
{
	system("chcp 1251 && cls");
	srand(time(NULL));
	
	int lenght;
	error_check_menu("Введіть довжину масиву: ", &lenght, 1, 2147483647, 2);
	
	int *array;
	array = (int *) malloc(lenght * sizeof(int));
	
	if (!array)
	{
		printf("Недостатньо пам'яті!\n");
		exit(1);
	}
	
	int choice;
	error_check_menu("Заповнення масиву:\n1 - випадково\n2 - вручну\n ", &choice, 1, 2, 5);
	
	switch (choice)
	{
		case 1:
			random_arr(array, lenght);
			break;
		case 2:
			handwrite_arr(array, lenght);
			break;
	}
	write_arr(array, lenght);
	
	int rows = 10;
	int option1, option2, option3;
	
	while (1)
	{
		error_check_menu("Меню\n1 - Додавання елелемента за індексом\n2 - Видалення елемента за індексом\n3 - Додавання діапазону елементів за індексом\n4 - Видалення елементів за діапазоном індексів\n5 - Завершити\n ", &choice, 1, 5, 8);
		switch(choice)
		{
			case 1:
				error_check_menu("Введіть індекс нового елемента: ", &option1, 0, lenght, 2);
				error_check_menu("Введіть значення нового елемента: ", &option2, 0, 9, 2);
				insert(array, &lenght, option2, option1);
				rows += 2;
				break;
			case 2:
				error_check_menu("Введіть індекс елемента для видалення: ", &option1, 0, lenght-1, 2);
				excise(array, &lenght, option1);
				rows += 1;
				break;
			case 3:
				error_check_menu("Введіть індекс для вставки елементів: ", &option1, 0, lenght, 2);
				error_check_menu("Введіть значення початкового елемента для вставки: ", &option2, 0, 9, 2);
				error_check_menu("Введіть значення кінцевого елемента для вставки: ", &option3, 0, 9, 2);
				insert_range(array, &lenght, option2, option3, option1);
				rows += 3;
				break;
			case 4:
				error_check_menu("Введіть індекс першого елемента для видалення: ", &option2, 0, lenght-1, 2);
				error_check_menu("Введіть індекс останнього елемента для видалення: ", &option3, 0, lenght-1, 2);
				excise_slice(array, &lenght, option2, option3);
				rows += 2;
				break;
			case 5:
				free(array);
				return 0;
				break;
		}
		clean_rows(rows);
		rows = 10;
		write_arr(array, lenght);
	}

}


void clean_rows(int count)
{
	int i;
	if (count == 0)
	{
		system("cls");
	}
	else if(count > 0) {
		printf("\33[2K\r");	
		for (i=0; i<count; i++)
		{			
			printf("\033[A");
			printf("\33[2K\r");
			
		}
	}
}
  
int num_safe_input(int *point, int limit_a, int limit_b)
{
	int max_char_count = 50;
	char input[max_char_count];
	int num;
	
	if (fgets(input, max_char_count, stdin) == NULL)
	{
        printf("Помилка зчитування!");
        return ERROR;
    }
    if (strlen(input) == 1)
	{
    	printf("Помилка: результат введення пустий!");
    	return ERROR;
	}
    int i;
    for (i = 0; i < strlen(input); i++) {
        if (!isdigit(input[i]) && input[i] != '\n')
		{
            printf("Помилка: результат введення має бути натуральним числом!");
            return ERROR;
        }
    }
    num = strtol(input, NULL, 10);
    if (num < limit_a || num > limit_b)
	{
        printf("Помилка: результат введення має бути між %d і %d!", limit_a, limit_b);
        return ERROR;
    }
    *point = num;
}
	   
void error_check_menu(char text[50], int *link, int limit_a, int limit_b, int clean)
{
	long int output;
	int iter = 0;
	do {
		if (iter)
		{
			getchar();
			clean_rows(clean);
		}
		printf(text);
		output = num_safe_input(link, limit_a, limit_b);
		iter++;
    } while (output == ERROR);
}

void write_arr(int *array, int lenght)
{
	printf("\n ");
	int i;
	for (i=0; i<lenght; i++)
		printf("%d ", array[i]);
	printf("\n\n");
}

void handwrite_arr(int *array, int lenght)
{
	printf("Введіть значення елементів масиву:\n");
	char number[10];
	
	int i;
	for (i=0; i<lenght; i++)
		sprintf(number, "%d. ", i+1);
		error_check_menu(number, &(array[i]), 0, 9, 2);
}

void random_arr(int *array, int lenght)
{
	int i;
	for (i=0; i<lenght; i++)
		array[i] = rand() % 10;
}


void insert(int *array, int *lenght, int value, int index)
{
	realloc(array, (*lenght+1) * sizeof(int));
	int i = *lenght;
	for (i; i>index; i--)
		array[i] = array[i-1];
	array[index] = value;
	*lenght += 1;
}

void insert_range(int *array, int *lenght, int start, int end, int index)
{
	int val;
	if (end > start)
	{
		for (val=start; val<end+1; val++)
		{
			insert(array, lenght, val, index);
			index++;
		}
	}
	else
	{
		for (val=start; val>end-1; val--)
		{
			insert(array, lenght, val, index);
			index++;
		}
	}
}
void excise(int *array, int *lenght, int index)
{
	if (*lenght == 1)
		return;
	int i = index;
	for (i; i<*lenght-1; i++)
		array[i] = array[i+1];
	realloc(array, (*lenght-1) * sizeof(int));
	*lenght -= 1;
}

void excise_slice(int *array, int *lenght, int start_ind, int end_ind)
{
	int i, limiter;
	if (end_ind > start_ind)
	{
		i = start_ind;
		limiter = end_ind + 1;
	}
	else
	{
		i = end_ind;
		limiter = start_ind + 1;
	}
	for (i; i<limiter; i++)
		excise(array, lenght, start_ind);
}

