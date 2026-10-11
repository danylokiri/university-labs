#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <time.h>
#define arrSize 4000

int comparisons = 0;
int swaps = 0;

void add_in_fileA(int arr_A[]);
void add_in_fileB(int arr_B[]);
void add_in_fileC(int arr_C[]);
void readFromFile(char *filename, int arr[], int size);
void shellSort(int array[], int n);
void ciuraShellSort(int arr[], int n);

int main() {
    system("chcp 1251");
    system("cls");
    int k;
    LARGE_INTEGER frequency, start_time, end_time;
    QueryPerformanceFrequency(&frequency);
    double milliseconds;

    srand(time(NULL));

    int arr_size;
    printf("Введіть розмір масиву 1 < x < 4000:\n");
    scanf("%d", &arr_size);

    if (arr_size > 4000 || arr_size < 1) {
        printf("Повторіть спробу!");
        return 1;
    }

    int arr_A[arrSize], arr_B[arrSize], arr_C[arrSize];
    int interval;

    FILE *fileA = fopen("Відсортований масив.txt", "r");
    FILE *fileB = fopen("Невідсортований масив.txt", "r");
    FILE *fileC = fopen("Масив рандомних значень.txt", "r");
    if (fileA && fileB && fileC) {
        fclose(fileA);
        fclose(fileB);
        fclose(fileC);
    } else {
        add_in_fileA(arr_A);
        add_in_fileB(arr_B);
        add_in_fileC(arr_C);
    }

    printf("Проміжки Шелла - ");
    for (interval = 0, k = 1; interval < arr_size; k++) {
        if (interval == 1) {
            break;
        }
        interval = arr_size / pow(2, k);
        printf("%d, ", interval);
    }
    printf("\n");
    printf("Проміжки Frank & Lazarus - ");
    for (interval = 0, k = 1; interval < arr_size; k++) {
        if (interval == 1) {
            break;
        }
        interval = 2 * (arr_size / pow(2, k + 1)) + 1;
        printf("%d, ", interval);
    }
    printf("\n");
    printf("Проміжки Papernov & Stasevich - 1, ");
    for (interval = 0, k = 1; interval < arr_size; k++) {
        interval = pow(2, k) + 1;
        if (interval >= arr_size) {
            break;
        }
        printf("%d, ", interval);
    }
    printf("\n");
    printf("Проміжки Knuth - ");
    for (interval = 0, k = 1; interval <= arr_size / 3; k++) {
        interval = ((pow(3, k)) - 1) / 2;
        printf("%d, ", interval);
    }
    printf("\n");
    printf("Проміжки Sedgewick, 1982 - 1, ");
    for (interval = 0, k = 1; interval <= arr_size; k++) {
        interval = pow(4, k) + 3 * pow(2, k - 1) + 1;
        if (interval >= arr_size) {
            break;
        }
        printf("%d, ", interval);
    }
    printf("\n");
    printf("Проміжки Sedgewick, 1986 - 1, ");
    for (interval = 0, k = 1; interval <= arr_size; k++) {
        if (k % 2 == 0)
            interval = 9 * (pow(2, k) - pow(2, k / 2)) + 1;
        else
            interval = 8 * pow(2, k) - 6 * pow(2, (k + 1) / 2) + 1;
        if (interval >= arr_size) {
            break;
        }
        printf("%d, ", interval);
    }
    printf("\n");
    printf("Проміжки Tokuda - ");
    for (interval = 0, k = 1; interval <= arr_size; k++) {
        int t = 9 / 4, o = 1 / 5;
        interval = ((9 * (pow(t, k - 1)) - 4) / 5);
        if (interval >= arr_size) {
            break;
        }
        printf("%d, ", interval);
    }
    printf("\n");

    readFromFile("Відсортований масив.txt", arr_A, arr_size);
    readFromFile("Невідсортований масив.txt", arr_B, arr_size);
    readFromFile("Масив рандомних значень.txt", arr_C, arr_size);

    QueryPerformanceCounter(&start_time);
    shellSort(arr_A, arr_size);
    QueryPerformanceCounter(&end_time);

    printf("\nНайкращий випадок: \n");

    LONGLONG time_taken = end_time.QuadPart - start_time.QuadPart;
    milliseconds = (double)(time_taken * 1000) / frequency.QuadPart;
    printf("Кількість перестановок %d\n", swaps);
    printf("Кількість порівнянь %d\n", comparisons);
    printf("Час виконання: %f мілісекунд\n\n", milliseconds);

    comparisons = 0;
    swaps = 0;

    QueryPerformanceCounter(&start_time);
    shellSort(arr_B, arr_size);
    QueryPerformanceCounter(&end_time);

    printf("\nСередній випадок: \n");

    time_taken = end_time.QuadPart - start_time.QuadPart;
    milliseconds = (double)(time_taken * 1000) / frequency.QuadPart;
    printf("Кількість перестановок %d\n", swaps);
    printf("Кількість порівнянь %d\n", comparisons);
    printf("Час виконання: %f мілісекунд\n\n", milliseconds);

    comparisons = 0;
    swaps = 0;

    QueryPerformanceCounter(&start_time);
    shellSort(arr_C, arr_size);
    QueryPerformanceCounter(&end_time);

    printf("\nНайгірший випадок: \n");

    time_taken = end_time.QuadPart - start_time.QuadPart;
    milliseconds = (double)(time_taken * 1000) / frequency.QuadPart;
    printf("Кількість перестановок %d\n", swaps);
    printf("Кількість порівнянь %d\n", comparisons);
    printf("Час виконання: %f мілісекунд\n\n", milliseconds);

    comparisons = 0;
    swaps = 0;

    readFromFile("Відсортований масив.txt", arr_A, arr_size);
    readFromFile("Невідсортований масив.txt", arr_B, arr_size);
    readFromFile("Масив рандомних значень.txt", arr_C, arr_size);

    QueryPerformanceCounter(&start_time);
    ciuraShellSort(arr_A, arr_size);
    QueryPerformanceCounter(&end_time);

    printf("\nНайкращий випадок: \n");

    time_taken = end_time.QuadPart - start_time.QuadPart;
    milliseconds = (double)(time_taken * 1000) / frequency.QuadPart;
    printf("Кількість перестановок %d\n", swaps);
    printf("Кількість порівнянь %d\n", comparisons);
    printf("Час виконання: %f мілісекунд\n\n", milliseconds);

    comparisons = 0;
    swaps = 0;

    QueryPerformanceCounter(&start_time);
    ciuraShellSort(arr_B, arr_size);
    QueryPerformanceCounter(&end_time);

    printf("\nСередній випадок: \n");

    time_taken = end_time.QuadPart - start_time.QuadPart;
    milliseconds = (double)(time_taken * 1000) / frequency.QuadPart;
    printf("Кількість перестановок %d\n", swaps);
    printf("Кількість порівнянь %d\n", comparisons);
    printf("Час виконання: %f мілісекунд\n\n", milliseconds);

    comparisons = 0;
    swaps = 0;

    QueryPerformanceCounter(&start_time);
    ciuraShellSort(arr_C, arr_size);
    QueryPerformanceCounter(&end_time);

    printf("\nНайгірший випадок: \n");
    time_taken = end_time.QuadPart - start_time.QuadPart;
    milliseconds = (double)(time_taken * 1000) / frequency.QuadPart;
    printf("Кількість перестановок %d\n", swaps);
    printf("Кількість порівнянь %d\n", comparisons);
    printf("Час виконання: %f мілісекунд\n\n", milliseconds);

    comparisons = 0;
    swaps = 0;

    return 1;
}

