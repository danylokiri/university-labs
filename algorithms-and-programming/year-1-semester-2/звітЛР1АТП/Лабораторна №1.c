#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int Menu(int***, char***, int*);
void print_list(int***, char***, int*);
int sum_medals(int, int***);
void sorting(int***, char***, int*);
void print_mini_list(int***, char***, int*);
void edit_list(int***, char***, int*);

int main(){
	
	system("chcp 1251");
	system("color 02");
	system("cls");
	printf("Практичне завдання 4\nКірієнко Данило\n");
	char **name_country = NULL, name[20];
	int size, i, j, *ps;
	ps = &size;
	
	printf("Введіть кількість початкових країн(1 < x <= 5): ");
	scanf("%d", &size);
	fflush(stdin);
	
	while (size < 1 || size > 5)
	{
				
		printf("Помикла при введені числа, прийнятний діапазон (0 <= x), повторіть введення правильно:");
		scanf("%d", &size);	
		fflush(stdin);
				
	}

	name_country = (char**) malloc (size * sizeof(char*));
	
	for (i = 0; i < size; i++) name_country[i] = malloc (20 * sizeof(char*));
	
	int **list_medals = NULL;
	
	list_medals = (int**) malloc (size * sizeof(int*));
	
	for (i = 0; i < size; i++){
		
		list_medals[i] = malloc (3 * sizeof(int));
		
	}

	for (i = 0; i < size; i++){
		
		printf("Введіть назву %d-ї країни(до 15 символів включно): ", i + 1);
		scanf("%s", &name[0]);
		strcpy(name_country[i], name);
		
	}
	
	for (i = 0; i < size; i++)
	{

		for (j = 0; j < 3; j++)
		{
			switch(j)
			{		
			case 0:
				printf("Введіть кількість золотих медалей %d-ї країни: ", i + 1);
				scanf("%d", &list_medals[i][0]);
				fflush(stdin);
	
				while (list_medals[i][0] < 0)
				{
					printf("Помикла при введені числа, прийнятний діапазон (0 <= x), повторіть введення правильно:");
				scanf("%d", &list_medals[i][0]);	
					fflush(stdin);	
				}
				break;
				
			case 1: printf("Введіть кількість срібних медалей %d-ї країни: ", i + 1);
			scanf("%d", &list_medals[i][1]);
			fflush(stdin);
	
			while (list_medals[i][1] < 0){
				
				printf("Помикла при введені числа, прийнятний діапазон (0 <= x), повторіть введення правильно:");
				scanf("%d", &list_medals[i][1]);	
				fflush(stdin);	
			}
			break;
				
			case 2: printf("Введіть кількість бронзових медалей %d-ї країни: ", i + 1);
			scanf("%d", &list_medals[i][2]);
			fflush(stdin);
	
			while (list_medals[i][2] < 0){
				
				printf("Помикла при введені числа, прийнятний діапазон (0 <= x), повторіть введення правильно:");
				scanf("%d", &list_medals[i][2]);	
				fflush(stdin);	
			}
			break;
				
			default: printf("Помилка, такого номеру не існує!!!");
			}
		}
	}	
	Menu(&list_medals, &name_country, ps);
	
	return 0;
}


int Menu(int ***list_medals, char ***name_country, int* size){
	
	while(1){
		
		printf("Меню\n");
		
		printf("\n1 - Змінити дані\n");
		
		printf("2 - Дані по країні\n");
		
		printf("3 - Зведена таблиця\n");
		
		printf("0 - Вихід\n");
		
		printf("Оберіть необхідне:\n\n");
		
		int menubar;
		scanf("%d", &menubar);
		fflush(stdin);
		
		switch(menubar){
			
			case 1:
					edit_list(list_medals, name_country, size);
			break;
			
			case 2:
					sorting(list_medals, name_country, size);
					print_mini_list(list_medals, name_country, size);
			break;
			
			case 3:
					sorting(list_medals, name_country, size);
					print_list(list_medals, name_country, size);
			break;
			
			case 0: printf("\nПрограма завершилась\n"); 
			return 0;
			
			default: printf("\nОбрано неіснуючий варіант. Повторіть введення\n\n");
			getchar();
		}
	}
}

