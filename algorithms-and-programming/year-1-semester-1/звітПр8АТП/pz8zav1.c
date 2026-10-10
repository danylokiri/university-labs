#include <stdio.h>
#include <math.h>

int main(void){
	system("chcp 1251");
	system("cls");
	int menu, flag = 1, n;
	while(1)
	{
		printf("\nМеню: \n");
		printf("1 - Перевірка числа на простоту\n");
		printf("2 - Вихід\n");
		printf("\n");
		scanf("%d", &menu);
		
	
		if(menu==1){		
			printf("\nВведіть число: ");
			scanf("%d", &n);
			
			
				
			for(int i = 2; i < n; i++)
			{
				if(n%i==0)
				{
					flag = 0;
					break;
				}
			}
			if(flag == 0)	printf("\nЧисло складове\n");
			else	printf("\nЧисло просте\n");
		}
		if(menu==2)	break;
		if(menu!=1 && menu!=2)	printf("\nНевірне значення, будь-ласка, оберіть варіант зі списку\n");
	}
	
	return 0;
}
