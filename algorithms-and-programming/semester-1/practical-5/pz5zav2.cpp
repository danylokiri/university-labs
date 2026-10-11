#include <stdio.h>
#include <math.h>

int main() {
	float x,y,doble;
	
	printf("Enter x coordinates:\t");
	scanf("%f",&x);
	printf("Enter y coordinates:\t");
	scanf("%f", &y);
	
	doble = pow(x, 2) + pow(y,2);
	
	if (fabs(y) + fabs(x) > sqrtf(0.5) && doble <= 1){
		printf("The point belongs to the figure A");
	}
	else {
		printf("The point is outside of the figure A");
	}
	
	return 0;
	
}
