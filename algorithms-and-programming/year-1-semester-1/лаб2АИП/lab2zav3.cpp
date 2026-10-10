#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int main() {
	system("chcp 1251"); 
	system("cls");
    
	float lenght, result;
	int convertType;   
  
	printf("¬вед≥ть величину(1 Ц дециметр, 2 Ц к≥лометр, 3 Ц метр, 4 Ц м≥л≥метр, 5 Ц сантиметр):\n");
	scanf("%d", &convertType);
  
	printf("¬вед≥ть довжину:\n");
	scanf("%f", &lenght);
  
  
	switch (convertType) {
   		case 1:
    		result = lenght / 10;
    		break;
    	case 2:
    		result  = lenght * 1000;
    		break;  
   		case 3:
    		result  = lenght;
    		break;
  	 	case 4:
   		 	result  = lenght / 1000;
 		   	break;
   		case 5:
    		result  = lenght / 100;
    		break;    
    	default:
			printf("Ќекорректна величина");
			break;
  }
  printf("ƒовжина в метрах:\t%f", result );
  
 return 0;

}
