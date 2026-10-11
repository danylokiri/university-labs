#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void print_arr(int C[11][8]);

int main()
{
	int menu;
	int C[11][8];
	printf("Zavdannya 1 Lab 6\n");
	while(1)
	{
		printf("\nchoose option: \n 1 - Start\n2 - Exit\n");
		scanf("%d", &menu);
		
		if(menu == 1)
		{
			printf("\nstart massive: \n")
			print_arr(C);
		}
		else if(menu == 2)	break;
		else printf("error (write a value from the list)");
	}	
}

void print_arr(int C[11][8])
{
	for(int i = 0; i < 11; i++)
	{
		for(int j = 0; j < 8; j++)
		printf("%d", C[i][j]);
	}
}