int sum_medals(int number_country, int ***list_medals){
	
	int i, sum = 0;
	for (i = 0; i < 3; i++){
		
		sum += (*list_medals)[number_country][i];
		
	}
	
	return sum;
}

void sorting(int ***list_medals, char ***name_country, int *size){
	
	
	int i, j, temporary_slot = 0, k;
	char temporary_string[20];
	
	for (j = 0; j < (*size) - 1; j++){
		
		for (i = 0; i < (*size) - 1; i++){
	
			if (sum_medals(i, list_medals) < sum_medals(i + 1, list_medals))
			{
			
				strcpy(temporary_string, (*name_country)[i]);
				strcpy((*name_country)[i], (*name_country)[i + 1]);
				strcpy((*name_country)[i + 1], temporary_string);  
			
				for (k = 0; k < 3; k++){
				
					temporary_slot = (*list_medals)[i][k];
					(*list_medals)[i][k] = (*list_medals)[i + 1][k];
					(*list_medals)[i + 1][k] = temporary_slot;
				
				} 
			}
		}	
	}
}

void print_list(int ***list_medals, char ***name_country, int *size){
	
	int i;
	printf("Місце\tКраїна\t\tЗолото\tСрібло\tБронза\tУсього\n");
	
	for (i = 0; i < *size; i++){

		if (strlen((*name_country)[i]) >= 8) printf("%d\t%s\t%d\t%d\t%d\t%d\n", i + 1, (*name_country)[i], (*list_medals)[i][0], (*list_medals)[i][1], (*list_medals)[i][2], sum_medals(i, list_medals));
		else printf("%d\t%s\t\t%d\t%d\t%d\t%d\n", i + 1, (*name_country)[i], (*list_medals)[i][0], (*list_medals)[i][1], (*list_medals)[i][2], sum_medals(i, list_medals));
		
	}

}

void print_mini_list(int ***list_medals, char ***name_country, int *size){
	
	sorting(list_medals, name_country, size);
	
	int i, index_country = -1, j, counter = 0;
	char country[20];
		
	printf("Введіть назву тієї країни, статистику якої хочете переглянути: ");
	fgets(country, sizeof(country), stdin);
		
	while (index_country == -1){
		
		for (i = 0; i < (*size); i++){
			for (j = 0; j < strlen((*name_country)[i]); j++)
			{
				if(country[j] != (*name_country)[i][j])
				{
					counter = 0;
					break;
					
				}	
				counter++;
			
			}
		
			if (counter == strlen((*name_country)[i]))
			{
				index_country = i;
				break;	
			}
		}
		
		if (index_country == -1)
		{
			
			memset(country, 0, sizeof(country));
			printf("Такої країни не існує, повторіть введення: ");
			fgets(country, sizeof(country), stdin);
			
		}
	}
	
	printf("Місце\tКраїна\t\tЗолото\tСрібло\tБронза\tУсього\n");
	if (strlen((*name_country)[i]) >= 8) printf("%d\t%s\t%d\t%d\t%d\t%d\n", index_country + 1, (*name_country)[index_country], (*list_medals)[index_country][0], (*list_medals)[index_country][1], (*list_medals)[index_country][2], sum_medals(index_country, list_medals));
	else printf("%d\t%s\t\t%d\t%d\t%d\t%d\n", i + 1, (*name_country)[index_country], (*list_medals)[index_country][0], (*list_medals)[index_country][1], (*list_medals)[index_country][2], sum_medals(index_country, list_medals));	
}

