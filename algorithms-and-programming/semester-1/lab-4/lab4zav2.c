#include <stdio.h>
#include <math.h>
#include <stdlib.h>


int main() { 
system("chcp 1251");
system("cls");

	double N,k;
	int i,j,a,c,b;
	printf ("¬вед≥ть N:");
	scanf("%lf",&N);
	printf ("¬вед≥ть систему численн€,в €ку хочете перевести:");
	scanf("%lf",&k);
    	
    for (i = 1,j = 1;j<=N;i++,j = pow(k,i))
        c=i;
    if (c>=0){
        for(;c>=0;c--){
    		a=N/pow(k,c);
            printf("%d",a);
            N = N - (pow(k,c)*a);
        }
    }
    else printf("Error");
    return 0;
}


