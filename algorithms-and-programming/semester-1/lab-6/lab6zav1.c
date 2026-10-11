#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void print_arr(int C[11][8]);
void create_random_arr(int C[11][8]);
int average_column(int C[11][8], int av_arr[11]);
int amount_largestnum(int C[11][8], int av_arr[11]);

int main()
{
	int menu;
	int C[11][8], av_arr[11];
	printf("Zavdannya 1 Lab 6\n");
	while (1)
	{
		printf("\nchoose option: \n1 - Start\n2 - Exit\n\n");
		scanf("%d", &menu);

		if (menu == 1)
		{
			create_random_arr(C);
			print_arr(C);
			printf("\n");

			average_column(C, av_arr);
			amount_largestnum(C, av_arr);
		}
		else if (menu == 2)	break;
		else printf("\nerror (write a value from the list)\n");
	}
}

void create_random_arr(int C[11][8])
{
	int n;
	printf("\nenter the maximum allowable value: ");
	scanf("%d", &n);
	printf("\nstart massive: \n");
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 8; j++)
			C[i][j] = rand() % n;
	}
}

void print_arr(int C[11][8])
{
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 8; j++)
			printf("%d ", C[i][j]);
		printf("\n");
	}
}

int average_column(int C[11][8], int av_arr[11])
{
	int sum;
	for (int i = 0; i < 11; i++)
	{
		sum=0;
		for (int j = 0; j < 8; j++)
			sum += C[i][j];
		av_arr[i] = sum / 11;
	}
}

int amount_largestnum(int C[11][8], int av_arr[11])
{
	int i, res;
	for (i = 0; i < 11; i++)
	{
		res = 0;
		printf("A row %d: ", i);
		for (int j = 0; j < 8; j++)
		{
			if (C[i][j] > av_arr[i])
			{
				printf("%d ", C[i][j]);
				res++;
			}
		}
		printf(" |  amount: %d\n", res);
	}
}