void edit_list(int ***list_medals, char ***name_country, int *size){
	
	int i, j, index_country = -1, counter = 0;
	char country[20];
	
	printf("Введіть назву країни, яка присутня в списку, або ж введіть іншу щоб додати нову: ");
	scanf("%s", &country[0]);
	
	while (index_country == -1)
	{
		
		for (i = 0; i < (*size); i++)
		{

			for (j = 0; j < strlen((*name_country)[i]); j++)
			{
				if(country[j] != (*name_country)[i][j])
				{
					counter = 0;
					index_country = -2;
					break;
					
				}	
				counter++;
			}
		
			if (counter == strlen((*name_country)[i]))
			{
			
				index_country = i;
				break;	
			}
		}
	}
	
	if (index_country < 0){
		
		printf("Введеної країни немає в списку, тому вона додається\n");
		
		*name_country = (char**) realloc (*name_country, ((*size) + 1) * sizeof(char*));
		(*name_country)[(*size)] = malloc (20 * sizeof(char*));
		strcpy((*name_country)[(*size)], country);
		
		*list_medals = (int**) realloc (*list_medals, ((*size) + 1) * sizeof(int*));
		(*list_medals)[(*size)] = malloc (3 * sizeof(int*)); 
		
		printf("\nВведіть кількість золотих медалей нової країни: ");
		scanf("%d", &((*list_medals)[*size][0]));
		fflush(stdin);
		
		while ((*list_medals)[*size][0] < 0 ){
				
			printf("Помикла при введені числа, прийнятний діапазон (0 <= x), повторіть введення правильно:");
			scanf("%d", &((*list_medals)[*size][0]));	
			fflush(stdin);	
				
		}
		
		printf("Введіть кількість срібних медалей нової країни: ");
		scanf("%d", &((*list_medals)[*size][1]));
		fflush(stdin);
		
		while ((*list_medals)[*size][1] < 0 ){
				
			printf("Помикла при введені числа, прийнятний діапазон (0 <= x), повторіть введення правильно:");
			scanf("%d", &((*list_medals)[*size][1]));	
			fflush(stdin);
			
		}
		
		printf("Введіть кількість бронзових медалей нової країни: ");
		scanf("%d", &((*list_medals)[*size][2]));
		fflush(stdin);
		
		while ((*list_medals)[*size][2] < 0 ){
				
			printf("Помикла при введені числа, прийнятний діапазон (0 <= x), повторіть введення правильно:");
			scanf("%d", &((*list_medals)[*size][2]));	
			fflush(stdin);
				
		}

		(*size)++;
	}  
	else{
		
		printf("Введена країна присутня в списку, тому ви можете змінити вже наявні дані\n\n");
		printf("Введіть кількість золотих медалей %d-ї країни: ", index_country + 1);
		scanf("%d", &((*list_medals)[index_country][0]));
		fflush(stdin);
		
		while ((*list_medals)[index_country][0] < 0 ){
				
			printf("Помикла при введені числа, прийнятний діапазон (0 <= x), повторіть введення правильно:");
			scanf("%d", &((*list_medals)[index_country][0]));	
			fflush(stdin);
				
		}
		
		printf("Введіть кількість срібних медалей %d-ї країни: ", index_country + 1);
		scanf("%d", &((*list_medals)[index_country][1]));
		fflush(stdin);
		
		while ((*list_medals)[index_country][1] < 0 ){
				
			printf("Помикла при введені числа, прийнятний діапазон (0 <= x), повторіть введення правильно:");
			scanf("%d", &((*list_medals)[index_country][1]));	
			fflush(stdin);
				
		}
		
		printf("Введіть кількість бронзових медалей %d-ї країни: ", index_country + 1);
		scanf("%d", &((*list_medals)[index_country][2]));
		fflush(stdin);
		
		while ((*list_medals)[index_country][2] < 0 ){
				
			printf("Помикла при введені числа, прийнятний діапазон ");
			scanf("%d", &((*list_medals)[index_country][2]));	
			fflush(stdin);
				
		}
	}
}
