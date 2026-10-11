#include <stdio.h>
#include <stdlib.h>
#include "arraylist.h"

ArrayList *arraylist_create(int initial_capacity) {
    ArrayList *list = malloc(sizeof(ArrayList));
    list->data = malloc(sizeof(int) * initial_capacity);
    list->size = 0;
    list->capacity = initial_capacity;
    return list;
}

void arraylist_destroy(ArrayList *list) {
    free(list->data);
    free(list);
}

void arraylist_add(ArrayList *list, int item) {
    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, sizeof(int) * list->capacity);
    }
    list->data[list->size++] = item;
}

void arraylist_insert(ArrayList *list, int index, int item) {
    if (index < 0 || index > list->size) return;

    if (list->size >= list->capacity) {
        list->capacity *= 2;
        list->data = realloc(list->data, sizeof(int) * list->capacity);
    }
    for (int i = list->size; i > index; i--) {
        list->data[i] = list->data[i - 1];
    }
    list->data[index] = item;
    list->size++;
}

int arraylist_size(ArrayList *list) {
    return list->size;
}

void arraylist_remove(ArrayList *list, int index) {
    if (index < 0 || index >= list->size) return;

    for (int i = index; i < list->size - 1; i++) {
        list->data[i] = list->data[i + 1];
    }
    list->size--;
}

void arraylist_set(ArrayList *list, int index, int item) {
    if (index < 0 || index >= list->size) return;

    list->data[index] = item;
}

int arraylist_get(ArrayList *list, int index) {
    if (index < 0 || index >= list->size) return -1;

    return list->data[index];
}
