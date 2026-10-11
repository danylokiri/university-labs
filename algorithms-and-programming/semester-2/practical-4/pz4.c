#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>
#include <string.h>

void handwrite_arr(int N,int M);
void random_arr(int N,int M);
void add_line(int N,int M,int d);
void del_line(int N,int M,int d);

int main ()
{
	system("chcp 1251 && cls");
	srand(time(NULL));
	
	int *a,N,M,menu, menu1,d,o;
	
	printf("Практичне завдання 4\nКірієнко Данило\n");
	while(1)
	{
		printf("\n1 - Заповнення вручну\n2 - Заповнення рандомно\n3 - Додати рядок\n4 - Видалити рядок\n5 - Вихід\n\n");
		scanf ("%d", &menu);
		
		switch(menu)
		{
  			case 1:
    			printf("Введіть кількість рядків:\n");
    			scanf("%d",&N);
    			printf("Введіть кількість :\n");
    			scanf("%d",&M);
    			handwrite_arr(N,M);
  				break;
  			case 2:
  				printf("Введіть кількість рядків:\n");
    			scanf("%d",&N);
    			printf("Введіть кількість :\n");
    			scanf("%d",&M);
    			random_arr(N,M);
    			break;
    		case 3:
    			printf("\nДодати рядок\n");
    			printf("Виберіть позицію рядка\n");
				scanf("%d",&d);
    			add_line(N,M,d);
  				break;
  			case 4:
  				printf("\nВидалити рядок\n");
    			printf("Додати позицію рядка\n");
    			scanf("%d",&d);
    			del_line(N,M,d);
  				break;
  			case 5: 
        		return 0;
   			default: 
        		printf("помилка 404\n");
		}
		continue;
	}
	return 0;
}

void random_arr(int N,int M)
{
	int i,j,**p = NULL;
	srand(time(NULL));
	p = (int**)malloc(N*sizeof(int*));
	
	for(i=0;i<N;i++)
		*(p+i) = (int*)malloc(M*sizeof(int));
		
	for(i=0; i< N; i++){
		for (j = 0; j <M; j++) {
				*(*(p+i)+j)= rand() % 10;
      		}
	}
	
	for(i=0; i< N; i++){
		for (j = 0; j < M; j++){
			printf("%d\t", p[i][j]);
		}
		printf("\n");
	}	
	
	for(i=0;i< N;i++)
		free(*(p+i));
		
	free(p);
}

void handwrite_arr(int N,int M)
{
int i,j,**p = NULL;
	p = (int**)malloc(N*sizeof(int*));
	
	for(i=0;i<N;i++)
		*(p+i) = (int*)malloc(M*sizeof(int));
		
	for(i=0; i< N; i++){
		for (j = 0; j <M; j++) {
				printf("p[%d][%d] = ", i, j);
      			scanf("%d",(*(p+i)+j));
      		}
	}
	
	for(i=0; i< N; i++){
		for (j = 0; j < M; j++){
			if(*(*(p+i)+j)<10 && *(*(p+i)+j) >=0)
				printf("%d\t", p[i][j]);
			else{
				printf("Діапазон [0,9]\t");
			}
		}
		printf("\n");
	}	
	
	for(i=0;i< N;i++)
		free(*(p+i));
		
	free(p);
}

void add_line(int N,int M,int d)
{
	int i,j,*a,**p = NULL;
	p = (int**)malloc(N*sizeof(int*));
	
	for(i=0;i<N;i++)
		*(p+i) = (int*)malloc(M*sizeof(int));
		
	for(i=0; i< N; i++){
		for (j = 0; j <M; j++) {
			printf("p[%d][%d] = ", i, j);
      		scanf("%d",(*(p+i)+j));
		}
	}
	
	for(i=0; i< N; i++){
		for (j = 0; j < M; j++)
			printf("%d\t", p[i][j]);
		printf("\n");
	}
	printf("\n");
	a = (int*)malloc(N*sizeof(int));
	
	for(i=0; i< N; i++)
		*(a+i)= 7;
		
	p = (int**)realloc(p, (++N)*sizeof(int*));
	
	for (i=0; i<N-1-d; i++) //Робиммо зміщення показників в масиві на 1
		*(p+N-1-i)= *(p+N-2-i);
	
	*(p+d)= NULL; //Обнуляємо показчик d в масиві
		
	*(p+d) = (int*)malloc(M*sizeof(int)); //Виділяємо пам'ять під масив що буде в d показчику
	
	memmove(*(p+d), a, M*sizeof(int)); //Копіюємо тимчасовий масив a у рядок d двухмірного масиву p

	for(i=0; i < N; i++){
		for (j = 0; j < M; j++){
			printf("%d\t", p[i][j]);
		}
		printf("\n");
	}

	for(i=0;i< N;i++)
		free(*(p+i));
		
	free(p);
	free(a);
}

void del_line(int N,int M,int d)
{
	int i,j,**p = NULL;
	p = (int**)malloc(N*sizeof(int*));
	
	for(i=0;i<N;i++)
		*(p+i) = (int*)malloc(M*sizeof(int));
		
	for(i=0; i< N; i++){
		for (j = 0; j <M; j++) {
			printf("p[%d][%d] = ", i, j);
      		scanf("%d",(*(p+i)+j));
		}
	}
	
	for(i=0; i< N; i++){
		for (j = 0; j < M; j++)
			printf("%d\t", p[i][j]);
		printf("\n");
	}
	printf("\n");
	
	for (i = d; i < N - 1; i++) {
        for (j = 0; j < M; j++)
            *(*(p+i)+j) = *(*(p+(i+1))+j);
        
    }
    N--;
    p = (int **) realloc(p, N * sizeof(int *));
    

	for(i=0; i < N; i++){
		for (j = 0; j < M; j++){
			printf("%d\t", p[i][j]);
		}
		printf("\n");
	}

	for(i=0;i< N;i++)
		free(*(p+i));
		
	free(p);
}



