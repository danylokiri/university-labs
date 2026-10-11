#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void insertion_sort(int arr[], int left, int right) {
	int i;
    for (i = left + 1; i <= right; i++) {
        int j = i;
        while (j > left && arr[j] < arr[j - 1]) {
            int temp = arr[j];
            arr[j] = arr[j - 1];
            arr[j - 1] = temp;
            j--;
        }
    }
}

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
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

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

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


void print_array(int arr[], int size) {
	int i;
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void write_array_to_file(const char* filename, int arr[], int n) {
	int i;
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Помилка відкриття файлу для запису");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < n; i++) {
        fprintf(file, "%d ", arr[i]);
    }

    fclose(file);
}

void generate_random_array(int arr[], int n) {
	int i;
    srand(time(NULL));
    for (i = 0; i < n; i++) {
        arr[i] = rand() % 1000; // Змініть 100 на максимальне значення
    }
}

void generate_sorted_array(int arr[], int n) {
	int i;
    for (i = 0; i < n; i++) {
        arr[i] = i;
    }
}

void generate_reverse_sorted_array(int arr[], int n) {
	int i;
    for (i = 0; i < n; i++) {
        arr[i] = n - i;
    }
}

int main() {
	system("chcp 1251");
 system("cls");
    const char* input_filename = "input.txt";
    const char* output_filename = "output.txt";

    int array_size;
    int choice;

    printf("Оберіть тип масиву:\n");
    printf("1. Випадковий масив\n");
    printf("2. Відсортований масив\n");
    printf("3. Відсортований у зворотному порядку\n");
    scanf("%d", &choice);

    printf("Введіть розмір масиву: ");
    scanf("%d", &array_size);

    int* arr = malloc(array_size * sizeof(int));
    if (arr == NULL) {
        perror("Помилка виділення пам'яті");
        exit(EXIT_FAILURE);
    }

    switch (choice) {
        case 1:
            generate_random_array(arr, array_size);
            break;
        case 2:
            generate_sorted_array(arr, array_size);
            break;
case 3:
            generate_reverse_sorted_array(arr, array_size);
            break;
        default:
            printf("Невірний вибір\n");
            free(arr);
            exit(EXIT_FAILURE);
    }

    printf("Початковий масив: \n");
    print_array(arr, array_size);

    // Запис початкового масиву ц файлі
    write_array_to_file(input_filename, arr, array_size);

    clock_t start, end;
    double cpu_time_used;

    start = clock();
    timsort(arr, array_size);
    end = clock();

    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC * 1000; // в мілісекундах

    printf("Відсортований масив: \n");
    print_array(arr, array_size);

    printf("Час сортування: %f мс\n", cpu_time_used);

    // зіпис відсортованого масиву у файл
    write_array_to_file(output_filename, arr, array_size);

    // звільнення пам'яті
    free(arr);

    return 0;
}
