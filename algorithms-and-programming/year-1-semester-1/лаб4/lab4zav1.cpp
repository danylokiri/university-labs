#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(void){
	
	int m, n, res;
	
	printf("enter m: ");
	scanf("%d", &m);
	printf("enter n: ");
	scanf("%d", &n);
	
	for(; m<=n; m++){
		res = m%9;
	//	printf("res = %d\t",res);
		printf("m = %d\t",m);
		if(res!=0){
			printf("divide is not entirely\n");
		}
		else{
			printf("divide entirely\n");
		}
		
	}
	return 0;
}
