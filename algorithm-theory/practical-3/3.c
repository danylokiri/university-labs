#include <stdio.h>
#include <stdlib.h>

int main() {
    system("chcp 1251");
    system("cls");

    int amount;

    printf("Введіть кількість цвяхів: ");
    scanf("%d", &amount);

    while (amount < 2 || amount > 100) {
        printf("error: кількість цвяхів має входитим в проміжок від 1 до 100");
        scanf("%d", &amount);
    }

    int A[amount], temp, range = 0;

    for (int j = 0; j < amount; j++) {
        printf("Введіть координату %d-го цвяха: ", j + 1);
        scanf("%d", &A[j]);

        for (int i = 0; i < j; i++) {
            while (A[j] == A[i] || A[j] > 10000 || A[j] < 0) {
                printf("error: коорднити цвяхів мають належати проміжку від 0 до 10000, два цвяха не можуть бути на одній коордитнаті");
                scanf("%d", &A[j]);
            }
        }
    }

    for (int i = 0; i < amount - 1; i++) {
        for (int j = 0; j < amount - 1 - i; j++) {
            if (A[j] < A[j + 1]) {
                temp = A[j];
                A[j] = A[j + 1];
                A[j + 1] = temp;
            }
        }
    }

    if (amount == 2) {
        printf("Мінімальна сумарна довжина ниток, дорівнює %d", A[0] - A[1]);
        return 0;
    }

    if (amount == 3) {
        printf("Мінімальна сумарна довжина ниток, дорівнює %d", A[0] - A[2]);
        return 0;
    }

    for (int i = 0; i < amount - 2; i++) {
        if (i == 0) {
            range += (A[i] - A[i + 1]);
            continue;
        }

        if (i == amount - 3) {
            range += (A[i + 1] - A[i + 2]);
            break;
        }

        if (A[i] - A[i + 1] < A[i + 1] - A[i + 2])
            range += (A[i] - A[i + 1]);
        else
            range += (A[i + 1] - A[i + 2]);
    }

    printf("Мінімальна сумарна довжина ниточок дорівнює %d", range);
    return 0;
}
