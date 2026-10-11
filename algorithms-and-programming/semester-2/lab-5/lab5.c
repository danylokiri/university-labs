#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
	
	int num;
	struct Node* next;
	
}Node;

void Menu_task();
void task_1();
void task_2();
void AddInEnd(Node**, int);
void AddInStart(Node**, int);
void AddRand(Node**, int);
void print_node(Node**);
void search_max(Node**);
Node* create_Node(int);
void Menu_inc_1(Node**);
void Menu_inc_2(Node**, int);
void read_from_file(Node**, FILE*);
void write_to_file(Node**, FILE*);

int main(){
	system("chcp 1251");
	system("cls");
	printf("Кірієнко Данило\nЛаболаторна робота 5\n");
	Menu_task();
	
	return 0;
}

void Menu_task(){
	
	while(1){
		
		printf("--------Меню вибору завдання--------\n");
		
		printf("\n1 - Завдання №1\n");
		
		printf("2 - Завдання №2\n");
		
		printf("0 - Вихід\n");
	
		printf("Оберіть необхідне:\n\n");
		
		int menubar;
		scanf("%d", &menubar);
		fflush(stdin);
		
		switch(menubar){
			
			case 1:	task_1();
			break;
			
			case 2: task_2();
			break;

			case 0: printf("\nПрограма завершилася\n"); 
			return;

			default: printf("\nОбрано неіснуючий варіант. Повторіть введення\n\n");
		}
		continue;
	}
}

void task_1(){
	
	FILE* fp1;
	FILE* fp2;
	Node* head1 = NULL;
	Node* head2 = NULL;
	int i, num;
	
	Menu_inc_1(&head1);
	
	fp1 = fopen("from1.txt", "w+t");
	write_to_file(&head1, fp1);
	fclose(fp1);
	
	Node* tmp = head1;
	while (tmp != NULL) {
		
		if (tmp->num < 10) {

			AddInEnd(&head2, tmp->num);
			
		}
		tmp = tmp->next;
	}
	
	fp2 = fopen("to1.txt", "wt");
	write_to_file(&head2, fp2);
	printf("Другий список: ");
	print_node(&head2);
}

void Menu_inc_1(Node** head){
	
	while (1){
		
		printf("--------Меню Ініціалізації--------\n");
		
		printf("\n1 - Ручне введення\n");
		
		printf("2 - Випадкові значення\n");
		
		printf("3 - Читання з файлу\n");
		
		printf("4 - Виведення списку\n");
		
		printf("0 - Вихід\n");
	
		printf("Оберіть необхідне:\n\n");
		
		Node* tmp = *head;
		int menubar, i = 0, j = 0, num = -1, size;
		FILE* fp;
		
		scanf("%d", &menubar);
		fflush(stdin);
		
		switch(menubar){
			
			case 1:	
				printf("Введіть послідовність цілих чисел, коли захочете закінчити, то введіть 0\n");
				while (num != 0){
					scanf("%d", &num);
					AddInStart(head, num);
				}
			break;
			
			case 2: 
				printf("Скільки чисел ви хочете додати до списку: ");
				scanf("%d", &size);
				while (size <= 1){
					
					printf("В списку має бути більше одного числа, Повторіть введення: ");
					scanf("%d", &size);
					
				}
				while (size > j){
					AddRand(head, 30);
					j++;
				}
			break;

			case 3:
				fp = fopen("from1.txt", "rt");
				read_from_file(head, fp);
			break;
			
			case 4: 
				printf("Ваш список: ");
				print_node(head);
			break;
			
			case 0: printf("\nПрограма завершилася\n"); 
			return;

			default: printf("\nОбрано неіснуючий варіант. Повторіть введення\n\n");
		}	
	}
}

void AddInStart(Node** head, int data){
	
	Node* tmp = *head;
	
    if (tmp == NULL) {
        *head = create_Node(data);    
    } 
	else {
        Node* node = create_Node(data);
        node->next = *head;
        *head = node;
    }
}

