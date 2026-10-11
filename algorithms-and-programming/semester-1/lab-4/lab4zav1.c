#include <stdio.h>
#include <math.h>

int main(void){
	int i;
	float a, b, d, res, x=0, ab;
	for(a = 1; a<1000 ;a++){
		for(b = 1; b<1000000 && b>0;b++){	//беремо за макс б = 1000000, бо там будуть повторюваться ті самі числа тільки з більшою кількістю 0
	
			d=b;
			for(x=0; d>=1; x++)		//рахуємо зі скількох цифр складається число
			{
				d=d/10;
			}
			res = pow(a,2)*pow(10,x)+b;		//приставляємо а к б
			ab = a*b;
			
			if(res/ab==3)
			printf("a = %.f\tb = %.f\n", a, b);
		}
	}
	return 0;
}
