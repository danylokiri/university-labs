#include <stdio.h>
#include <stdlib.h>

void random_arr(int M, int *d_arr[M]);
void handwrite_arr(int M, int *d_arr[M]);
void print_arr(int M, int *d_arr[M]);

int main()
{
	system("chcp 1251");
	system("cls");
	
	int menu, M;
	
	printf("Практична 3\nТема: Динамічні одномірні масиви\n");
	while(1)
	{
		printf("\nВиберіть варіант:\n1 - Згенерувати массив рандомно\n2 - Ввести значення для генерації масиву\n3 - Вихід\n");
		scanf("%d", &menu);
		printf("\nВиберіть кількість елементів массиву: \n\n");
		scanf("%d", &M);
		printf("\n");
		
		if(menu == 1)
	}
	
	
	
	
	return 0;
}