void ciuraShellSort(int arr[], int n) {
    int arrCiura[] = {701, 301, 132, 57, 23, 10, 4, 1};
    int index, i, j;
    for (index = 0; index < sizeof(arrCiura) / sizeof(arrCiura[0]); index++) {
        int gap = arrCiura[index];
        for (i = gap; i < n; i++) {
            comparisons++;
            int temp = arr[i];
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
                swaps++;
            }
            arr[j] = temp;
        }
    }
}

void shellSort(int array[], int n) {
    int interval, i;
    for (interval = n / 2; interval > 0; interval /= 2) {
        for (i = interval; i < n; i++) {
            comparisons++;
            int temp = array[i];
            int j;
            for (j = i; j >= interval && array[j - interval] > temp; j -= interval) {
                array[j] = array[j - interval];
                swaps++;
            }
            array[j] = temp;
        }
    }
}

void readFromFile(char *filename, int arr[], int size) {
    int i;
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Не вдалося відкрити файл!");
        return;
    }
    for (i = 0; i < size; i++) {
        if (fscanf(file, "%d", &arr[i]) != 1) {
            perror("Помилка при зчитуванні з файлу!");
            break;
        }
    }
    fclose(file);
}

void add_in_fileA(int arr_A[]) {
    FILE *fp;
    int i;
    if ((fp = fopen("Відсортований масив.txt", "w")) == NULL) {
        perror("Не вдалося відкрити файл!\n");
        return;
    }

    for (i = 0; i < arrSize; i++) {
        arr_A[i] = i + 1;
        fprintf(fp, "%d ", arr_A[i]);
    }
    fclose(fp);
}

void add_in_fileB(int arr_B[]) {
    FILE *fp;
    int i, j;
    if ((fp = fopen("Невідсортований масив.txt", "w")) == NULL) {
        perror("Не вдалося відкрити файл!\n");
        return;
    }

    for (j = arrSize, i = 0; i < arrSize; i++, j--) {
        arr_B[i] = j;
        fprintf(fp, "%d ", arr_B[i]);
    }
    fclose(fp);
}

void add_in_fileC(int arr_C[]) {
    FILE *fp;
    int i;
    if ((fp = fopen("Масив рандомних значень.txt", "w")) == NULL) {
        perror("Не вдалося відкрити файл!\n");
        return;
    }

    for (i = 0; i < arrSize; i++) {
        arr_C[i] = rand() % arrSize;
        fprintf(fp, "%d ", arr_C[i]);
    }
    fclose(fp);
}
