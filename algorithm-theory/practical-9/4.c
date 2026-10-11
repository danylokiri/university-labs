#include <stdio.h>

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

// основна функція
int main() {
	system("chcp 1251");
 system("cls");
  int data[] = {8, 7, 2, 1, 0, 9, 6};
  
  int n = sizeof(data) / sizeof(data[0]);
  
  printf("Початковий масив\n");
  printArray(data, n);
  
  // виконати швидке сортування даних
quickSort(data, 0, n - 1);
  
  printf("Відсортований масив: \n");
  printArray(data, n);
}