void task_2(){
	
	FILE* fp;
	int size, i = 0, menubar;
	Node* head = NULL;
	
	
	printf("Введіть кількість груп: ");
	scanf("%d", &size);
	
	while (size < 2){
		
		printf("Кількість груп, не може бути менше 2, Повторіть введення: ");
		scanf("%d", &size);
		
	}
	
	Menu_inc_2(&head, size);
	Node* tmp = head;
	int data;	
	
	fp = fopen("base_data_2.txt", "wt");
	write_to_file(&head, fp);
	
	search_max(&head);

	print_node(&head);
}

void Menu_inc_2(Node** head, int size){
	
	while (1){
		
		printf("--------Меню Ініціалізації--------\n");
		
		printf("\n1 - Ручне введення\n");
		
		printf("2 - Випадкові значення\n");
		
		printf("3 - Читання з файлу\n");
		
		printf("4 - Виведення списку\n");
		
		printf("0 - Вихід\n");
	
		printf("Оберіть необхідне:\n\n");
		
		Node* tmp = *head;
		int menubar, i = 0, j = 0, data = 0;
		FILE* fp;
		
		scanf("%d", &menubar);
		fflush(stdin);
		
		switch(menubar){
			
			case 1:	
				while (size > i){
					printf("Введіть середній бал групи: ");
					scanf("%d", &data);
					AddInEnd(head, data);
					i++;
				}
			break;
			
			case 2: 
				while (size > j){
					AddRand(head, 100);
					j++;
				}
			break;

			case 3:
				fp = fopen("base_data_2.txt", "rt");
				read_from_file(head, fp);
			break;
			
			case 4: 
				printf("Середні бали груп: ");
				print_node(head);
			break;
			
			case 0: printf("\nПрограма завершилася\n"); 
			return;

			default: printf("\nОбрано неіснуючий варіант. Повторіть введення\n\n");
		}	
	}
}

Node* create_Node(int data){
	
    Node* node = (struct Node*) malloc(1 * sizeof(struct Node));
    node->num = data;
    node->next = NULL;
    return node;
    
}

void write_to_file(Node **head, FILE *fp){

    Node *current = *head;

    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    
    while (current != NULL) {
        fprintf(fp, "%d\n", current->num);
        current = current->next;
    }

    fclose(fp);
}

void read_from_file(Node **head, FILE *fp){
	
    int num;

    if (fp == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }

	while (fscanf(fp, "%d", &num) == 1) {
        	
		Node *newNode = create_Node(num);
       	newNode->next = *head;
       	*head = newNode;
    		
	}
    
    Node *prev = NULL;
    Node *current = *head;
    Node *next = NULL;

    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    *head = prev;
    
    fclose(fp);
}

void AddInEnd(Node** head, int data){
	
	Node* tmp = *head;
	Node* last = *head;
	
    if (tmp == NULL) {
        *head = create_Node(data);    
    } 
	else {
		while (last->next != NULL) last = last->next; 
        Node* node = create_Node(data);
        last->next = node;
    }
}

void AddRand(Node** head, int coef){
	
	FILE* file;
	Node* tmp = *head;
	int data = rand() % coef, num = 0;

    if (tmp == NULL) {
        *head = create_Node(data);    
    } 
	else {
        Node* node = create_Node(data);
        node->next = *head;
        *head = node;
    }
    
    tmp = *head;
}

void print_node(Node** head){
	
	Node* tmp = *head;
	while (tmp != NULL){
		printf("%d ", tmp->num);
		tmp = tmp->next;
	}
	printf("\n");
	
}

void search_max(Node** head){
	
	Node* tmp = *head;
	int i = 0, max = 0, imax = 0;
	
	while (tmp != NULL){
		
		if (tmp->next == NULL){
			
			if (tmp->num > max){
			
				max = tmp->num;
				imax = i;
			
			}
			break;		
		}
		if (tmp->num > max){
			
			max = tmp->num;
			imax = i;
			
		}
		tmp = tmp->next;
		i++;	
	}
	
	printf("В %d-й групі, найбільший середній бал - %d\n", imax + 1, max);
}
