#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void){
	int n, k, a, d, res, flag; 
	
	printf("enter N: ");
	scanf("%d", &n);
	printf("enter k: ");
	scanf("%d", &k);
	
	if(2<=k && k<=16){
		for(;d>0;n=d){
			d=n/k;
			a=n%k;
			printf("%d",a);
	}
		printf("%d",a);
}
	else{
		printf("error!please write number 2-16");
	}
	
	return 0;
}
