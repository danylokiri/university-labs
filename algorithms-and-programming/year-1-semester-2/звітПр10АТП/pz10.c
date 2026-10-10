#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node
{
	
	int num;
	struct Node* next;
	
}Node;

struct Node* create_Node(int);
void main_manu();
void menu_task_1(Node**);
void menu_task_2(Node**);
void task_1();
void task_2();
void print_node(Node**);
void add_end(Node**);
void add_start(Node**);
void sort(Node**);

int main()
{
	
	system("chcp 1251");
	system("cls");
	
	printf("Практичне завдання 10\nКірієнко Данило\n");
	
	main_manu();
	
	return;
}

void main_manu()
{
	
	while(1)
	{
		
		printf("Меню\n");
		
		printf("\n1 - Перше завдання\n");
		
		printf("2 - Друге завдання\n");
		
		printf("3 - Вихід\n");

		
		int menu;
		scanf("%d", &menu);
		fflush(stdin);
		
		switch(menu)
		{
			case 1:	task_1();
			break;
			
			case 2: task_2();
			break;

			return;

			default: printf("\nПомилка! такого варіанту не існує, спробуйте ще раз\n\n");
		}
		continue;
	}
}

void menu_task_2(struct Node** head)
{

	while(1)
	{
		
		printf("\n1 - Додати нове значення в початок списку\n");
		
		printf("2 - Додати нове значення в кінець списку\n");
		
		printf("3 - Друк усіх вузлів списку\n");
		
		printf("0 - Назад\n");
		
		int menu;
		scanf("%d", &menu);
		fflush(stdin);
		
		switch(menu)
		{
			
			case 1:	add_start(head);
			break;
			
			case 2: add_end(head);
			break;
			
			case 3:	
			if (*head == NULL)
			{
				printf("Список пустий, спочатку додайте значення та спробуйте ще раз!\n");	
				break;
			}
			else print_node(head);
			break;
			
			case 0: 
			if (*head == NULL || (*head)->next == NULL )
			{
				printf("Ви не можете покинути меню заповнення списку, бо список заповнений недостатньо!\n");	
				break;
			}
			else
			{
				printf("\nЗаповнення завершене!\n"); 
				return;
			}
			
			default: printf("\nОбрано неіснуючий варіант. Повторіть введення\n\n");
		}
		continue;
	}
}

void task_1()
{
	
	int size = 0, num = 0;
	Node* head = NULL;
	menu_task_1(&head);
	
	return;
}

Node* create_Node(int data)
{
    Node* node = (struct Node*) malloc(1 * sizeof(struct Node));
    node->num = data;
    node->next = NULL;
    return node;
}


void add_end(Node** head)
{
	
	Node* tmp = *head;
	int data;
	
	printf("Введіть значення списку: ");
	scanf("%d", &data);

	while (tmp != NULL)
	{
		
		while (data == tmp->num)
		{
					
			printf("Помилка, таке значення існує, повторіть введення: ");
			scanf("%d", &data);
				
		}
		tmp = tmp->next;	
	}
	tmp = *head;
	
    if (tmp == NULL)
	{
        *head = create_Node(data);    
    } 
	else
	{
		while (tmp->next != NULL) tmp = tmp->next;
		
        Node* node = create_Node(data);
        tmp->next = node;
    }
}

void add_start(Node** head)
{
	
	Node* tmp = *head;
	int data;
	
	printf("Введіть значення списку: ");
	scanf("%d", &data);

	while (tmp != NULL)
	{
		
		while (data == tmp->num)
		{
					
			printf("Помилка, таке значення існує, повторіть введення: ");
			scanf("%d", &data);
				
		}
		tmp = tmp->next;	
	}
	tmp = *head;
	
    if (tmp == NULL)
	{
        *head = create_Node(data);    
    } 
	else
	{
        Node* node = create_Node(data);
        node->next = *head;
        *head = node;
    }
}

void print_node(Node** head)
{
	printf("Ваш список: ");
	Node* tmp = *head;
	while (tmp != NULL)
	{
		printf("%d ", tmp->num);
		tmp = tmp->next;
	}
	printf("\n");
	
}

