#include <stdio.h>
#include <math.h>

int main(){
	
	int n,i,f;			//f - factorial
	f = 1;
	
	printf("enter the number: \n");
	scanf("%d", &n);
	
	if(n==0){
		printf("factorial = 1");
		return 0;
	}
	if(n<0){
		printf("error(enter n>0)");
		return 0;
	}	
	for(i=1;i<=n;i++)
	{
		f = f*i;
	}
	printf("factorial = %d", f);
return 0; 
}

