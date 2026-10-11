#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct student
{
	char secondname[15];
	char name[15];
	char middle_name[15];
	struct
	{
		int day;
		int month;
		int year;
	} date;
	int grade;
};

void menu(struct student** students, int*);
void add_student(struct student** students, int*);
void print_list(struct student** students, int*);
void del_student(struct student** students, int*);
void sort_students(struct student** students, int*);
void print_search(struct student** students, int*, int);
void search_secondname(struct student** students, int*);
void search_date(struct student** students, int*);
void search_grade(struct student** students, int*);
void search_menu(struct student** students, int*);

int main()
{
	system("chcp 1251");
	system("cls");
	
	printf("Практичне завдання 4\nКірієнко Данило\n"); 
	
	int size = 0;
	struct student *students = (struct student*)malloc (size * sizeof(struct student));
	
	menu(&students, &size);
	return 0;
}

void menu(struct student **students, int* size)
{
	
	while(1)
	{
		
		printf("\n1 - Додати студента\n");
		
		printf("2 - Пошук студента\n");
		
		printf("3 - Видалити студента\n");
		
		printf("4 - Друк списку групи\n");
		
		printf("5 - Завершити програму\n\n");
		
		int menubar;
		scanf("%d", &menubar);
		fflush(stdin);
		
		switch(menubar)
		{
			case 1:
				add_student(students, size);
				break;
			
			case 2: 
				if((*size) == 0)
				{
					printf("Список студентів порожній, додайте студента в попередньому меню та спробуйте ще раз\n");
					break;
				}
				else search_menu(students, size);
				break;
			
			case 3:
				if((*size) == 0)
				{
					printf("Список студентів порожній, додайте студента в попередньому меню та спробуйте ще раз\n");
					break;
				}
				else del_student(students, size);
				break;
			
			case 4:
				if((*size) == 0)
				{
					printf("Список студентів порожній, додайте студента в попередньому меню та спробуйте ще раз\n");
					break;
				}
				else print_list(students, size);
				break;
			
			case 5:
			printf("\nПрограму завершено\n"); 
			return;
			
			default: printf("\nПомилка! Такого варінту не існує. Повторіть введення\n\n");
		}
	}
}

void search_menu(struct student **students, int* size)
{
	
	while(1)
	{	
		printf("\n1 - Пошук студента за прізвищем\n");
		
		printf("2 - Пошук студента за діапазоном успішності\n");
		
		printf("3 - Пошук студента за датою народження\n");
		
		printf("4 - Назад\n\n");
		
		int menubar;
		scanf("%d", &menubar);
		fflush(stdin);
		
		switch(menubar)
		{
			case 1:
				search_secondname(students, size);
				break;
			
			case 2:
				search_grade(students, size);
				break;
			
			case 3:
				search_date(students, size);
				break;

			case 4:
				return;
			
			default: printf("\nПомилка! Такого варінту не існує. Повторіть введення\n\n");
		}
	}
}

