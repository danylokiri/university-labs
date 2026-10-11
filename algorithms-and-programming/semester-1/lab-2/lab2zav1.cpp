#include <stdio.h>
#include <math.h>

int main() {
 

int number, A, a1, a2, a3, a4;

	printf("Enter a number(4 digits):\n");
	scanf("%d", &number);
	printf("Enter a number A:\n");
	scanf("%d", &A);
	
	a1 = (number-(number % 1000)) / 1000;
 
    a2 = ((number-(number % 100))-(number-(number % 1000))) / 100;
 
    a3 = ((number-(number % 10))-(number-(number % 100))) / 10;
 
    a4 = number % 10;
	if (a1+a2+a3+a4>A){
		printf("Sum is more than %d", A);
	}
	else {
		printf("sum is less than %d", A);
	}

 return 0;

}
