#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int solveKTUtil(int x, int y, int movei, int **sol, int N, int *xMove, int *yMove, int *iteration_count);
int isSafe(int x, int y, int **sol, int N);
void printSolution(int **sol, int N);
int solveKT(int N, int startX, int startY, int *dimension_count, int *iteration_count);
int solveKTEuler(int N, int startX, int startY, int *dimension_count, int *iteration_count);
int solveKTWardermonde(int N, int startX, int startY, int *dimension_count, int *iteration_count);
int solveKTMunkCollin(int N, int startX, int startY, int *dimension_count, int *iteration_count);

int main()
{
    system("chcp 1251");
    system("cls");
    int N, startX, startY;
    int method_choice;

    while (1)
    {
        printf("Enter the size of the chessboard: ");
        if (scanf("%d", &N) != 1 || N <= 0)
        {
            printf("Invalid input. Please enter a valid size. \n");
            return 1;
        }	

        printf("Enter the starting position for the knight (x y): ");
        if (scanf("%d %d", &startX, &startY) != 2 || startX < 1 || startX > N || startY < 1 || startY > N)
        {
            printf("Invalid input. Please enter valid coordinates.\n");
            return 1;
        }

        printf("Choose the solving method:\n");
        printf("1. Backtracking (works only for desk size 8 and less)\n");
        printf("2. Euler's method\n");
        printf("3. Wardermonde's method\n");
        printf("4. Munk and Collin's method\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &method_choice);
        
        getchar();
                
        int d_backtracking = 0, n_backtracking = 0;
  		int d_euler = 0, n_euler = 0;
 		int d_wardermonde = 0, n_wardermonde = 0;
  		int d_munkcollin = 0, n_munkcollin = 0;
    	int d_vandorff = 0, n_vandorff = 0;
                
        if (method_choice != 1 && method_choice != 2 && method_choice != 3 && method_choice != 4 && method_choice != 0)
        {
            printf("Invalid input. Please enter valid number for the method choice.\n");
            return 1;
        }

        clock_t start_time, end_time;
        double cpu_time_used;

        if (method_choice == 1)
        {
            start_time = clock();
            solveKT(N, startX, startY, &d_backtracking, &n_backtracking);
            end_time = clock();
            cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
            printf("Backtracking: Time taken: %f seconds\n", cpu_time_used);
            printf("Backtracking: Dimension = %d, Iteration = %d\n", d_backtracking, n_backtracking);
        }
        else if (method_choice == 2)
        {
            start_time = clock();
            solveKTEuler(N, startX, startY, &d_euler, &n_euler);
            end_time = clock();
            cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
            printf("Euler's method: Time taken: %f seconds\n", cpu_time_used);
            printf("Euler's method: Dimension = %d, Operations = %d\n", d_euler, n_euler);
        }
        else if (method_choice == 3)
        {
            start_time = clock();
            solveKTWardermonde(N, startX, startY, &d_wardermonde, &n_wardermonde);
            end_time = clock();
            cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
            printf("Wardermonde's method: Time taken: %f seconds\n", cpu_time_used);
            printf("Wardermonde's method: Dimension = %d, Operations = %d\n", d_wardermonde, n_wardermonde);
        }
        else if (method_choice == 4)
        {
            start_time = clock();
            solveKTMunkCollin(N, startX, startY, &d_munkcollin, &n_munkcollin);
            end_time = clock();
            cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
            printf("Munk and Collin's method: Time taken: %f seconds\n", cpu_time_used);
            printf("Munk and Collin's method: Dimension = %d, Operations = %d\n", d_munkcollin, n_munkcollin);
        }
        else if (method_choice == 0)
        {
            return 0;
        }
    }

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
        printf("Backtracking Solution does not exist\n");
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
        (*iteration_count)++;
        if (isSafe(next_x, next_y, sol, N))
        {
            (*iteration_count)++;
            sol[next_x][next_y] = movei;
            if (solveKTUtil(next_x, next_y, movei + 1, sol, N, xMove, yMove, iteration_count) == 1)
                return 1;
            else
                sol[next_x][next_y] = -1;
        }
    }

    return 0;
}

