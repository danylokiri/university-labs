#ifndef ARRAYLIST_H
#define ARRAYLIST_H

typedef struct {
    int *data;
    int size;
    int capacity;
} ArrayList;

ArrayList *arraylist_create(int initial_capacity);
void arraylist_destroy(ArrayList *list);
void arraylist_add(ArrayList *list, int item);
void arraylist_insert(ArrayList *list, int index, int item);
int arraylist_size(ArrayList *list);
void arraylist_remove(ArrayList *list, int index);
void arraylist_set(ArrayList *list, int index, int item);
int arraylist_get(ArrayList *list, int index);

#endif
