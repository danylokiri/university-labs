#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct student
{
	char second_name[15];
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

void menu_main(struct student** students, int*);
void menu_search(struct student** students, int*);
void menu_clas(struct student **students, int*);
void add_student(struct student** students, int*);
void del_student(struct student** students, int*);
void print_list(struct student** students, int*);
void search_print_students(struct student** students, int*, int);
void search_second_name(struct student** students, int*);
void search_date(struct student** students, int*);
void search_grade(struct student** students, int*);
void first_class(struct student **students, int*);
void second_class(struct student **students, int*);
void third_class(struct student **students, int*);
void sort_students(struct student** students, int*);


int main()
{
	
	system("chcp 1251");
	system("cls");
	
	printf("Лабалоторна робота 3\nКірієнко Данило\n");
	
	int size = 0;
	struct student *students = (struct student*)malloc (size * sizeof(struct student));
	
	menu_main(&students, &size);
	return 0;
}

void menu_main(struct student **students, int* size)
{
	
	while(1)
	{
		
		printf("Меню\n");
		
		printf("\n1 - Додати студента\n");
		
		printf("2 - Пошук студента\n");
		
		printf("3 - Видалити студента\n");
		
		printf("4 - Класифікація за категоріями\n");
		
		printf("5 - Друк списку групи\n");
		
		printf("6 - Вихід\n");
		
		int menubar;
		scanf("%d", &menubar);
		fflush(stdin);
		
		switch(menubar)
		{
			case 1:	add_student(students, size);
				break;
			
			case 2: 
			if((*size) == 0)
			{
				printf("список студентів порожній\n");
				break;
			}
			else menu_search(students, size);
				break;
			
			case 3:
			if((*size) == 0)
			{
				printf("список студентів порожній\n");
				break;
			}
			else del_student(students, size);
			break;
			
			case 4:
			if((*size) == 0)
			{
				printf("список студентів порожній\n");
				break;
			}
			else menu_clas(students, size);
				break;
			
			case 5:
			if((*size) == 0)
			{
				printf("список студентів порожній\n");
				break;
			}
			else print_list(students, size);
				break;
			
			case 6:
				exit(0);
			
			default: printf("\nОбрано неіснуючий варіант. Повторіть введення\n\n");
			getchar();
		}
	}
}

void menu_search(struct student **students, int* size)
{
	
	while(1)
	{
		
		printf("Пошук\n");
		
		printf("\n1 - Пошук прізвищем\n");
		
		printf("2 - Пошук за успішностю\n");
		
		printf("3 - Пошук за датою народження\n");
		
		printf("4 - Назад\n\n");
		
		int menubar;
		scanf("%d", &menubar);
		fflush(stdin);
		
		switch(menubar)
		{
			case 1:
				search_second_name(students, size);
				break;
			
			case 2:
				search_grade(students, size);
				break;
			
			case 3:
				search_date(students, size);
				break;

			case 4:
				return;

			default: printf("\nПомилка!Цього варінту не існує\n\n");
			getchar();
		}
		continue;
	}
}

void menu_clas(struct student **students, int* size)
{

	while(1)
	{
		
		printf("Класифікація\n");
		
		printf("\n1 - Високий бал\n");
		
		printf("2 - Низький бал\n");
		
		printf("3 - Без заліку\n");
		
		printf("0 - Назад\n");
		
		int menubar;
		scanf("%d", &menubar);
		fflush(stdin);
		
		switch(menubar)
		{
			 
			return;
			break;
			
			case 1:	first_class(students, size);
			break;
			
			case 2: second_class(students, size);
			break;
			
			case 3: third_class(students, size);
			break;
			
			default: printf("\nПомилка!Цього варінту не існує\n\n");
			getchar();
		}
		continue;
	}
}

void print_list(struct student **students, int* size)
{

	FILE *file;
	int i = 0;
	
	if ((file = fopen("common.dat", "rb")) != NULL) fread(&students, sizeof(struct student), (*size), file);
	else
	{
		printf("Помилка! Не вдалося зчитати файл\n");
		return;
	}
	fclose(file);

	printf("Номер\tПрізвище\tІм'я\t\tПо-батькові\tДата народження\tСередня успішність\n");
	
	for (i = 0; i < (*size); i++)
	{
		
		if (strlen((*students)[i].second_name) >= 8 && strlen((*students)[i].name) >= 8 && strlen((*students)[i].middle_name) >= 8) printf("%d\t\t%s\t%s\t%s\t%02d.%02d.%d\t%d\n", i + 1, (*students)[i].second_name, (*students)[i].name, (*students)[i].middle_name, (*students)[i].date.day, (*students)[i].date.month, (*students)[i].date.year, (*students)[i].grade);
		else if(strlen((*students)[i].second_name) >= 8 && strlen((*students)[i].name) >= 8 && strlen((*students)[i].middle_name) < 8) printf("%d\t\t%s\t%s\t%s\t\t%02d.%02d.%d\t%d\n", i + 1, (*students)[i].second_name, (*students)[i].name, (*students)[i].middle_name, (*students)[i].date.day, (*students)[i].date.month, (*students)[i].date.year, (*students)[i].grade);
		else if(strlen((*students)[i].second_name) >= 8 && strlen((*students)[i].name) < 8 && strlen((*students)[i].middle_name) < 8) printf("%d\t\t%s\t%s\t\t%s\t\t%02d.%02d.%d\t%d\n", i + 1, (*students)[i].second_name, (*students)[i].name, (*students)[i].middle_name, (*students)[i].date.day, (*students)[i].date.month, (*students)[i].date.year, (*students)[i].grade);
		else if(strlen((*students)[i].second_name) < 8 && strlen((*students)[i].name) < 8 && strlen((*students)[i].middle_name) < 8) printf("%d\t\t%s\t\t%s\t\t%s\t\t%02d.%02d.%d\t%d\n", i + 1, (*students)[i].second_name, (*students)[i].name, (*students)[i].middle_name, (*students)[i].date.day, (*students)[i].date.month, (*students)[i].date.year, (*students)[i].grade);
		else if(strlen((*students)[i].second_name) < 8 && strlen((*students)[i].name) < 8 && strlen((*students)[i].middle_name) >= 8) printf("%d\t\t%s\t\t%s\t\t%s\t%02d.%02d.%d\t%d\n", i + 1, (*students)[i].second_name, (*students)[i].name, (*students)[i].middle_name, (*students)[i].date.day, (*students)[i].date.month, (*students)[i].date.year, (*students)[i].grade);
		else if(strlen((*students)[i].second_name) < 8 && strlen((*students)[i].name) >= 8 && strlen((*students)[i].middle_name) >= 8) printf("%d\t\t%s\t\t%s\t%s\t%02d.%02d.%d\t%d\n", i + 1, (*students)[i].second_name, (*students)[i].name, (*students)[i].middle_name, (*students)[i].date.day, (*students)[i].date.month, (*students)[i].date.year, (*students)[i].grade);
		else if(strlen((*students)[i].second_name) >= 8 && strlen((*students)[i].name) < 8 && strlen((*students)[i].middle_name) >= 8) printf("%d\t\t%s\t%s\t\t%s\t%02d.%02d.%d\t%d\n", i + 1, (*students)[i].second_name, (*students)[i].name, (*students)[i].middle_name, (*students)[i].date.day, (*students)[i].date.month, (*students)[i].date.year, (*students)[i].grade);
	
	}
}

void del_student(struct student **students, int* size)
{
	
	FILE *file;
	int i, number_student, counter = 0;
	
	if ((file = fopen("common.dat", "rb")) != NULL) fread(&students, sizeof(struct student), (*size), file);
	else
	{
		printf("Помилка! Не вдалося зчитати файл\n");
		return;
	}
	fclose(file);
	printf("Введіть номер студента якого хочете вилучити: ");
	scanf("%d", &number_student);
	fflush(stdin);
	number_student--;
	
	while (number_student < 0 || number_student > (*size))
	{
		
		printf("Такого студента не існує, Повторіть введення: ");
		scanf("%d", &number_student);
		fflush(stdin);
		
	}
	
	for (i = number_student; i < (*size) - 1; i++) (*students)[i] = (*students)[i + 1];
	printf("Студента видалено\n");
	(*size)--;
	*students = realloc((*students), (*size) * sizeof(struct student));
	if((file = fopen("common.dat", "wb")) != NULL)
		fwrite(&students, sizeof(struct student), (*size), file);
	else
	{
		printf("Помилка! Не вдалося зчитати файл\n");
		return;
	}
	fclose(file);
}

void add_student(struct student **students, int* size)
{
	
	FILE *file;
	int i;
	char temporary_string[15];
	(*students) = realloc ((*students), ((*size) + 1) * sizeof(struct student));

	printf("Введіть прізвище студента(макс 15 символів): ");
	scanf("%s", &(*students)[(*size)].second_name);
	fflush(stdin);
	
	printf("Введіть ім'я студента(макс 15 символів): ");
	scanf("%s", &(*students)[(*size)].name);
	fflush(stdin);
	
	printf("Введіть по-батькові студента(макс 15 символів): ");
	scanf("%s", &(*students)[(*size)].middle_name);
	fflush(stdin);
	
	printf("Введіть дату народження студента(в форматі дд.мм.рррр): ");
	scanf("%02d.%02d.%d", &(*students)[(*size)].date.day, &(*students)[(*size)].date.month, &(*students)[(*size)].date.year);
	fflush(stdin);
	
	while((*students)[(*size)].date.year > 2023 || (*students)[(*size)].date.year < 1980)
	{
		
		printf("Помилка! Рік має бути числом від 1980 до 2023. Спробуйте ще раз:  ");
		scanf("%d", &(*students)[(*size)].date.year);
		fflush(stdin);
		
	}

	
	while((*students)[(*size)].date.month > 12 || (*students)[(*size)].date.month < 1)
	{
		
		printf("Помилка! Місяць має бути числом від 1 до 12. Спробуйте ще раз: ");
		scanf("%02d", &(*students)[(*size)].date.month);
		fflush(stdin);
		
	}

	while((*students)[(*size)].date.day > 31 && (*students)[(*size)].date.month == 1 || (*students)[(*size)].date.day > 31 && (*students)[(*size)].date.month == 3 || (*students)[(*size)].date.day > 31 && (*students)[(*size)].date.month == 5 || (*students)[(*size)].date.day > 31 && (*students)[(*size)].date.month == 7 || (*students)[(*size)].date.day > 31 && (*students)[(*size)].date.month == 8 || (*students)[(*size)].date.day > 31 && (*students)[(*size)].date.month == 10 || (*students)[(*size)].date.day > 31 && (*students)[(*size)].date.month == 12 || (*students)[(*size)].date.day > 30 && (*students)[(*size)].date.month == 4 || (*students)[(*size)].date.day > 30 && (*students)[(*size)].date.month == 6 || (*students)[(*size)].date.day > 30 && (*students)[(*size)].date.month == 9 || (*students)[(*size)].date.day > 30 && (*students)[(*size)].date.month == 11 || (*students)[(*size)].date.day > 28 && (*students)[(*size)].date.month == 2 && (*students)[(*size)].date.year % 4 != 0 || (*students)[(*size)].date.day > 29 && (*students)[(*size)].date.month == 2 && (*students)[(*size)].date.year % 4 == 0)
	{
		
		printf("Помилка! День має бути числом від 1 до 31(в залежностій від місяця і року може бути 30, 29, 28). Спробуйте ще раз:  ");
		scanf("%02d", &(*students)[(*size)].date.day);
		fflush(stdin);
		
	} 
	
	printf("Введіть оцінку студента від 0-100: ");
	scanf("%d", &(*students)[(*size)].grade);
	fflush(stdin);
	
	while((*students)[(*size)].grade > 100 || (*students)[(*size)].grade < 0)
	{
		
		printf("Помилка! Число має бути від 0 до 100. Спробуйте ще раз:  ");
		scanf("%d", &(*students)[(*size)].grade);
		fflush(stdin);
		
	}

	(*size)++;
	if ((*size) > 1) sort_students(students, size);
	
	if ((file = fopen("common.dat", "wb")) != NULL) fwrite(&students, sizeof(struct student), (*size), file);
	else{
		printf("Помилка! Не вдалося зчитати файл\n");
		return;
	}
	fclose(file);
}

void sort_students(struct student **students, int* size)
{
    int i, j, temporary = 0;
    char temp[15];
    
    for (i = 0; i < (*size) - 1; i++) {
        for (j = i + 1; j < (*size); j++) {
            if (strcmp((*students)[i].second_name, (*students)[j].second_name) > 0)
			{

            	strcpy(temp, (*students)[i].second_name); 
                strcpy((*students)[i].second_name, (*students)[j].second_name);
                strcpy((*students)[j].second_name, temp);

                strcpy(temp, (*students)[i].name);
                strcpy((*students)[i].name, (*students)[j].name);
                strcpy((*students)[j].name, temp);

                strcpy(temp, (*students)[i].middle_name);
                strcpy((*students)[i].middle_name, (*students)[j].middle_name);
                strcpy((*students)[j].middle_name, temp);

                temporary = (*students)[i].date.day;
                (*students)[i].date.day = (*students)[j].date.day;
                (*students)[j].date.day = temporary;
                
                temporary = (*students)[i].date.month;
                (*students)[i].date.month = (*students)[j].date.month;
                (*students)[j].date.month = temporary;
                
                temporary = (*students)[i].date.year;
                (*students)[i].date.year = (*students)[j].date.year;
                (*students)[j].date.year = temporary;

                temporary = (*students)[i].grade;
                (*students)[i].grade = (*students)[j].grade;
                (*students)[j].grade = temporary; 
            }
        }
    }
}
void first_class(struct student **students, int* size)
{
	
	FILE *file, *file_txt;
	int i, array_students[50], counter_students = 0;
	struct student tmp;
	
	if ((file_txt = fopen("first_stage.txt", "wt")) != NULL);
	else
	{
		printf("Помилка! Не вдалося зчитати файл\n");
		return;
	} 

	if ((file_txt = fopen("first_stage.txt", "a+t")) != NULL);
	else
	{
		printf("Помилка! Не вдалося зчитати файл\n");
		return;
	} 
	
	for (i = 0; i < (*size); i++)
	{
		
		if ((*students)[i].grade >= 70 && (*students)[i].grade <= 100)
		{
			
			array_students[counter_students] = i;
			tmp = (*students)[i];
			fwrite(&tmp, sizeof(struct student), 1, file_txt);	
			counter_students++;
			
		}
	}
	fclose(file_txt);
	search_print_students(students, array_students, counter_students);
	return;
}

void second_class(struct student **students, int* size)
{
	
	FILE *file, *file_txt;
	int i, array_students[50], counter_students = 0;
	struct student tmp;
	
	if ((file_txt = fopen("second_stage.txt", "wt")) != NULL);
	else
	{
		printf("Помилка! Не вдалося зчитати файл\n");
		return;
	}
	fclose(file_txt);
	
	if ((file_txt = fopen("second_stage.txt", "a+t")) != NULL);
	else
	{
		printf("Помилка! Не вдалося зчитати файл\n");
		return;
	}
	
	for (i = 0; i < (*size); i++)
	{
		
		if((*students)[i].grade >= 50 && (*students)[i].grade <= 69)
		{
			
			array_students[counter_students] = i;
			tmp = (*students)[i];
			fwrite(&tmp, sizeof(struct student), 1, file_txt);	
			counter_students++;
			
		}
	}
	fclose(file_txt);
	search_print_students(students, array_students, counter_students);
	return;
}
void third_class(struct student **students, int* size)
{
	FILE *file, *file_txt;
	int i, array_students[50], counter_students = 0;
	struct student tmp;
	
	if ((file_txt = fopen("third_stage.txt", "wt")) != NULL);
	else{
		printf("Помилка! Не вдалося зчитати файл\n");
		return;
	}
	fclose(file_txt);

	
	if ((file_txt = fopen("third_stage.txt", "a+t")) != NULL);
	else{
		printf("Помилка! Не вдалося зчитати файл\n");
		return;
	}
	for (i = 0; i < (*size); i++){
		
		if((*students)[i].grade >= 0 && (*students)[i].grade <= 49)
		{
			
			array_students[counter_students] = i;
			tmp = (*students)[i];
			fwrite(&tmp, sizeof(struct student), 1, file_txt);	
			counter_students++;
			
		}	
	}
	fclose(file_txt);
	search_print_students(students, array_students, counter_students);
	return;
}

void search_second_name(struct student **students, int* size)
{

	int i, j, k = 0, counter = 0, counter_students = 0, array_students[50];
	char second_name[15];
	
	printf("Введіть прізвище: ");
	fgets(second_name, sizeof(second_name), stdin);

	for (i = 0; i < (*size); i++)
	{
		for (j = 0; j < strlen((*students)[i].second_name); j++)
		{
			if(second_name[j] != (*students)[i].second_name[j])
			{			
				counter = 0;
				break;
					
			}	
			counter++;
		}
		
		if (counter == strlen((*students)[i].second_name))
		{
			
			array_students[k] = i;
			k++;
			counter_students++;	
			
		}
		counter = 0;
	}
		
	if (counter_students == 0)
	{
		
		printf("Такого прізвища не існує\n");
		return;
	}
	else search_print_students(students, array_students, counter_students);
}

void search_date(struct student **students, int* size)
{

	int i, j = 0, day, month, counter_students = 0;
	int array_students[50];
	
	printf("Введіть дату (дд.мм): ");
	scanf("%02d.%02d", &day, &month);
	fflush(stdin);
	
	while(month > 12 || month < 1 || day > 31 && month == 1 || day > 31 && month == 3 || day > 31 && month == 5 || day > 31 && month == 7 || day > 31 && month == 8 || day > 31 && month == 10 || day > 31 && month == 12 || day > 30 && month == 4 || day > 30 && month == 6 || day > 30 && month == 9 || day > 30 && month == 11 || day > 28 && month == 2)
	{
		
		printf("Помилка! Введене неправильне число. спробуйте ще раз:  ");
		scanf("%02d.%02d", &day, &month);
		fflush(stdin);
	}
	
	for (i = 0; i < (*size); i++)
	{
		
		if ((*students)[i].date.day == day && (*students)[i].date.month == month)
		{
			
			array_students[j] = i;
			j++;
			counter_students++;
		}
	}
	if (counter_students == 0)
	{
		
		printf("Помилка! Студентів з такою датою народження не існує\n");
		return;
	}
	else search_print_students(students, array_students, counter_students);
}

void search_grade(struct student **students, int* size)
{

	int i, j = 0, ming, maxg, counter_students = 0;
	int array_students[50];
	
	printf("Введіть мінімальну межу (від 0 до 100): ");
	scanf("%d", &ming);
	fflush(stdin);
	
	while(ming < 0 || ming > 100)
	{
		
		printf("Помилка! Введене неправильне число. Спробуйте ще раз: ");
		scanf("%d", &ming);
		fflush(stdin);
		
	}
	
	printf("Введіть максимальну межу (від 0 до 100): ");
	scanf("%d", &maxg);
	fflush(stdin);
	
	while(maxg < 0 || maxg > 100)
	{
		
		printf("Помилка! Введене неправильне число. Спробуйте ще раз: ");
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
		
		printf("Студентів, чиї оцінки є в цих межах, не виявлено!\n");
		return;
	}
	else search_print_students(students, array_students, counter_students);
}
	
void search_print_students(struct student **students, int array_print[50], int size)
{
	
	int i;

	printf("Номер\tПрізвище\tІм'я\t\tПо-батькові\tДата народження\tСередня успішність\n");
	for (i = 0; i < size; i++)
	{
		
		if (strlen((*students)[array_print[i]].second_name) >= 8 && strlen((*students)[array_print[i]].name) >= 8 && strlen((*students)[array_print[i]].middle_name) >= 8) printf("%d\t\t%s\t%s\t%s\t%02d.%02d.%d\t%d\n", i + 1, (*students)[array_print[i]].second_name, (*students)[array_print[i]].name, (*students)[array_print[i]].middle_name, (*students)[array_print[i]].date.day, (*students)[array_print[i]].date.month, (*students)[array_print[i]].date.year, (*students)[array_print[i]].grade);
		else if(strlen((*students)[array_print[i]].second_name) >= 8 && strlen((*students)[array_print[i]].name) >= 8 && strlen((*students)[array_print[i]].middle_name) < 8) printf("%d\t\t%s\t%s\t%s\t\t%02d.%02d.%d\t%d\n", i + 1, (*students)[array_print[i]].second_name, (*students)[array_print[i]].name, (*students)[array_print[i]].middle_name, (*students)[array_print[i]].date.day, (*students)[array_print[i]].date.month, (*students)[array_print[i]].date.year, (*students)[array_print[i]].grade);
		else if(strlen((*students)[array_print[i]].second_name) >= 8 && strlen((*students)[array_print[i]].name) < 8 && strlen((*students)[array_print[i]].middle_name) < 8) printf("%d\t\t%s\t%s\t\t%s\t\t%02d.%02d.%d\t%d\n", i + 1, (*students)[array_print[i]].second_name, (*students)[array_print[i]].name, (*students)[array_print[i]].middle_name, (*students)[array_print[i]].date.day, (*students)[array_print[i]].date.month, (*students)[array_print[i]].date.year, (*students)[array_print[i]].grade);
		else if(strlen((*students)[array_print[i]].second_name) < 8 && strlen((*students)[array_print[i]].name) < 8 && strlen((*students)[array_print[i]].middle_name) < 8) printf("%d\t\t%s\t\t%s\t\t%s\t\t%02d.%02d.%d\t%d\n", i + 1, (*students)[array_print[i]].second_name, (*students)[array_print[i]].name, (*students)[array_print[i]].middle_name, (*students)[array_print[i]].date.day, (*students)[array_print[i]].date.month, (*students)[array_print[i]].date.year, (*students)[array_print[i]].grade);
		else if(strlen((*students)[array_print[i]].second_name) < 8 && strlen((*students)[array_print[i]].name) < 8 && strlen((*students)[array_print[i]].middle_name) >= 8) printf("%d\t\t%s\t\t%s\t\t%s\t%02d.%02d.%d\t%d\n", i + 1, (*students)[array_print[i]].second_name, (*students)[array_print[i]].name, (*students)[array_print[i]].middle_name, (*students)[array_print[i]].date.day, (*students)[array_print[i]].date.month, (*students)[array_print[i]].date.year, (*students)[array_print[i]].grade);
		else if(strlen((*students)[array_print[i]].second_name) < 8 && strlen((*students)[array_print[i]].name) >= 8 && strlen((*students)[array_print[i]].middle_name) >= 8) printf("%d\t\t%s\t\t%s\t%s\t%02d.%02d.%d\t%d\n", i + 1, (*students)[array_print[i]].second_name, (*students)[array_print[i]].name, (*students)[array_print[i]].middle_name, (*students)[array_print[i]].date.day, (*students)[array_print[i]].date.month, (*students)[array_print[i]].date.year, (*students)[array_print[i]].grade);
		else if(strlen((*students)[array_print[i]].second_name) >= 8 && strlen((*students)[array_print[i]].name) < 8 && strlen((*students)[array_print[i]].middle_name) >= 8) printf("%d\t\t%s\t%s\t\t%s\t%02d.%02d.%d\t%d\n", i + 1, (*students)[array_print[i]].second_name, (*students)[array_print[i]].name, (*students)[array_print[i]].middle_name, (*students)[array_print[i]].date.day, (*students)[array_print[i]].date.month, (*students)[array_print[i]].date.year, (*students)[array_print[i]].grade);

	}
}
