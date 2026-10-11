#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main (void){
	
	system("chcp 1251");
	system("cls");
	
	float mnozh=1, r, x, xmax, step, f, e = 2.71828182846;
	int k;
	printf ("Введіть мінімальне значення х: ");
	scanf("%f",&x);
	printf ("Введіть максимальне значення х: ");
	scanf("%f",&xmax);
	printf ("Введіть крок: ");
	scanf("%f",&step);
	
	for(; x<=xmax; x=x+step)
	{
		if(x>0)
			f=3-pow(e,3*x);
		else
			f=3+pow(e,3*x);
			
		for(k=0; k<6; k++)
		{
			r=pow(1+x/2,k)+f;
			mnozh *= r;
		}
		printf("При значенні x = %.1f\tF(x) = %.2f\tR(x) = %.3f\n", x, f, mnozh);
		mnozh = 1;
	}


	return 0;
}