int solveKTEuler(int N, int startX, int startY, int *dimension_count, int *iteration_count)
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

    int currentX = startX - 1;
    int currentY = startY - 1;
    int movei = 1;

    clock_t euler_start_time = clock();

     while (movei < N * N)
    {
        int minMoves = N + 1;
        int minIndex = -1;

        for (int k = 0; k < 8; k++)
        {
            int nextX = currentX + xMove[k];
            int nextY = currentY + yMove[k];

            if (isSafe(nextX, nextY, sol, N))
            {
                int countMoves = 0;

                for (int m = 0; m < 8; m++)
                {
                    int nx = nextX + xMove[m];
                    int ny = nextY + yMove[m];
                    if (isSafe(nx, ny, sol, N))
                        countMoves++;
                }

                if (countMoves < minMoves)
                {
                    minMoves = countMoves;
                    minIndex = k;
                }
            }
        }

        if (minIndex == -1)
            break;

        currentX += xMove[minIndex];
        currentY += yMove[minIndex];
        sol[currentX][currentY] = movei;
        movei++;
        (*iteration_count)++;
    }

    if (movei == N * N)
        printSolution(sol, N);
    else
        printf("Euler's method Solution does not exist\n");

    for (int i = 0; i < N; i++)
        free(sol[i]);
    free(sol);

    return 1;
}

int solveKTWardermonde(int N, int startX, int startY, int *dimension_count, int *iteration_count)
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

    int currentX = startX - 1;
    int currentY = startY - 1;
    int movei = 0;

    sol[currentX][currentY] = movei;
    movei++;

    while (movei < N * N)
    {
        int minIndex = -1;
        int minDegree = 9; 

        for (int k = 0; k < 8; k++)
        {
            int nextX = currentX + xMove[k];
            int nextY = currentY + yMove[k];

            if (isSafe(nextX, nextY, sol, N))
            {
                int degree = 0;

                for (int m = 0; m < 8; m++)
                {
                    int nx = nextX + xMove[m];
                    int ny = nextY + yMove[m];
                    if (isSafe(nx, ny, sol, N))
                        degree++;
                }

                if (degree < minDegree)
                {
                    minDegree = degree;
                    minIndex = k;
                }
            }
        }

        if (minIndex == -1)
            break;

        currentX += xMove[minIndex];
        currentY += yMove[minIndex];
        sol[currentX][currentY] = movei;
        movei++;
        (*iteration_count)++;
    }

    if (movei == N * N)
        printSolution(sol, N);
    else
        printf("Wardermonde's method Solution does not exist\n");

    for (int i = 0; i < N; i++)
        free(sol[i]);
    free(sol);

    return 1;
}

int solveKTMunkCollin(int N, int startX, int startY, int *dimension_count, int *iteration_count)
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

    int currentX = startX - 1;
    int currentY = startY - 1;
    int movei = 0;

    sol[currentX][currentY] = movei;
    movei++;

    while (movei < N * N)
    {
        int minIndex = -1;
        int minDegree = 9;

        for (int k = 0; k < 8; k++)
        {
            int nextX = currentX + xMove[k];
            int nextY = currentY + yMove[k];

            if (isSafe(nextX, nextY, sol, N))
            {
                int degree = 0;

                for (int m = 0; m < 8; m++)
                {
                    int nx = nextX + xMove[m];
                    int ny = nextY + yMove[m];
                    if (isSafe(nx, ny, sol, N))
                        degree++;
                }

                if (degree < minDegree)
                {
                    minDegree = degree;
                    minIndex = k;
                }
            }
        }

        if (minIndex == -1)
            break;

        currentX += xMove[minIndex];
        currentY += yMove[minIndex];
        sol[currentX][currentY] = movei;
        movei++;
        (*iteration_count)++;
    }

    if (movei == N * N)
        printSolution(sol, N);
    else
        printf("Munk and Collin's method Solution does not exist\n");

    for (int i = 0; i < N; i++)
        free(sol[i]);
    free(sol);

    return 1;
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
