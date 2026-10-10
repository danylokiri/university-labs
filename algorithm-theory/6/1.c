#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>

int comparisons = 0;
int swaps = 0;
long long int comparisons1 = 0;
long long int swaps1 = 0;

void swap_heap(int *a, int *b);
void heapify(int arr[], int n, int i);
void heapSort(int arr[], int n);
void printArrayA(int arr[], int n);
void swap_quick(int *a, int *b);
int partition(int array[], int low, int high);
void quickSort(int array[], int low, int high);
void printArrayB(int array[], int size);

int main() {
    system("chcp 1251");
    system("cls");

    int size, i, j;
    double milliseconds;
    srand(time(NULL));

    LARGE_INTEGER frequency, start_time, end_time;
    QueryPerformanceFrequency(&frequency);

    printf("Введіть розмір масиву: ");
    scanf("%d", &size);
    int arrA[size], arrB[size];

    printf("\nHEAPSORT:\n\n");

    for (i = 0, j = size; i < size; i++, j--) {
        arrA[i] = j;
    }

    QueryPerformanceCounter(&start_time);
    heapSort(arrA, size);
    QueryPerformanceCounter(&end_time);

    printf("\nНайгірший випадок: \n");

    LONGLONG time_taken = end_time.QuadPart - start_time.QuadPart;
    milliseconds = (double)(time_taken * 1000) / frequency.QuadPart;
    printf("Кількість перестановок %d\n", swaps);
    printf("Кількість порівнянь %d\n", comparisons);
    printf("Час виконання: %f\n\n", milliseconds);

    comparisons = 0;
    swaps = 0;

    for (i = 0; i < size; i++) {
        arrA[i] = i + 1;
    }

    QueryPerformanceCounter(&start_time);
    heapSort(arrA, size);
    QueryPerformanceCounter(&end_time);

    printf("\nНайкращий випадок: \n");

    time_taken = end_time.QuadPart - start_time.QuadPart;
    milliseconds = (double)(time_taken * 1000) / frequency.QuadPart;
    printf("Кількість перестановок %d\n", swaps);
    printf("Кількість порівнянь %d\n", comparisons);
    printf("Час виконання: %f\n\n", milliseconds);

    comparisons = 0;
    swaps = 0;

    for (i = 0; i < size; i++) {
        arrA[i] = rand() % 100;
    }

    QueryPerformanceCounter(&start_time);
    heapSort(arrA, size);
    QueryPerformanceCounter(&end_time);

    printf("\nСередній випадок: \n");

    time_taken = end_time.QuadPart - start_time.QuadPart;
    milliseconds = (double)(time_taken * 1000) / frequency.QuadPart;
    printf("Кількість перестановок %d\n", swaps);
    printf("Кількість порівнянь %d\n", comparisons);
    printf("Час виконання: %f\n\n", milliseconds);

    printf("QUICKSORT:\n\n");

    for (i = 0, j = size; i < size; i++, j--) {
        arrB[i] = j;
    }

    QueryPerformanceCounter(&start_time);
    quickSort(arrB, 0, size - 1);
    QueryPerformanceCounter(&end_time);

    printf("\nНайгірший випадок: \n");

    time_taken = end_time.QuadPart - start_time.QuadPart;
    milliseconds = (double)(time_taken * 1000) / frequency.QuadPart;
    printf("Кількість перестановок %d\n", swaps1);
    printf("Кількість порівнянь %d\n", comparisons1);
    printf("Час виконання: %f\n\n", milliseconds);

    comparisons1 = 0;
    swaps1 = 0;

    for (i = 0; i < size; i++) {
        arrB[i] = i + 1;
    }

    QueryPerformanceCounter(&start_time);
    quickSort(arrB, 0, size - 1);
    QueryPerformanceCounter(&end_time);

    printf("\nНайкращий випадок: \n");

    time_taken = end_time.QuadPart - start_time.QuadPart;
    milliseconds = (double)(time_taken * 1000) / frequency.QuadPart;
    printf("Кількість перестановок %d\n", swaps1);
    printf("Кількість порівнянь %d\n", comparisons1);
    printf("Час виконання: %f \n\n", milliseconds);

    comparisons1 = 0;
    swaps1 = 0;

    for (i = 0; i < size; i++) {
        arrB[i] = rand() % 100;
    }

    QueryPerformanceCounter(&start_time);
    quickSort(arrB, 0, size - 1);
    QueryPerformanceCounter(&end_time);

    printf("\nСередній випадок: \n");

    time_taken = end_time.QuadPart - start_time.QuadPart;
    milliseconds = (double)(time_taken * 1000) / frequency.QuadPart;
    printf("Кількість перестановок %d\n", swaps1);
    printf("Кількість порівнянь %d\n", comparisons1);
    printf("Час виконання: %f\n\n", milliseconds);

    return 0;
}

void swap_heap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
    swaps++;
}

void heapify(int arr[], int n, int i) {
    comparisons++;
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && arr[left] > arr[largest])
        largest = left;
    if (right < n && arr[right] > arr[largest])
        largest = right;
    if (largest != i) {
        swap_heap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    int i;
    for (i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    for (i = n - 1; i >= 0; i--) {
        swap_heap(&arr[0], &arr[i]);
        heapify(arr, i, 0);
    }
}

void printArrayA(int arr[], int n) {
    int i;
    for (i = 0; i < n; ++i)
        printf("%d ", arr[i]);
    printf("\n");
}

void swap_quick(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
    swaps1++;
}

int partition(int array[], int low, int high) {
    int pivot = array[high];
    int i = (low - 1), j;
    for (j = low; j < high; j++) {
        comparisons1++;
        if (array[j] <= pivot) {
            i++;
            swap_quick(&array[i], &array[j]);
        }
    }
    swap_quick(&array[i + 1], &array[high]);

    return (i + 1);
}

void quickSort(int array[], int low, int high) {
    if (low < high) {
        int pi = partition(array, low, high);
        quickSort(array, low, pi - 1);
        quickSort(array, pi + 1, high);
    }
}

void printArrayB(int array[], int size) {
    int i;
    for (i = 0; i < size; ++i) {
        printf("%d ", array[i]);
    }
    printf("\n");
}
