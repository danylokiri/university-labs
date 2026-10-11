#include <stdio.h>
#include <string.h>

#define MAX_DEVICES 10
#define MAX_NAME_LENGTH 50

struct device {
    int number;
    char name[MAX_NAME_LENGTH];
    int year;
    float cost;
};

void add_device(struct device devices[MAX_DEVICES], int *num_devices);
void remove_device(struct device devices[MAX_DEVICES], int *num_devices);
void print_devices(struct device devices[MAX_DEVICES], int num_devices);

int main() {
	system("chcp 1251");
	system("cls");
    struct device devices[MAX_DEVICES];
    int num_devices = 0;
    int menubar;
	printf("Практичне завдання 4\nКірієнко Данило\n");
    while (1) {
        printf("\n1 - Додати пристрій \n");
        printf("2 - Видалити пристрій\n");
        printf("3 - Надрукувати список пристроїв\n");
        printf("4 - Вихід \n");
        scanf("%d", &menubar);

        switch(menubar)
		{   
            case 1:
                add_device(devices, &num_devices);
                break;
            case 2:
                remove_device(devices, &num_devices);
                break;
            case 3:
                print_devices(devices, num_devices);
                break;
            case 4:
                printf("\nПрограма завершена\n");
                return 0;
            default:
                printf("Помилка, повторіть спробу\n");
        }
        continue;
    }
    return 0;
}

void add_device(struct device devices[], int *num_devices) {
	int i;
    if (*num_devices >= MAX_DEVICES) {
        printf("Помилка! Досягнуто максимальний ліміт пристроїв\n");
        return;
    }
    struct device new_device;
    printf("Введіть номер пристрою: ");
    scanf("%d", &new_device.number);
    for (i = 0; i < *num_devices; i++) {
        if (new_device.number == devices[i].number) {
            printf("Цей пристрій вже є\n");
            return;
        }
    }
    printf("Введіть назву пристроїв: ");
    scanf("%s", new_device.name);
    printf("Введіть рік пристрою: ");
    scanf("%d", &new_device.year);
    printf("Введіть вартість пристрою: ");
    scanf("%f", &new_device.cost);
    devices[*num_devices] = new_device;
    (*num_devices)++;
    
    printf("Пристрій додано\n");
}

void remove_device(struct device devices[], int *num_devices) {
    int number,i,j;
    printf("Введіть номер пристрою: ");
    scanf("%d", &number);
    for (i = 0; i < *num_devices; i++) {
        if (number == devices[i].number) {
            for (j = i; j < *num_devices - 1; j++) {
                devices[j] = devices[j+1];
            }
            (*num_devices)--;
            printf("Пристрій додано\n");
            return;
        }
    }
    printf("Пристрою з цим номерос не існує\n");
}

void print_devices(struct device devices[], int num_devices) {
	int i;
    printf("Номер\tНазва\tРік\tВартість\n");
    for (i = 0; i < num_devices; i++) {
        printf("%d\t%s\t%d\t%.2f\n", devices[i].number, devices[i].name, devices[i].year, devices[i].cost);
    }
}

