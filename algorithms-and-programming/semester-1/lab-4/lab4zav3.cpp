#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void){
	double n, k, k1, k2, k3, f, s, i, fact, delta;
	double inf = INFINITY;
	int x=0;
	k1=2*k;
	k2=2*k-1;
	k3=k+1;
	printf("x\tF\tS\tdelta\n", x, f, s, delta);
	while(x>=0 && x<=0.9){
		for(i=1; i<=k1; i++){
			fact = fact * i;
		}
		f=0;
		for (k=1;;)
		{
			f+=(pow(-1,k3)*pow(2,k2)*pow(x,2*k))/fact;	
			
		}
	s = f - (pow(sin(x),2));
	delta = f - s;
	
	printf("%d\t%.6f\t%.6f%.6f", x, f, s, delta);
	x=x+0.1;
}
	return 0;
}
