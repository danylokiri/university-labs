#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
	float N, i;
	double x=1.0;
	
	printf("Enter N:\t");
	scanf("%f", &N);
	if(N<=0){
		printf("erron(N<0)");
	}
	else{		
		while(N>0){
			i=sin(N*x);
			N=N-1;			
		}
	}
	
	printf("sum=\t%.4f", i);
  
    return 0;
}
