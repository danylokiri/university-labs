#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct device
{
	int number;
	char name[50];
	int year;
	float cost;
};

struct device devices[10];

int device_count = 0;

void add_device();
void del_device();
void print_device();
void find_with_year();
void find_with_cost();
void max_price();

int main()
{
	system("chcp 1251"); 
    system("cls"); 
    
	printf("Практичне завдання 4\nКірієнко Данило\n");    
    
    while(1)
	{
 	     printf("1 - Додавання пристрою \n");
 	     printf("2 - Видалення пристрою\n");
 	     printf("3 - Друк списку пристроїв\n");
  	     printf("4 - Пошук (рік виготовлення) \n");
 	     printf("5 - Пошук (диапазон цін) \n");
 	     printf("6 - Пошук (найдорожчий)\n");
  	     printf("7 - Вихід  \n");
 	   
    	int menu;
    	scanf ("%d", &menu);
    	switch(menu)
		{
        	case 1:
				add_device(); break;
       		case 2:
				del_device();break;
        	case 3:
				print_device();break;
        	case 4:
				find_with_year();break;
        	case 5:
				find_with_cost();break;
       		case 6:
				max_price();break;
        	case 7:
				printf("\nПрограмма завершилась\n");
				return 0;
        default: printf("Помилка! Такого варінту не існує\n");
		}
	}
}
void add_device()
{
	int i;
	struct device d;
	printf("Введіть номер пристрою:");
	scanf("%d",&d.number);
	
	 for ( i = 0; i < device_count; i++)
	 {
        if (devices[i].number == d.number)
		{
            printf("Пристрій з таким номером вже існує!\n");
            return;
        }
    }
	printf("Введіть назву пристрою:");
	scanf("%s",d.name);
	printf("Введіть рік пристрою:");
	scanf("%d",&d.year);
	printf("Введіть ціну пристрою:");
	scanf("%f",&d.cost);
	
	  devices[device_count++] = d;
    printf("Пристрій успішно доданий!\n");
}

void del_device(){
	int number;
	printf("Введіть інвентарний номер пристрою, який потрібно видалити:");
	scanf("%d",&number);
	
	int i,j;
	for(i=0;i<device_count;i++){
		if(devices[i].number== number){
		for(j=0;j<device_count-1;j++){
			devices[i]=devices[j+1];
		}
		device_count--;
		 printf("Пристрій видалений!\n");
            return;
        }
	}
	 printf("Помилка! Пристрою з таким номером не існує\n");
}
void print_device(){
	int i;
	  printf("Номер  Назва\t Рік\t Ціна\n");
	for (i=0;i<device_count;i++){
		printf("%d\t %s\t %d\t %.2f",devices[i].number,devices[i].name,devices[i].year,devices[i].cost);
		printf("\n");
	}
	
	if(device_count ==0){
		printf("Список пуст!\n");
	}
}
void find_with_year()
{
	int year;
	printf("Введіть рік пристрою, який потрібно знайти:");
	scanf("%d",&year);
	int i;
		int found = 0;
    for ( i = 0; i < device_count; i++)
	{
		if(devices[i].year==year)
		{
			printf("%d\t %s\t %d\t %.2f",devices[i].number,devices[i].name,devices[i].year,devices[i].cost);
			found = 1;
		}
	}
		if (!found) 
		{
			printf("Такий рік не знайдено!\n");
		}

}
void find_with_cost()
{
    float min_cost,max_cost;
    int i;
    printf("\nВведіть мінімальну ціну: ");
    scanf("%f", &min_cost);
    printf("Введіть максимальну ціну: ");
    scanf("%f", &max_cost);
	int found = 0;
    for ( i = 0; i < device_count; i++)
	{
        if (devices[i].cost >= min_cost && devices[i].cost <= max_cost) {
            	printf("%d\t %s\t %d\t %.2f",devices[i].number,devices[i].name,devices[i].year,devices[i].cost);
            	printf("\n");
            found = 1;
        }
    }

    if (!found) {
        printf("Приладів з такою ціною не знайдено.\n");
    }
}
void max_price()
{
	float max_cost =0.0;
	 int max_index = -1;
	 int i;
    for (i = 0; i < device_count; i++) 
	{
        if (devices[i].cost > max_cost) 
		{
            max_cost = devices[i].cost;
            max_index = i;
        }
    }

   if (max_index != -1) 
   {
   	 printf("\nНайдорожчий прилад:\n");
        	printf("%d\t %s\t %d\t %.2f",devices[max_index].number,devices[max_index].name,devices[max_index].year,devices[max_index].cost);
    }
    else 
	{
    	printf("Приладів не знайдено.\n");      
	}
}

