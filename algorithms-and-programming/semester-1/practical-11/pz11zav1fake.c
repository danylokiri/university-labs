
 #include <stdio.h>
 #include <stdlib.h>
 #include <ctype.h>
 #include <time.h>


 int findMin(int sizeN, int arr[sizeN]);
 int *getMaxValues_columns(int sizeN, int arr[sizeN][sizeN]);

 void printArr(int sizeN, int arr[sizeN][sizeN]);
 void update_mainDiagonal(int sizeN, int arr[sizeN][sizeN], int num);
 void randomlyInitializeArr(int sizeN, int arr[sizeN][sizeN]);

 int main()
 {
 	
     srand(time(NULL));

     int sizeN;
     int mode;

     printf("please enter N: ");
     scanf("%d", &sizeN);


     int arr_pt[sizeN][sizeN];
    
     printf("would you like to use random initialization? (1/2) ");
     scanf("%d", &mode);

     if(mode == 1)
     {
         randomlyInitializeArr(sizeN, arr_pt);
     }
     else 
     {   
         for(int k = 0; k < sizeN; k++)
         {
             for(int j = 0; j < sizeN; j++)
             {
                 int num;
                 scanf("%d", &num);

                 arr_pt[k][j] = num;
             }
         }
     }

     printf("BEFORE: \n");
     printArr(sizeN, arr_pt);

     int *maxValues_arr = getMaxValues_columns(sizeN, arr_pt);
     int minVal = findMin(sizeN, maxValues_arr);

     update_mainDiagonal(sizeN, arr_pt, minVal);

     printf("AFTER: \n");
     printArr(sizeN, arr_pt);

     return 0;
 }


 void printArr(int sizeN, int arr[sizeN][sizeN])
 {
     for(int k = 0; k < sizeN; k++)
     {
         for(int j = 0; j < sizeN; j++)
         {
             printf("%d ",arr[k][j]);
         }
         printf("\n");
     }
     printf("~~~~~~~~~~\n\n");
 }

 int findMin(int sizeN, int arr[sizeN]) 
 {
     int min = arr[0];
     for(int k = 0; k < sizeN; k++)
     {
         if(arr[k] > min)
             min = arr[k];
     }

     return min;
 }

 int *getMaxValues_columns(int sizeN, int arr[sizeN][sizeN])
 {
     int *result_arr = (int*) malloc(sizeN * sizeof(int));

     for(int k = 0; k < sizeN; k++)
     {
         int max = arr[k][0];
         for(int j = 0; j < sizeN; j++)
         {
             if(max > arr[j][k])
                 max = arr[j][k];
         }

         result_arr[k] = max;
     }

     return result_arr;
 }

 void update_mainDiagonal(int sizeN, int arr[sizeN][sizeN], int num)
 {
     for(int k = 0; k < sizeN; k++)
         arr[k][k] = num;
 }

 void randomlyInitializeArr(int sizeN, int arr[sizeN][sizeN])
 {
     for(int k = 0; k < sizeN; k++)
     {
         for(int j = 0; j < sizeN; j++)
         {
             arr[k][j] = rand() % 100;
         }
     }
 }
