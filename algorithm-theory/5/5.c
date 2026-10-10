#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <windows.h>

void randomizeArray(int* arr, int size);

int main() {
    system("chcp 1251");
    system("cls");
    printf("кірієнко кс24 пз5\n\n");

    int size, i, j, temp;
    LARGE_INTEGER frequency, start_time, end_time;
    unsigned long long int compares = 0, moves = 0;

    if (!QueryPerformanceFrequency(&frequency)) {
        printf("error: Не вдалося отримати частоту.\n");
        return 1;
    }

    printf("Введіть розмір масиву: ");
    scanf("%d", &size);

    while (size < 2) {
        printf("error: Розмір массиву має бути від 2 і вище. Спробуйте ще раз");
        scanf("%d", &size);
        fflush(stdin);
    }

    int arr[size];
    randomizeArray(arr, size);
    bool isRight = true;

    QueryPerformanceCounter(&start_time);

    for (i = 0; i < size; i++) {
        for (j = 0; j < size - 1; j++) {
            compares++;
            if (arr[j] < arr[j + 1]) {
                isRight = false;
                moves++;
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
        if (isRight) break;
    }

    QueryPerformanceCounter(&end_time);

    LONGLONG time_taken = end_time.QuadPart - start_time.QuadPart;
    double time_rand = (double)(time_taken) / frequency.QuadPart;

    printf("Середній випадок: Порівнянь = %llu, перестановок = %llu, Витрачено часу = %lf\n\n", compares, moves, time_rand);

    compares = 0;
    moves = 0;
    isRight = true;

    QueryPerformanceCounter(&start_time);

    for (i = 0; i < size; i++) {
        for (j = 0; j < size - 1; j++) {
            compares++;
            if (arr[j] < arr[j + 1]) {
                isRight = false;
                moves++;
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
        if (isRight) break;
    }

    QueryPerformanceCounter(&end_time);
    time_taken = end_time.QuadPart - start_time.QuadPart;
    double time_best = (double)(time_taken) / frequency.QuadPart;

    printf("Найкращий випадок: Порівнянь = %llu, перестановок = %llu, Витрачено часу = %lf\n\n", compares, moves, time_best);

    compares = 0;
    moves = 0;
    isRight = true;

    randomizeArray(arr, size);

    QueryPerformanceCounter(&start_time);

    for (i = 0; i < size; i++) {
        for (j = 0; j < size - 1; j++) {
            compares++;
            if (arr[j] < arr[j + 1]) {
                isRight = false;
                moves++;
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
        if (isRight) break;
    }

    QueryPerformanceCounter(&end_time);

    time_taken = end_time.QuadPart - start_time.QuadPart;
    double time_worst = (double)(time_taken) / frequency.QuadPart;

    printf("Найгірший випадок: Порівнянь = %llu, перестановок = %llu, Витрачено часу = %lf\n\n", compares, moves, time_worst);

    return 0;
}

void randomizeArray(int* arr, int size) {
    int i;
    for (i = 0; i < size; i++) {
        arr[i] = rand();
    }
}