void add_student(struct student **students, int* size)
{
	
	int i;
	char temporary_string[15];
	(*students) = realloc ((*students), ((*size) + 1) * sizeof(struct student));

	printf("Введіть прізвище студента(макс. 15 символів): ");
	scanf("%s", &(*students)[(*size)].secondname);
	fflush(stdin);
	
	printf("Введіть ім'я студента(макс. 15 символів): ");
	scanf("%s", &(*students)[(*size)].name);
	fflush(stdin);
	
	printf("Введіть по-батькові студента(макс. 15 символів): ");
	scanf("%s", &(*students)[(*size)].middle_name);
	fflush(stdin);
	
	printf("Введіть дату народження студента дд.мм.рррр: ");
	scanf("%02d.%02d.%d", &(*students)[(*size)].date.day, &(*students)[(*size)].date.month, &(*students)[(*size)].date.year);
	fflush(stdin);
	
	while((*students)[(*size)].date.year > 2023 || (*students)[(*size)].date.year < 1980)
	{
		
		printf("Невірне значення року, Повторіть введення року народження: ");
		scanf("%d", &(*students)[(*size)].date.year);
		fflush(stdin);		
	}
	
	while((*students)[(*size)].date.month > 12 || (*students)[(*size)].date.month < 1)
	{
		
		printf("Невірне значення місяця, Повторіть введення місяця народження: ");
		scanf("%02d", &(*students)[(*size)].date.month);
		fflush(stdin);
	}
	
	while((*students)[(*size)].date.day > 31 && (*students)[(*size)].date.month == 1 || (*students)[(*size)].date.day > 31 && (*students)[(*size)].date.month == 3 || (*students)[(*size)].date.day > 31 && (*students)[(*size)].date.month == 5 || (*students)[(*size)].date.day > 31 && (*students)[(*size)].date.month == 7 || (*students)[(*size)].date.day > 31 && (*students)[(*size)].date.month == 8 || (*students)[(*size)].date.day > 31 && (*students)[(*size)].date.month == 10 || (*students)[(*size)].date.day > 31 && (*students)[(*size)].date.month == 12 || (*students)[(*size)].date.day > 30 && (*students)[(*size)].date.month == 4 || (*students)[(*size)].date.day > 30 && (*students)[(*size)].date.month == 6 || (*students)[(*size)].date.day > 30 && (*students)[(*size)].date.month == 9 || (*students)[(*size)].date.day > 30 && (*students)[(*size)].date.month == 11 || (*students)[(*size)].date.day > 28 && (*students)[(*size)].date.month == 2 && (*students)[(*size)].date.year % 4 != 0 || (*students)[(*size)].date.day > 29 && (*students)[(*size)].date.month == 2 && (*students)[(*size)].date.year % 4 == 0)
	{
		
		printf("Невірне значення дня народження, Повторіть введення дня народження: ");
		scanf("%02d", &(*students)[(*size)].date.day);
		fflush(stdin);
		
	} 
	
	printf("Введіть оцінку студента за 100-юальною шкалою: ");
	scanf("%d", &(*students)[(*size)].grade);
	fflush(stdin);
	
	while((*students)[(*size)].grade > 100 || (*students)[(*size)].grade < 0)
	{
		
		printf("Помилка! Значення має бути від 0 до 100, Повторіть введення: ");
		scanf("%d", &(*students)[(*size)].grade);
		fflush(stdin);
		
	}
	(*size)++;
	if ((*size) > 1) sort_students(students, size);
}

void sort_students(struct student **students, int* size)
{
    int i, j, temporary = 0;
    char temp[15];
    
    for (i = 0; i < (*size) - 1; i++) {
        for (j = i + 1; j < (*size); j++) {
            if (strcmp((*students)[i].secondname, (*students)[j].secondname) > 0) {
            	// прізвище
            	strcpy(temp, (*students)[i].secondname); 
                strcpy((*students)[i].secondname, (*students)[j].secondname);
                strcpy((*students)[j].secondname, temp);
                // ім'я
                strcpy(temp, (*students)[i].name);
                strcpy((*students)[i].name, (*students)[j].name);
                strcpy((*students)[j].name, temp);
                // по-батькові
                strcpy(temp, (*students)[i].middle_name);
                strcpy((*students)[i].middle_name, (*students)[j].middle_name);
                strcpy((*students)[j].middle_name, temp);
                //  дата
                temporary = (*students)[i].date.day;
                (*students)[i].date.day = (*students)[j].date.day;
                (*students)[j].date.day = temporary;
                
                temporary = (*students)[i].date.month;
                (*students)[i].date.month = (*students)[j].date.month;
                (*students)[j].date.month = temporary;
                
                temporary = (*students)[i].date.year;
                (*students)[i].date.year = (*students)[j].date.year;
                (*students)[j].date.year = temporary;
                // оцінка
                temporary = (*students)[i].grade;
                (*students)[i].grade = (*students)[j].grade;
                (*students)[j].grade = temporary; 
            }
        }
    }
}

