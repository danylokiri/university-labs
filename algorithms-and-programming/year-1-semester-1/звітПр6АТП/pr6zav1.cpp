#include <stdio.h>
#include <math.h>

int main(){
	float a, b;
	char t;
	
	printf("enter first number:\t\n");
	scanf("%f", &a);
	printf("choose action(+;-;*;/):\t\n");
	scanf("%s", &t);
	printf("enter second number:\t\n");
	scanf("%f", &b);
	
	switch(t)
	{
		case '+':
			printf("%f+%f=%f\n", a, b, a+b);
			break;
		case '-':
			printf("%f-%f=%f\n", a, b, a-b);
			break;
		case '*':
			printf("%f*%f=%f\n", a, b, a*b);
			break;
		case '/': 
			if(b==0){
				printf("error(0 is forbidden)");
			}
			else{
				printf("%f/%f=%f\n", a, b, a/b);
			}
			break;	
	}
	
	
	return 0;
}
