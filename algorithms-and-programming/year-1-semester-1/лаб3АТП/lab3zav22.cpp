#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(){
	
	int n, a, sum;
	printf("enter the numbers: ");
	scanf("%d", &n);
	
	for(;;){
		scanf("%d ", &a);
		
		if(a=0){
			printf("sum = %d", sum);
			break;
		}
		else{
			if(n=a){
				sum+=n+a;
		}
			else{
				n=a;
		}
		}
		
	}
	
	return 0;
}