void print_list(struct student **students, int* size)
{

	int i = 0;

	printf("Номер\tПрізвище\tІм'я\t\tПо-батькові\tДата народження\tСередня успішність\n");
	
	for (i = 0; i < (*size); i++)
	{
		
		if (strlen((*students)[i].secondname) >= 8 && strlen((*students)[i].name) >= 8 && strlen((*students)[i].middle_name) >= 8) printf("%d\t\t%s\t%s\t%s\t%02d.%02d.%d\t%d\n", i + 1, (*students)[i].secondname, (*students)[i].name, (*students)[i].middle_name, (*students)[i].date.day, (*students)[i].date.month, (*students)[i].date.year, (*students)[i].grade);
		else if(strlen((*students)[i].secondname) >= 8 && strlen((*students)[i].name) >= 8 && strlen((*students)[i].middle_name) < 8) printf("%d\t\t%s\t%s\t%s\t\t%02d.%02d.%d\t%d\n", i + 1, (*students)[i].secondname, (*students)[i].name, (*students)[i].middle_name, (*students)[i].date.day, (*students)[i].date.month, (*students)[i].date.year, (*students)[i].grade);
		else if(strlen((*students)[i].secondname) >= 8 && strlen((*students)[i].name) < 8 && strlen((*students)[i].middle_name) < 8) printf("%d\t\t%s\t%s\t\t%s\t\t%02d.%02d.%d\t%d\n", i + 1, (*students)[i].secondname, (*students)[i].name, (*students)[i].middle_name, (*students)[i].date.day, (*students)[i].date.month, (*students)[i].date.year, (*students)[i].grade);
		else if(strlen((*students)[i].secondname) < 8 && strlen((*students)[i].name) < 8 && strlen((*students)[i].middle_name) < 8) printf("%d\t\t%s\t\t%s\t\t%s\t\t%02d.%02d.%d\t%d\n", i + 1, (*students)[i].secondname, (*students)[i].name, (*students)[i].middle_name, (*students)[i].date.day, (*students)[i].date.month, (*students)[i].date.year, (*students)[i].grade);
		else if(strlen((*students)[i].secondname) < 8 && strlen((*students)[i].name) < 8 && strlen((*students)[i].middle_name) >= 8) printf("%d\t\t%s\t\t%s\t\t%s\t%02d.%02d.%d\t%d\n", i + 1, (*students)[i].secondname, (*students)[i].name, (*students)[i].middle_name, (*students)[i].date.day, (*students)[i].date.month, (*students)[i].date.year, (*students)[i].grade);
		else if(strlen((*students)[i].secondname) < 8 && strlen((*students)[i].name) >= 8 && strlen((*students)[i].middle_name) >= 8) printf("%d\t\t%s\t\t%s\t%s\t%02d.%02d.%d\t%d\n", i + 1, (*students)[i].secondname, (*students)[i].name, (*students)[i].middle_name, (*students)[i].date.day, (*students)[i].date.month, (*students)[i].date.year, (*students)[i].grade);
		else if(strlen((*students)[i].secondname) >= 8 && strlen((*students)[i].name) < 8 && strlen((*students)[i].middle_name) >= 8) printf("%d\t\t%s\t%s\t\t%s\t%02d.%02d.%d\t%d\n", i + 1, (*students)[i].secondname, (*students)[i].name, (*students)[i].middle_name, (*students)[i].date.day, (*students)[i].date.month, (*students)[i].date.year, (*students)[i].grade);
	
	}
}

void del_student(struct student **students, int* size)
{
	
	int i, number_student, counter = 0;
	
	printf("Введіть номер студента якого хочете вилучити зі списку: ");
	scanf("%d", &number_student);
	fflush(stdin);
	number_student--;
	
	while (number_student < 0 || number_student > (*size))
	{
		
		printf("Помилка! Такого студента не існує. Повторіть введення: ");
		scanf("%d", &number_student);
		fflush(stdin);
		
	}
	
	for (i = number_student; i < (*size) - 1; i++) (*students)[i] = (*students)[i + 1];
	printf("Успішно видалено зі списку!\n");
	(*size)--;
}

