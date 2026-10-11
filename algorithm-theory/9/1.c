#include <stdio.h>
#include <stdlib.h>

#define MIN_MERGE 32

// Сортировка вставками
void insertion_sort(int arr[], int left, int right) {
    for (int i = left + 1; i <= right; i++) {
        int j = i;
        while (j > left && arr[j] < arr[j - 1]) {
            // Обмін значеннями
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

    // Створення тимчасових масивів
    int L[n1], R[n2];

    // Копіювання даних у тимчасові масиви L[] і R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Злиття тимчасових масивів назад в основний масив arr[l..r]
    i = 0; // Індекс початкового елемента першого підмасиву
    j = 0; // Індекс початкового елемента другого підмасиву
    k = l; // Індекс початкового елемента підмасиву, що зливається
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

    // Копіювання решти елементів L[], якщо такі є
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Копіювання решти елементів R[], якщо такі є
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// Timsort
void timsort(int arr[], int n) {
    for (int i = 0; i < n; i += MIN_MERGE) {
        insertion_sort(arr, i, i + MIN_MERGE - 1 < n - 1 ? i + MIN_MERGE - 1 : n - 1);
    }

    for (int size = MIN_MERGE; size < n; size = 2 * size) {
        for (int left = 0; left < n; left += 2 * size) {
            int mid = left + size - 1;
            int right = (left + 2 * size - 1 < n) ? left + 2 * size - 1 : n - 1;
merge(arr, left, mid, right);
        }
    }
}

// Виведення масиву
void print_array(int arr[], int size) {
    for (int i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

// Приклад використання
int main() {
	system("chcp 1251");
 system("cls");
    int arr[] = {4, 2, 7, 1, 9, 5, 8, 3, 6};
    int n = sizeof(arr) / sizeof(arr[0]);

    printf("Початковий масив: \n");
    print_array(arr, n);

    timsort(arr, n);

    printf("Відсортований масив: \n");
    print_array(arr, n);

    return 0;
}
