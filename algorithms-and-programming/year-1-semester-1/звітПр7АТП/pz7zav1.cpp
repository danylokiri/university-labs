#include <stdio.h>
#include <math.h>

int main(void)
{
	float y, fx, e, dx, x=0;
	e=2.718;
	
	while(0<=x && x<=5){
		if(x>1)
		{
			fx = sqrtf(1+x/2);
		} else
		{
			fx = 1/(1+x/2);
		}
		y=((2 + pow(e,x))/(1 - pow(e,x))) + 3 * fx;
		
		printf("x = %f		y=%f\n", x,y);
		
	x=x+0.5;
	}
	
	
return 0; 
}

