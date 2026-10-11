#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define arrSize 10000
int comparisons = 0, swaps = 0;

void JSort(int arr[], int n);
void add_in_fileA();
void add_in_fileB();
void add_in_fileC();
void readFromFile(char *filename, int arr[], int size);

int main() {
    system("chcp 1251");
    system("cls");

    LARGE_INTEGER frequency, start_time, end_time;
    QueryPerformanceFrequency(&frequency);
    double milliseconds;

    srand(time(NULL));

    FILE *fileA = fopen("SortedArray.txt", "r");
    FILE *fileB = fopen("UnsortedArray.txt", "r");
    FILE *fileC = fopen("RandomArray.txt", "r");

    if (fileA && fileB && fileC) {
        fclose(fileA);
        fclose(fileB);
        fclose(fileC);
    } else {
        add_in_fileA();
        add_in_fileB();
        add_in_fileC();
    }

    int arr_size;
    printf("Enter array size (1 < size < 10000):\n");
    scanf("%d", &arr_size);

    if (arr_size > arrSize || arr_size < 1) {
        printf("Invalid size! Try again.\n");
        return 1;
    }

    int arr_A[arr_size], arr_B[arr_size], arr_C[arr_size];

    readFromFile("SortedArray.txt", arr_A, arr_size);
    readFromFile("UnsortedArray.txt", arr_B, arr_size);
    readFromFile("RandomArray.txt", arr_C, arr_size);

    QueryPerformanceCounter(&start_time);
    JSort(arr_A, arr_size);
    QueryPerformanceCounter(&end_time);

    printf("\nBest Case:\n");

    LONGLONG time_taken = end_time.QuadPart - start_time.QuadPart;
    milliseconds = (double)(time_taken * 1000) / frequency.QuadPart;
    printf("Swaps: %d\n", swaps);
    printf("Comparisons: %d\n", comparisons);
    printf("Execution Time: %f milliseconds\n\n", milliseconds);

    comparisons = 0;
    swaps = 0;

    QueryPerformanceCounter(&start_time);
    JSort(arr_B, arr_size);
    QueryPerformanceCounter(&end_time);

    printf("\nAverage Case:\n");

    time_taken = end_time.QuadPart - start_time.QuadPart;
    milliseconds = (double)(time_taken * 1000) / frequency.QuadPart;
    printf("Swaps: %d\n", swaps);
    printf("Comparisons: %d\n", comparisons);
    printf("Execution Time: %f milliseconds\n\n", milliseconds);

    comparisons = 0;
    swaps = 0;

    QueryPerformanceCounter(&start_time);
    JSort(arr_C, arr_size);
    QueryPerformanceCounter(&end_time);

    printf("\nWorst Case:\n");

    time_taken = end_time.QuadPart - start_time.QuadPart;
    milliseconds = (double)(time_taken * 1000) / frequency.QuadPart;
    printf("Swaps: %d\n", swaps);
    printf("Comparisons: %d\n", comparisons);
    printf("Execution Time: %f milliseconds\n\n", milliseconds);

    comparisons = 0;
    swaps = 0;
    return 0;
}

void JSort(int arr[], int n) {
    int i, j, temp;
    int gap = n;
    do {
        gap = (gap + 1) / 2;
        for (i = gap; i < n; i++) {
            temp = arr[i];
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
                comparisons++;
                swaps++;
            }
            arr[j] = temp;
            swaps++;
        }
    } while (gap > 1);
}

void readFromFile(char *filename, int arr[], int size) {
    int i;
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Failed to open the file!");
        return;
    }

    for (i = 0; i < size; i++) {
        if (fscanf(file, "%d", &arr[i]) != 1) {
            perror("Error reading from the file!");
            break;
        }
    }

    fclose(file);
}

void add_in_fileA() {
    FILE *fp;
    int i;
    if ((fp = fopen("SortedArray.txt", "w")) == NULL) {
        perror("Failed to open the file!\n");
        return;
    }

    for (i = 1; i <= arrSize; i++) {
        fprintf(fp, "%d ", i);
    }

    fclose(fp);
}

void add_in_fileB() {
    FILE *fp;
    int i, j;
    if ((fp = fopen("UnsortedArray.txt", "w")) == NULL) {
        perror("Failed to open the file!\n");
        return;
    }

    for (j = arrSize, i = 0; i < arrSize; i++, j--) {
        fprintf(fp, "%d ", j);
    }

    fclose(fp);
}

void add_in_fileC() {
    FILE *fp;
    int i;
    if ((fp = fopen("RandomArray.txt", "w")) == NULL) {
        perror("Failed to open the file!\n");
        return;
    }

    for (i = 0; i < arrSize; i++) {
        int k = rand() % arrSize;
        fprintf(fp, "%d ", k);
    }

    fclose(fp);
}