void search_secondname(struct student **students, int* size)
{
	
	int i, j, k = 0, counter = 0, counter_students = 0, array_students[50];
	char secondname[15];
	
	printf("Введіть прізвище, за яким ви хочете переглянути дані: ");
	fgets(secondname, sizeof(secondname), stdin);

	for (i = 0; i < (*size); i++)
	{
		for (j = 0; j < strlen((*students)[i].secondname); j++)
		{
			if(secondname[j] != (*students)[i].secondname[j])
			{
				counter = 0;
				break;
					
			}	
			counter++;
		}
		
		if (counter == strlen((*students)[i].secondname)) 
		{
			array_students[k] = i;
			k++;
			counter_students++;	
			
		}
		counter = 0;
	}
		
	if (counter_students == 0)
	{
		
		printf("Такого прізвища немає в списку\n")
		;
		return;
	}
	else print_search(students, array_students, counter_students);
}

void search_date(struct student **students, int* size)
{
	int i, j = 0, day, month, counter_students = 0;
	int array_students[50];
	
	printf("Введіть дату за якою хочете здійснити пошук(в форматі дд.мм): ");
	scanf("%02d.%02d", &day, &month);
	fflush(stdin);
	
	while(month > 12 || month < 1 || day > 31 && month == 1 || day > 31 && month == 3 || day > 31 && month == 5 || day > 31 && month == 7 || day > 31 && month == 8 || day > 31 && month == 10 || day > 31 && month == 12 || day > 30 && month == 4 || day > 30 && month == 6 || day > 30 && month == 9 || day > 30 && month == 11 || day > 28 && month == 2){
		
		printf("Введене некоректна дата(параметри введення такі ж як і при додаванні студента в список), Повторіть введення: ");
		scanf("%02d.%02d", &day, &month);
		fflush(stdin);
	}
	
	for (i = 0; i < (*size); i++){
		
		
		if ((*students)[i].date.day == day && (*students)[i].date.month == month)
		{
			
			array_students[j] = i;
			j++;
			counter_students++;
		}
	}
	if (counter_students == 0)
	{
		
		printf("Студентів з такою датою народження не виявлено!\n");
		return;
	}
	else print_search(students, array_students, counter_students);
}

void search_grade(struct student **students, int* size)
{
	int i, j = 0, ming, maxg, counter_students = 0;
	int array_students[50];
	
	printf("Введіть мінімальну межу діапазону: ");
	scanf("%d", &ming);
	fflush(stdin);
	
	while(ming < 0 || ming > 100)
	{
		
		printf("Помилка! Число має бути від 0 до 100. Повторіть введення: ");
		scanf("%d", &ming);
		fflush(stdin);
		
	}
	
	printf("Введіть максимальну межу діапазону: ");
	scanf("%d", &maxg);
	fflush(stdin);
	
	while(maxg < 0 || maxg > 100)
	{
		
		printf("Помилка! Число має бути від 0 до 100. Повторіть введення: ");
		scanf("%d", &maxg);
		fflush(stdin);
		
	}
	
	for (i = 0; i < (*size); i++)
	{
		
		if (ming <= (*students)[i].grade && maxg >= (*students)[i].grade)
		{
			
			array_students[j] = i;
			j++;
			counter_students++;
		}
	}
	if (counter_students == 0)
	{
		
		printf("Таких студентів не існує!\n");
		return;
	}
	else print_search(students, array_students, counter_students);
}
	
