#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main(){
	
	int n, a, sum;
	printf("enter the numbers: ");
	scanf("%d", &n);
	
	while(n!=0){
		scanf("%d ", &a);
		
	
			if(n=a){
			sum+=n+a;
		}
		else{
			n=a;
		}
		
		
	}
	
	return 0;
}
