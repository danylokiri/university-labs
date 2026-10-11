#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void){
	float sum, r, x, x1, step, k, f;
	printf("enter x min: ");   //мінімальне значення
	scanf("%f", &x);
	printf("enter x max: ");	//максимальне значення
	scanf("%f", &x1);
	printf("enter step: ");		//приріст
	scanf("%f", &step);
	
	while(x<=x1){
		if(x<1){
			f=sqrtf(2-x);
		}
		else{
			f=sqrtf(2+x);
		}
		for(k=1;k<=8;k++){
			sum+=k * sqrtf(sinf((k*x)/15));
		}
		r=f*sum;
		printf("If x = %f\t F(x)= %f\t R(x) = %f\n", x, f, r);
		x = x+step;
	}
	
	
	return 0;
}
