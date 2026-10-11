#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	system("chcp 1251");
	system("cls");
	
	char str[1000], word[1000], last_word[1000];
	int i = 0, j = 0, k = 0, lenght, menu;
	
	printf("Практична 2\nТема: Символьні рядки\n");
	while(1)
	{
		printf("\nОберіть варіант:\n1 - Виконати завдання\n2 - Вийти\n\n");
		scanf("%d", &menu);
		
		if(menu == 1)
		{
			getchar();
			printf("\nВведіть рядок зі слів, складених через пробіли, довжиною до 100 символів:\n");
			fgets( str, 1000, stdin);
		//	printf("\n");
	
			for (i = 0; str[i] != '\0'; i++)
			{
				if(str[i]==' ')
				{
					word[k] = '\0';
					k =  0;
					if(strcmp(word, last_word)!= 0)
					{
						lenght = strlen(word);
						if(lenght % 2 != 0)
						{
							for(j = 0; j < lenght; j++)
							{
								if(j != lenght/2)
								{
									printf("%c", word[j]);
								}
							}
							printf(" ");
						}
						else  printf("%s ", word);
					}
				}
				else word[k++] = str[i];
			}
	
			if (strcmp(word, last_word) != 0) {
     		   lenght = strlen(word);
     		   if (lenght % 2 != 0)
				{
      		      for (j = 0; j < lenght; j++)
					{
        		        if (j != lenght/2)
						{
       		             printf("%c", word[j]);
        		        }
          		  }
       			}
				else 
           		printf("%s", word);
   			}

		}
		else exit(1);
	}
	return 0;
}
