#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_MERGE 32

// Сортування вставками
void insertion_sort(int arr[], int left, int right) {
	int i;
    for (i = left + 1; i <= right; i++) {
        int j = i;
        while (j > left && arr[j] < arr[j - 1]) {
            // обмін значеннями
            int temp = arr[j];
            arr[j] = arr[j - 1];
            arr[j - 1] = temp;
            j--;
        }
    }
}

// злиття відсортованих масивів
void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // створення тимчасових масивів
    int L[n1], R[n2];

    // копіювання даних в тимчасові масиви L[] і R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // злиття тимчасових масивів назад у основний масив arr[l..r]
    i = 0; // індекс початкового елемента першого підмасива
    j = 0; // індекс початкового елемента другого підмасива
    k = l; // індекс початкового елемента злитого підмасива
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // копіювання залишених елементів L[], якщо такі є
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // копіювання залишених елементів R[], якщо такі є
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Timsort
void timsort(int arr[], int n) {
	int i, size, left;
    int min_merge = n / 8; 

    for (i = 0; i < n; i += min_merge) {
        insertion_sort(arr, i, i + min_merge - 1 < n - 1 ? i + min_merge - 1 : n - 1);
    }

    for (size = min_merge; size < n; size = 2 * size) {
        for (left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = (left + 2 * size - 1 < n) ? left + 2 * size - 1 : n - 1;

            merge(arr, left, mid, right);
        }
    }
}

// вивід масиву
void print_array(int arr[], int size) {
	int i;
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

// читання масиву з файлу
int* read_array_from_file(const char* filename, int* n) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Помилка відкриття файлу");
        exit(EXIT_FAILURE);
    }

    int num;
    int capacity = 10;  // початкова ємність масиву
    int* arr = (int*)malloc(capacity * sizeof(int));

    if (arr == NULL) {
        perror("Помилка виділення пам'яті");
        exit(EXIT_FAILURE);
    }

    *n = 0;
while (fscanf(file, "%d", &num) == 1) {
        if (*n == capacity) {
            capacity *= 2;
            arr = realloc(arr, capacity * sizeof(int));
            if (arr == NULL) {
                perror("Помилка збільшення розміру масиву");
                exit(EXIT_FAILURE);
            }
        }

        arr[*n] = num;
        (*n)++;
    }

    fclose(file);
    return arr;
}

// запис масиву в файл
void write_array_to_file(const char* filename, int arr[], int n) {
	int i;
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Помилка відкриття файлу зля запису");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < n; i++) {
        fprintf(file, "%d ", arr[i]);
    }

    fclose(file);
}

//  виміоювання часу виконання сортування в мілісекундах
void measure_time(int arr[], int n) {
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    timsort(arr, n);
    end = clock();

    cpu_time_used = ((double)(end - start)) / (CLOCKS_PER_SEC / 1000); // в мілісекундах

    printf("Час виконання сортування: %f мілісекунд\n", cpu_time_used);
}

// приклад використання
int main() {
	system("chcp 1251");
 system("cls");
    const char* input_filename = "input.txt";
    const char* output_filename = "output.txt";

    int n;
    int* arr = read_array_from_file(input_filename, &n);
    printf("Вхідний масив: \n");
    print_array(arr, n);

    measure_time(arr, n);

    printf("Відсортований масив: \n");
    print_array(arr, n);

    write_array_to_file(output_filename, arr, n);

    free(arr); // збільшення виділеної пам'яті

    return 0;
}