void AddInSelect(Node** head)
{
	
	Node* tmp = *head;
	int data = 0, search_num = 0, temp_slot = 0;
	bool cycleEnd = false;
	
	printf("Введіть значення списку яке хочете додати: ");
	scanf("%d", &data);

	while (tmp != NULL)
	{
		
		while (data == tmp->num)
		{
					
			printf("Помилка, таке значення існує, повторіть введення: ");
			scanf("%d", &data);
				
		}
		tmp = tmp->next;	
	}
	tmp = *head;
	
	printf("Введіть значення після якого хочете вставити нове: ");
	scanf("%d", &search_num);
	
	print_node(head);
	
	while (!cycleEnd)
	{
		while (tmp->next != NULL)
		{
		
			if (search_num == tmp->num)
			{
				
				cycleEnd = true;
				break;	
				
			}
			tmp = tmp->next;	
			
		}
		if (cycleEnd = true) break;
		temp_slot = search_num;
		while (search_num == temp_slot)
		{
			
			printf("Такого числа в списку не знайдено, повторіть введення: "); 
			scanf("%d", &search_num);
			
		} 
	}

    Node* node = create_Node(data);
    node->next = tmp->next;
    tmp->next = node;
}


void search_num(Node** head)
{
	
	Node* tmp = *head;
	int search_num = 0;
	
	printf("Введіть значення яке хочете знайти в списку: ");
	scanf("%d", &search_num);
	
	while (tmp != NULL)
	{
		
		if (tmp->num == search_num)
		{
			
			printf("Значення присутнє в списку\n");
			break;	
		}
		tmp = tmp->next;
	}
	if (tmp == NULL) printf("Значення відсутнє в списку\n");
}

void menu_task_1(struct Node** head)
{

	while(1){
		
		printf("Меню 1\n");
		
		printf("\n1 - Додати нове значення в початок списку\n");
		
		printf("2 - Додати нове значення в кінець списку\n");
		
		printf("3 - Додати нове значення після вказаної позиції\n");
		
		printf("4 - Друк усіх вузлів списку\n");
		
		printf("5 - Пошук значення списку\n");
		
		printf("0 - Назад\n");
	
		printf("Оберіть необхідне:\n\n");
		
		int menu;
		scanf("%d", &menu);
		fflush(stdin);
		
		switch(menu)
		{
			
			case 1:	add_start(head);
			break;
			
			case 2: add_end(head);
			break;
			
			case 3:	
			if (*head == NULL)
			{
				printf("Список пустий, спочатку додайте значення та спробуйте ще раз!\n");	
				break;
			}
			else AddInSelect(head);
			break;
			
			case 4:	
			if (*head == NULL)
			{
				printf("Список пустий, спочатку додайте значення та спробуйте ще раз!\n");	
				break;
			}
			else print_node(head);
				break;
			
			case 5:	
			if (*head == NULL)
			{
				printf("Список пустий, спочатку додайте значення та спробуйте ще раз!\n");	
				break;
			}
			else search_num(head);
				break;
			
			case 0: printf("\nВиконання завдання завершене\n"); 
				return;
			
			default: printf("\nОбрано неіснуючий варіант. Повторіть введення\n\n");
			getchar();
		}
		continue;
	}
}

void task_2()
{
	
	int size = 0, num = 0;
	Node* head1 = NULL;
	
	printf("Заповніть 1 список:\n");
	menu_task_2(&head1);
	printf("Відсортований 1 список:\n");
	sort(&head1);
	print_node(&head1);
	Node* head2 = NULL;
	
	printf("Заповніть 2 список:\n");
	menu_task_2(&head2);
	printf("Відсортований 2 список:\n");
	sort(&head2);
	print_node(&head2);
	Node* last1 = head1;
	
	while (last1->next != NULL) last1 = last1->next;
	last1->next = head2;
	printf("Загальний список:\n");
	print_node(&head1);
	sort(&head1);
	printf("Відсорторований загальний список:\n");
	print_node(&head1);
	
	return;
}

void swapNodes(Node* a, Node* b)
{
	
    int temp = a->num;
    
    a->num = b->num;
    b->num = temp;
}

void sort(Node** head)
{
	
    int swapped;
    Node* ptr1;
    Node* lptr = NULL;
    
    if (head == NULL)
	{
        return;
    }
    do
	{
        swapped = 0;
        ptr1 = *head;
        while (ptr1->next != lptr)
		{
            if (ptr1->num > ptr1->next->num) {
                swapNodes(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
	while (swapped);
}

