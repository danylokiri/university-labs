#include <stdio.h>
#include <math.h>

int main(void){
	system("chcp 1251");
	system("cls");
	
	int menu;
	float x, s, y=1, n, i, yx, fact=1, sum=0, e = 2.71828182846;
	
	while(1)
	{
		printf("Меню: \n");
		printf("1 - Завдання 2\n");
		printf("2 - Вихід\n");
		printf("\n");
		scanf("%d", &menu);
		
		if(menu==1)
		{	
			printf("\nВведіть число: ");
			scanf("%f", &x);
				
			for(n = 0; fabs(y)>0.0001 && n<666; n++)
			{
				for(i = 1; i<=n; i++)
					fact *=i;
				y = pow(x+2,n)/fact;
				sum+=y;
			}
			yx = pow(e, x);
			s = pow(e,-2)*(1+sum);
			printf("\nS = %f\ny(x) = %f\n\n", s, yx);
			sum = 0;
			y = 0;
		}
		if(menu==2)	break;
		if(menu!=1 && menu!=2)	printf("\nНевірне значення, будь-ласка, оберіть варіант зі списку\n\n");
	}
	
	return 0;
}
