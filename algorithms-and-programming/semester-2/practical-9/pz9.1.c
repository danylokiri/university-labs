#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

int main ()
{
	
	system("chcp 1251");
	printf("Практичне завдання 9\nЗавдання 1\nКірієнко Данило\n");
	system("cls");
	
	FILE *file;
	int i, max = 0, min = 0, num = 0;
	
	if ((file = fopen("file_in.dat", "wb")) != NULL)
	{
		
		for (i = 0; i < 10; i++)
		{
			
			num = rand() % 100;
			fwrite(&num, sizeof(int), 1, file);
			printf("число %d = %d\n", i + 1, num);
			
		}
	}
	else
		printf("Помилка при зчитанні файлу\n");
		
	fclose(file);
	
	if ((file = fopen("file_in.dat", "rb")) != NULL)
	{
		
		fread(&num, sizeof(int), 1, file);
		for (i = 1, max = num, min = num; i < 10; i++)
		{
			
			fread(&num, sizeof(int), 1, file);
			if (max < num) max = num;
			if (min > num) min = num;
			
		}
	}
	else
		printf("Помилка при зчитанні файлу\n");
		
	fclose(file);
	printf("max = %d, min = %d\n", max, min);
	
	return 0;
}
