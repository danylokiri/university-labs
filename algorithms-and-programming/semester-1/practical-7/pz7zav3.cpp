#include <stdio.h>
#include <math.h>

int main(){
	
	float a,b;
	b=0;
	printf("enter a number: \n");
	scanf("%f", &a);
	
	while(1){
		scanf("%f", &b);
		
		if(b<=a){
			printf("Your money is in the fire!");
			break;
		}
		else{
			a=b;
		}
	}	
	return 0;	
}