void print_search(struct student **students, int array_print[50], int size)
{
	int i;
	printf("Номер\tПрізвище\tІм'я\t\tПо-батькові\tДата народження\tСередня успішність\n");
	for (i = 0; i < size; i++)
	{
		
		if (strlen((*students)[array_print[i]].secondname) >= 8 && strlen((*students)[array_print[i]].name) >= 8 && strlen((*students)[array_print[i]].middle_name) >= 8) printf("%d\t\t%s\t%s\t%s\t%02d.%02d.%d\t%d\n", i + 1, (*students)[array_print[i]].secondname, (*students)[array_print[i]].name, (*students)[array_print[i]].middle_name, (*students)[array_print[i]].date.day, (*students)[array_print[i]].date.month, (*students)[array_print[i]].date.year, (*students)[array_print[i]].grade);
		else if(strlen((*students)[array_print[i]].secondname) >= 8 && strlen((*students)[array_print[i]].name) >= 8 && strlen((*students)[array_print[i]].middle_name) < 8) printf("%d\t\t%s\t%s\t%s\t\t%02d.%02d.%d\t%d\n", i + 1, (*students)[array_print[i]].secondname, (*students)[array_print[i]].name, (*students)[array_print[i]].middle_name, (*students)[array_print[i]].date.day, (*students)[array_print[i]].date.month, (*students)[array_print[i]].date.year, (*students)[array_print[i]].grade);
		else if(strlen((*students)[array_print[i]].secondname) >= 8 && strlen((*students)[array_print[i]].name) < 8 && strlen((*students)[array_print[i]].middle_name) < 8) printf("%d\t\t%s\t%s\t\t%s\t\t%02d.%02d.%d\t%d\n", i + 1, (*students)[array_print[i]].secondname, (*students)[array_print[i]].name, (*students)[array_print[i]].middle_name, (*students)[array_print[i]].date.day, (*students)[array_print[i]].date.month, (*students)[array_print[i]].date.year, (*students)[array_print[i]].grade);
		else if(strlen((*students)[array_print[i]].secondname) < 8 && strlen((*students)[array_print[i]].name) < 8 && strlen((*students)[array_print[i]].middle_name) < 8) printf("%d\t\t%s\t\t%s\t\t%s\t\t%02d.%02d.%d\t%d\n", i + 1, (*students)[array_print[i]].secondname, (*students)[array_print[i]].name, (*students)[array_print[i]].middle_name, (*students)[array_print[i]].date.day, (*students)[array_print[i]].date.month, (*students)[array_print[i]].date.year, (*students)[array_print[i]].grade);
		else if(strlen((*students)[array_print[i]].secondname) < 8 && strlen((*students)[array_print[i]].name) < 8 && strlen((*students)[array_print[i]].middle_name) >= 8) printf("%d\t\t%s\t\t%s\t\t%s\t%02d.%02d.%d\t%d\n", i + 1, (*students)[array_print[i]].secondname, (*students)[array_print[i]].name, (*students)[array_print[i]].middle_name, (*students)[array_print[i]].date.day, (*students)[array_print[i]].date.month, (*students)[array_print[i]].date.year, (*students)[array_print[i]].grade);
		else if(strlen((*students)[array_print[i]].secondname) < 8 && strlen((*students)[array_print[i]].name) >= 8 && strlen((*students)[array_print[i]].middle_name) >= 8) printf("%d\t\t%s\t\t%s\t%s\t%02d.%02d.%d\t%d\n", i + 1, (*students)[array_print[i]].secondname, (*students)[array_print[i]].name, (*students)[array_print[i]].middle_name, (*students)[array_print[i]].date.day, (*students)[array_print[i]].date.month, (*students)[array_print[i]].date.year, (*students)[array_print[i]].grade);
		else if(strlen((*students)[array_print[i]].secondname) >= 8 && strlen((*students)[array_print[i]].name) < 8 && strlen((*students)[array_print[i]].middle_name) >= 8) printf("%d\t\t%s\t%s\t\t%s\t%02d.%02d.%d\t%d\n", i + 1, (*students)[array_print[i]].secondname, (*students)[array_print[i]].name, (*students)[array_print[i]].middle_name, (*students)[array_print[i]].date.day, (*students)[array_print[i]].date.month, (*students)[array_print[i]].date.year, (*students)[array_print[i]].grade);
	}
}
