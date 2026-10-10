#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#define N 5

struct product
{
	char name[10];
	int count;
	double price;
} product[N];

int replace_product(struct product*, struct product);
void print_list(struct product*);
void add_product(struct product*, int);
double double_check_errors(double);
int check_errors(int);
void fake_product(struct product*);

int main ()
{
	
	system("chcp 1251");
	system("cls");
	printf("Практичне завдання 9\nЗавдання 2\nКірієнко Данило\n");
	
	FILE *file;
	int i, product_i = -1;
	struct product tmp;
	
	printf("Додання товару\n");
	add_product(product, N);
	
	if ((file = fopen("file_in2", "wb")) != NULL) fwrite(&product, sizeof(struct product), 5, file);
	else
	{
		
		printf("Помилка при зчитуванні файлу!\n");
		return 0;
		
	} 
	
	print_list(product);
	fclose(file);
	printf("Копіювання даних виконано! Натисніть ENTER\n");
	system("PAUSE");
	
	product_i = replace_product(product, tmp);
	
	if ((file = fopen("file_in2", "rb+")) != NULL)
	{
		
		fseek(file, product_i * sizeof(struct product), SEEK_SET);
		fwrite(&tmp, sizeof(struct product), 1, file);
		
	}
	else{
		
		printf("Помилка при зчитуванні файлу!\n");
		return 0;
		
	}
	
	fclose(file);
	print_list(product);
	printf("Заміна даних виконано! Натисніть ENTER\n");
	system("PAUSE");
	
	fake_product(product);
	
	return 0;
}

void fake_product(struct product* product)
{
	
	int i, j, bytes = 0;
	FILE *file;
	struct product fake = {"Mask", 100, 10.5};
	struct product tmp;
	
	printf("Номер\tНазва\tКількість\tЦіна\n");
	
	if ((file = fopen("file_in2", "wb")) != NULL)
	{
		
		for (i = 0, j = 0; i < N * 2; i++)
		{
		
			if (i % 2 == 0){
				
				tmp.count = product[j].count;
				tmp.price = product[j].price;
				strcpy(tmp.name, product[j].name);
				
				fseek(file, bytes * sizeof(struct product), SEEK_SET);
				fwrite(&tmp, sizeof(struct product), 1, file);
				bytes++;
				
				if (strlen(tmp.name) >= 8) printf("%d\t%s\t\t%d\t%lf\n", i + 1, tmp.name, tmp.count, tmp.price);
				else printf("%d\t%s\t%d\t%lf\n", i + 1, tmp.name, tmp.count, tmp.price);
				j++;
			}
			else
			{
			
				fseek(file, bytes * sizeof(struct product), SEEK_SET);
				fwrite(&fake, sizeof(struct product), 1, file);
				bytes++;
				printf("%d\t%s\t%d\t%lf\n", i + 1, fake.name, fake.count, fake.price);
				
			}
		}	
	}
	else
	{
		
		printf("Помилка зчитування файлу\n");
		return;
		
	}
	fclose(file);
}

void add_product(struct product* product, int size)
{
	
	int i;
	
	for (i = 0; i < size; i++)
	{
		
		printf("Введіть назву %d товару(макс. 10 символів)\n", i + 1);
		scanf("%s", &product[i].name);
		
		printf("Введіть кількість %d товару в штуках\n", i + 1);
		scanf("%d", &product[i].count);
		product[i].count = check_errors(product[i].count);
		
		printf("Введіть вартість всієї кількості %d товару\n", i + 1);
		scanf("%lf", &product[i].price);
		product[i].price = double_check_errors(product[i].price);
		
	}
}

void print_list(struct product* product)
{
	
	int i;
	
	printf("Номер\tНазва\t\tКількість\tЦіна\n");
	for (i = 0; i < N; i++)
	{
		
		if (strlen(product[i].name) >= 8) printf("%d\t%s\t\t%d\t\t%.2lf\n", i + 1, product[i].name, product[i].count, product[i].price);
		else printf("%d\t%s\t\t%d\t\t%.2lf\n", i + 1, product[i].name, product[i].count, product[i].price);
		
	}
}

int replace_product(struct product* product, struct product tmp)
{
	
	char string[10];
	int i, product_i = -1;
	
	printf("Введіть назву товару, який хочете змінити(макс. 10 символів): ");
	scanf("%s", &string);
	
	while(product_i == -1)
	{
		
		for (i = 0; i < N; i++)
		{
		
			if (strcmp(string, product[i].name) == 0)
			{
			
				printf("Товар із заданим іменем існує\n");
				product_i = i;
				printf("Індекс продукту - %d\n", product_i);
				
			}
		}	
		if (product_i == -1)
		{
			
			printf("Продукт з таким іменем не знайдено, Повторіть введення(10 символів): ");
			scanf("%s", &string);
	
		}
	}
	
	printf("Введіть назву %d товару(макс. 10 символів)\n", product_i + 1);
	scanf("%s", &tmp.name);
	strcpy(product[product_i].name, tmp.name);
	
	printf("Введіть кількість %d товару в штуках\n", product_i + 1);
	scanf("%d", &tmp.count);
	tmp.count = check_errors(tmp.count);
	product[product_i].count = tmp.count;
	
	printf("Введіть вартість всієї кількості %d товару\n", product_i + 1);
	scanf("%lf", &tmp.price);
	tmp.price = double_check_errors(tmp.price);
	product[product_i].price = tmp.price;
	
	return product_i;
}

int check_errors(int check_number)
{
	while (check_number < 0)
	{
		
		printf("Помилка! Введене неправильне значення. Повторіть введення: ");
		scanf("%d", &check_number);
		fflush(stdin);
		
	}
	return check_number;
}

double double_check_errors(double check_number)
{
	while (check_number < 0)
	{
		
		printf("Помилка! Введене неправильне значення. Повторіть введення: ");
		scanf("%d", &check_number);
		fflush(stdin);
		
	}
	return check_number;
}
