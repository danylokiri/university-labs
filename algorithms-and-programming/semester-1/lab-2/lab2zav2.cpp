#include <stdio.h>
#include <math.h>

int main() {
	float x, y;

	printf("Enter x coordinates:\t");
	scanf("%f", &x);
	printf("Enter y coordinates:\t");
	scanf("%f", &y);


	if (y <= -fabs(x)) {
		printf("The point belongs to the figure");
	}
	else {
		printf("The point is outside of the figure");
	}

 return 0;

}
