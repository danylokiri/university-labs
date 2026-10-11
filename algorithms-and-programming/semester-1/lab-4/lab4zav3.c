#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main (void){
	system("chcp 1251");
	system("cls");
	

	float  s, x, f1=1, sum=0, sum1=0, delta, e, n, k;
	
	printf ("¬вед≥ть eпс≥лон: ");
	scanf("%f",&e);
	printf ("\n");
	
	printf("x\tF(x)\tS(x)\tdelta\n");
	
	for(x = 0.1; x<1; x=x+0.1)
	{
		for(k = 1; fabs(f1)>e && k<10; k++)		//перша с≥гма
		{

			for(n = 1; n<=k; n++)	//друга с≥гма
				sum1+=1/n;
			f1=pow(-1,k+1)*k*pow(x,k-1)*sum1;
			sum+=f1;
			sum1 = 0;
		}
	s = sum - (1-logf(1+x)/pow(1+x,2));
	delta = sum - s;
	printf("%.4f\t%.4f\t%.4f\t%.4f\n", x, sum, s, delta);
	sum = 0;
	f1=1;
	}
	return 0;
}
