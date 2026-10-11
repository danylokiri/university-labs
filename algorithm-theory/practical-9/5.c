#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// функція для заміни елементів місцями
void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

// функція для знаходження позиції розділу
int partition(int array[], int low, int high) {
  
  // вибрати крайній правий елемент як півот
  int pivot = array[high];
  
  // покажчик на більший елемент
  int i = (low - 1);

  // обхід кожного елементу масиву
  // порівнюємо їх зі стовпчиком
  for (int j = low; j < high; j++) {
    if (array[j] <= pivot) {
        
      // якщо знайдено елемент менший за pivot
      // поміняти його місцями з більшим елементом, на який вказує i
      i++;
      // поміняти місцями елемент i з елементом j
      swap(&array[i], &array[j]);
    }
  }

  // поміняти місцями поворотний елемент з більшим елементом при i
  swap(&array[i + 1], &array[high]);
  
  // повернути точку розділу
  return (i + 1);
}

void quickSort(int array[], int low, int high) {
  if (low < high) {
    
    // знайти півот-елемент такий, що
    // елементи, менші за півот, знаходяться зліва від півота
    // елементи, більші за pivot, знаходяться праворуч від pivot
    int pi = partition(array, low, high);
    
    // рекурсивний виклик зліва від pivot
    quickSort(array, low, pi - 1);
    
    // рекурсивний виклик праворуч від pivot
    quickSort(array, pi + 1, high);
  }
}

// функція для виведення елементів масиву
void printArray(int array[], int size) {
  for (int i = 0; i < size; ++i) {
    printf("%d ", array[i]);
  }
  printf("\n");
}

// функція для вимірювання часу сортування в мілісекундах
void measureSortingTime(int array[], int size) {
  // отримати початковий час
  struct timespec start_time, end_time;
  clock_gettime(CLOCK_MONOTONIC, &start_time);

  // виконати швидке сортування даних
  quickSort(array, 0, size - 1);

  // отримати кінцевий час
  clock_gettime(CLOCK_MONOTONIC, &end_time);

  // вивести час сортування в мілісекундах
  double time_taken = (end_time.tv_sec - start_time.tv_sec) * 1e3 +
                      (end_time.tv_nsec - start_time.tv_nsec) * 1e-6;
  printf("Час сортування: %f мілісекунд\n", time_taken);
}

// функція для читання масиву з текстового файлу
int* readArrayFromFile(const char* filename, int* size) {
FILE* file = fopen(filename, "r");
  if (!file) {
    perror("Помилка відкриття файлу");
    exit(EXIT_FAILURE);
  }

  // визначити розмір масиву
  int count = 0;
  int value;
  while (fscanf(file, "%d", &value) == 1) {
    count++;
  }

  // повторно встановити позицію файлового вказівника на початок
  fseek(file, 0, SEEK_SET);

  // виділити пам'ять для зберігання масиву
  int* array = (int*)malloc(count * sizeof(int));
  if (!array) {
    perror("Помилка виділення пам'яті");
    exit(EXIT_FAILURE);
  }

  // зчитати масив з файлу
  for (int i = 0; i < count; i++) {
    fscanf(file, "%d", &array[i]);
  }

  // встановити розмір масиву
  *size = count;

  fclose(file);
  return array;
}

// функція для запису масиву в текстовий файл
void writeArrayToFile(const char* filename, int array[], int size) {
  FILE* file = fopen(filename, "w");
  if (!file) {
    perror("Помилка відкриття файлу для запису");
    exit(EXIT_FAILURE);
  }

  // записати масив в файл
  for (int i = 0; i < size; i++) {
    fprintf(file, "%d ", array[i]);
  }

  fclose(file);
}

// основна функція
int main() {
  system("chcp 1251");
 system("cls");
  int size;
  int* data = readArrayFromFile("input.txt", &size);
printf("Початковий масив\n");
  printArray(data, size);

  // виміряти час сортування
  measureSortingTime(data, size);

  // записати відсортований масив у текстовий файл
  writeArrayToFile("output.txt", data, size);
  
  printf("Відсортований масив: \n");
    printArray(data, size);
  
  free(data); // важливо звільнити виділену пам'ять

  return 0;
}
