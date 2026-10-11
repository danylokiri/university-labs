#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct writing{
	char name[15];
	int year;
	int size;
	union {
		struct{
			char author[30];
		} book;
		struct{
			int edition;
			int article;
		} magazine;
	};
};

int menu(struct writing**, int*, int*, int*, int*, int*);
void add_writing(struct writing**, int*, int*, int*, int*, int*);
void print_books(struct writing**, int*, int*);
void print_magazines(struct writing**, int*, int*);
void print_editions(struct writing **, int*, int*);

int main()
{
	
	system("chcp 1251");
	system("cls");
	
	printf("Практичне завдання 4\nКірієнко Данило\n");
	
	int size = 0, size_books = 1, size_magazines = 1;
	int *magazines, *books;
	
	magazines = (int*) malloc(size_magazines + 1 * sizeof(int));
	books = (int*) malloc(size_books + 1 * sizeof(int));
    
	struct writing *text = (struct writing*) malloc (size * sizeof(struct writing));
    
    menu(&text, &size, magazines, books, &size_magazines, &size_books);
}

int menu(struct writing **text, int* size, int *magazines, int *books, int* size_magazines, int* size_books){
	
	while(1)
	{
		printf("\n1 - Додавання книги/журналу\n");
		
		printf("2 - Інформація про всі книги\n");
		
		printf("3 - Інформація про всі журнали\n");
		
		printf("4 - Загальна кількість статей у всіх журналах\n");
		
		printf("0 - Завершити програму\n");
		
		int menubar;
		scanf("%d", &menubar);
		fflush(stdin);
		
		switch(menubar){
			case 1:
				add_writing(text, size, books, magazines, size_books, size_magazines);
				break;
			
			case 2: 
				if ((*size_books) == 1)
				{
					printf("Список книжок порожній\n");
					break;
				}
				print_books(text, books, size_books);
				break;
			
			case 3:
				if ((*size_magazines) == 1)
				{
					printf("Список журналів порожній\n");
					break;
				}
				print_magazines(text, magazines, size_magazines);
				break;
			
			case 4: 
				if ((*size_magazines) == 1)
				{
					printf("Список журналів порожній\n");
					break;
				}
				print_editions(text, magazines, size_magazines);
				break;
			
			case 0:
				printf("\nПрограму завершено\n"); 
				return 0;
			
			default: printf("\nПомилка: такого варінту не існує. Повторіть введення\n\n");
		}
	}
}

int error_check(int check_number){
	while (check_number < 0){
		
		printf("Помилка: введено некоректне значення. Повторіть введення: ");
		scanf("%d", &check_number);
		fflush(stdin);
		
	}
	return check_number;
}

void add_writing(struct writing **text, int* size, int* books, int* magazines, int* size_books, int* size_magazines){
	
	(*text) = realloc ((*text), ((*size) + 1) * sizeof(struct writing));
	
	char temporary_slot[15];
	int type = 0, i, j;
	
	printf("Визначте тип документу:\n");
	printf("1 - Книга\n");
	printf("2 - Журнал\n");
	scanf("%d", &type);
	fflush(stdin);
	while (type != 1 && type != 2){
		
		printf("Помилка: такого варінту не існує. Повторіть введення\n");
		scanf("%d", &type);
		fflush(stdin);
		
	}
	
	if (type == 1) printf("Тип введення даних = 'Книга'\n");
	else printf("Тип введення даних = 'Журнал'\n");
	printf("Введіть назву(макс. 15 символів): ");
	scanf("%s", &temporary_slot[0]);
	strcpy((*text)[(*size)].name, temporary_slot);

		
	printf("Введіть рік видання: ");
	scanf("%d", &(*text)[(*size)].year);
	fflush(stdin);
	(*text)[(*size)].year = error_check((*text)[(*size)].year); 
		
	printf("Введіть кількість сторінок: ");
	scanf("%d", &(*text)[(*size)].size);
	fflush(stdin);
	(*text)[(*size)].size = error_check((*text)[(*size)].size);
	
	if (type == 1){
		
		books[(*size_books) - 1] = (*size);
		books = (int*) realloc(books, ((*size_books) + 1) * sizeof(int));
		
		char temp_slot[30];

		printf("Введіть автора книги(введіть '_' між прізвищем та ім'ям): ");
		scanf("%s", &temp_slot[0]);
		strcpy((*text)[(*size)].book.author, temp_slot);
		(*size_books)++;
	}
	else if(type == 2){
		
		magazines[(*size_magazines) - 1] = (*size);
		magazines = (int*) realloc(magazines, ((*size_magazines) + 1) * sizeof(int));
		
		printf("Введіть номер випуску журналу: ");
		scanf("%d", &(*text)[(*size)].magazine.edition);
		fflush(stdin);
		(*text)[(*size)].magazine.edition = error_check((*text)[(*size)].magazine.edition);
		
		printf("Введіть кількість статей в журналі: ");
		scanf("%d", &(*text)[(*size)].magazine.article);
		fflush(stdin);
		(*text)[(*size)].magazine.article = error_check((*text)[(*size)].magazine.article);
		(*size_magazines)++;
	}
	(*size)++;
}

void print_books(struct writing **text, int* books, int* size_books)
{
	int i;
	printf("Виведення всіх книг зі списку\n");
	printf("Назва\t\tРік видання\tКількість сторінок\tАвтор\n");
	
	for (i = 0; i < (*size_books) - 1; i++)
	{

		if (strlen((*text)[books[i]].name) >= 8) printf("%s\t%d\t\t%d\t\t\t%s\n", (*text)[books[i]].name, (*text)[books[i]].year, (*text)[books[i]].size, (*text)[books[i]].book.author);
		else printf("%s\t\t%d\t\t%d\t\t\t%s\n", (*text)[books[i]].name, (*text)[books[i]].year, (*text)[books[i]].size, (*text)[books[i]].book.author);
			
	}
}

void print_magazines(struct writing **text, int* magazines, int* size_magazines)
{
	int i;
	printf("Виведення всіх журналів зі списку\n");
	printf("Назва\t\tРік видання\tКількість сторінок\tВипуск\tКількість статей\n");
	
	for (i = 0; i < (*size_magazines) - 1; i++)
	{
		if (strlen((*text)[magazines[i]].name) >= 8) printf("%s\t%d\t\t%d\t\t\t%d\t\t%d\n", (*text)[magazines[i]].name, (*text)[magazines[i]].year, (*text)[magazines[i]].size, (*text)[magazines[i]].magazine.edition, (*text)[magazines[i]].magazine.article);
		else printf("%s\t\t%d\t\t%d\t\t\t%d\t\t%d\n", (*text)[magazines[i]].name, (*text)[magazines[i]].year, (*text)[magazines[i]].size, (*text)[magazines[i]].magazine.edition, (*text)[magazines[i]].magazine.article);		
	}
}

void print_editions(struct writing **text, int* magazines, int* size_magazines)
{
	int i, sum = 0;
	printf("\nВиведення всіх статей, журналів зі списку\n");
	
	for (i = 0; i < (*size_magazines) - 1; i++)
	{
		sum += (*text)[magazines[i]].magazine.article;
		
	}
	printf("Загальна кількість статей - %d\n\n", sum);
}
