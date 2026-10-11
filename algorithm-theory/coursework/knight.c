#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
int solveKTUtil(int x, int y, int movei, int **sol, int N, int *xMove, int *yMove, int *iteration_count);
int isSafe(int x, int y, int **sol, int N);
void printSolution(int **sol, int N);
int solveKT(int N, int startX, int startY, int *dimension_count, int *iteration_count);

int main()
{
    system("chcp 1251");
    system("cls");
    int N, startX, startY;
    int d = 0;
    int n; 

    printf("Enter the size of the chessboard (8 and less): ");
    if (scanf("%d", &N) != 1 || N <= 0 || N > 8)
    {
        printf("Invalid input. Please enter a valid size. \n");
        return 1;
    }

    n = N * N; 

    printf("Enter the starting position for the knight (x y): ");
    if (scanf("%d %d", &startX, &startY) != 2 || startX < 1 || startX > N || startY < 1 || startY > N)
    {
        printf("Invalid input. Please enter valid coordinates.\n");
        return 1;
    }

    clock_t start_time = clock();

    solveKT(N, startX, startY, &d, &n); 

    clock_t end_time = clock();
    double cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("Time taken: %f seconds\n", cpu_time_used);
    printf("Computed values: dimension = %d, iteration = %d\n", d, n);
    printf("Number of elements: %d\n", d);

    return 0;
}

int solveKT(int N, int startX, int startY, int *dimension_count, int *iteration_count)
{
    int **sol = (int **)malloc(N * sizeof(int *));
    for (int i = 0; i < N; i++)
    {
        sol[i] = (int *)malloc(N * sizeof(int));
        *dimension_count += N * sizeof(int);
        for (int j = 0; j < N; j++)
            sol[i][j] = -1;
    }

    int xMove[8] = {2, 1, -1, -2, -2, -1, 1, 2};
    int yMove[8] = {1, 2, 2, 1, -1, -2, -2, -1};

    sol[startX - 1][startY - 1] = 0;

    if (solveKTUtil(startX - 1, startY - 1, 1, sol, N, xMove, yMove, iteration_count) == 0)
    {
        printf("Solution does not exist");
        return 0;
    }
    else
        printSolution(sol, N);

    for (int i = 0; i < N; i++)
        free(sol[i]);
    free(sol);

    return 1;
}

int solveKTUtil(int x, int y, int movei, int **sol, int N, int *xMove, int *yMove, int *iteration_count)
{
    int k, next_x, next_y;
    (*iteration_count)++;

    if (movei == N * N)
        return 1;

    for (k = 0; k < 8; k++)
    {
        next_x = x + xMove[k];
        next_y = y + yMove[k];
        if (isSafe(next_x, next_y, sol, N))
        {
            sol[next_x][next_y] = movei;
            if (solveKTUtil(next_x, next_y, movei + 1, sol, N, xMove, yMove, iteration_count) == 1)
                return 1;
            else
                sol[next_x][next_y] = -1;
        }
    }

    return 0;
}

int isSafe(int x, int y, int **sol, int N)
{
    return (x >= 0 && x < N && y >= 0 && y < N &&
            sol[x][y] == -1);
}

void printSolution(int **sol, int N)
{
    printf("Solution:\n");
    for (int x = 0; x < N; x++)
    {
        for (int y = 0; y < N; y++)
            printf(" %2d ", sol[x][y]);
        printf("\n");
    }
}
