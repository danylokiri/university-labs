#include <stdio.h>
#include <math.h>
#include <stdbool.h>

int main (){ 
	int x;
	float a, b, result = 1, numerator, denominator;
	int flag = true;

	for (x = 0; flag==true; x++ ){
    	a = b;
    	numerator = (1 + (2 * x));
    	denominator = (2 + (2 * x));
    	b = numerator / denominator;
    	result *= b;
    	if(b-a <= pow(10, -4)) {
      		printf("the end - amout of operations %d\n", x);
  			flag = false;
    }
  }
  printf("product = %f\n", result);
  return 0;
}

