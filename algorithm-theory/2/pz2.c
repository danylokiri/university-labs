#include <stdio.h> 
 
int countCombinations(int amount) { 
       long count = 0;    
	   int cents50;    
	   int cents25;    
	   int cents10;    
	   int cents5; 
         for (cents50 = 0; cents50 <= amount / 50; cents50++) {        
			 for (cents25 = 0; cents25 <= amount / 25; cents25++) {            
				 for (cents10 = 0; cents10 <= amount / 10; cents10++) {                 
					 for (cents5 = 0; cents5 <= amount / 5; cents5++) {                     
		 				int cents1 = amount - (cents50 * 50 + cents25 * 25 + cents10 * 10 + cents5 * 5);                     
						 if (cents1 >= 0) {      
							 count++; 
     					   } 
             		   } 
          			} 
        	}   
		} 
    return count; 
} 
 int main() {     
 printf("кірієнко кс 24\nпрактичне завдання 2\n");  
 system("chcp 1251");       
 system("cls"); 
    
	int amount;    
	printf("Введіть суму в центах: ");    
	scanf("%d", &amount); 
 
    int combinations = countCombinations(amount); 
 
    printf("Кількість комбінацій розміну: %d\n", combinations); 
 
    return 0; 
} 
