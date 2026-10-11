#include <stdio.h>
#include <math.h>

int main(){
	
	int n,f;			//f - factorial
	f=1;
	printf("enter the number: \n");
	scanf("%d", &n);
 
	while(n>1){
		n=n-1;
		f=f * n;
	}
	printf("factorial = %d", f);
return 0; 
}

