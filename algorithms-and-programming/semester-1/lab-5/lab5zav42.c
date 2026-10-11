#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>


void initialize_arr(int *arr);

void add_to_list(int *arr, int elem, int idx);
void remove_from_list(int *arr, int idx);
void print_list(int *arr);

bool isFull(int *arr);
bool isEmpty(int *arr);

int main()
{
    int *p_allocated_arr = (int*) malloc(20 * sizeof(int));
    initialize_arr(p_allocated_arr);
    printf("The list of inveted\n");
    int menu;
    
    while(1)
    {
        int action;

        printf("choose option: \n");
		printf("1 - Show the list\n");
		printf("2 - Add\n");
		printf("3 - Remove\n");
		printf("4 - Exit\n\n");
        scanf("%d", &menu);
        printf("\n");

        if(menu == 1) print_list(p_allocated_arr);
        if(menu == 2) 
        {
                int guest_num, pos;

                printf("enter a number: ");
                scanf("%d", &guest_num);

                printf("enter position: ");
                scanf("%d", &pos);

                add_to_list(p_allocated_arr, guest_num, pos - 1);
        }
        if(menu == 3)
        { 
                int pos;

                printf("enter position in list: ");
                scanf("%d", &pos);

                remove_from_list(p_allocated_arr, pos - 1);
        }
        if(menu == 4)   break;      
        if(menu!=1 && menu!=2 && menu!=3 && menu!=4) printf("\nerror(choose option from meny)\n\n");

         
    }

    free(p_allocated_arr);
 }
 
 void initialize_arr(int *arr)
{	for(int k = 0; k < 20; k++)
	arr[k] = -1;
}
 
 void print_list(int *arr)
 {
     int a = 1;
     for(int k = 0; k < 20; k++)
         if(arr[k] != -1)
         {
             printf("%d. %d\n", a, arr[k]);
             a++;
         }
 }

 void add_to_list(int *arr, int elem, int idx)
 {
     if(arr[idx] == -1)
     {
         arr[idx] = elem;
         printf("success!\n");
     }
 }

 void remove_from_list(int *arr, int idx)
 {
     if(arr[idx] != -1)
     {
         arr[idx] = -1;
         printf("success\n");
     }

 }

