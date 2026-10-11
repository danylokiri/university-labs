#include <stdio.h>
#include "arraylist.h"

void print_menu() {
    printf("\nMenu:\n");
    printf("1. Add element to the end of the list (add)\n");
    printf("2. Insert element at a specific position (insert)\n");
    printf("3. Get the number of elements in the list (size)\n");
    printf("4. Remove element by index (remove)\n");
    printf("5. Update value of an element (set)\n");
    printf("6. Get value of an element by index (get)\n");
    printf("7. Exit\n");
    printf("Choose an action: ");
}

int main() {
    int initial_capacity = 5;
    ArrayList *list = arraylist_create(initial_capacity);
    int choice, item, index;

    while (1) {
        print_menu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter value to add: ");
                scanf("%d", &item);
                arraylist_add(list, item);
                printf("Element %d added.\n", item);
                break;

            case 2:
                printf("Enter index to insert at: ");
                scanf("%d", &index);
                printf("Enter value to insert: ");
                scanf("%d", &item);
                arraylist_insert(list, index, item);
                printf("Element %d inserted at index %d.\n", item, index);
                break;

            case 3:
                printf("Number of elements: %d\n", arraylist_size(list));
                break;

            case 4:
                printf("Enter index to remove: ");
                scanf("%d", &index);
                arraylist_remove(list, index);
                printf("Element at index %d removed.\n", index);
                break;

            case 5:
                printf("Enter index to set: ");
                scanf("%d", &index);
                printf("Enter new value: ");
                scanf("%d", &item);
                arraylist_set(list, index, item);
                printf("Element at index %d set to %d.\n", index, item);
                break;

            case 6:
                printf("Enter index to get: ");
                scanf("%d", &index);
                item = arraylist_get(list, index);
                if (item != -1) {
                    printf("Value at index %d: %d\n", index, item);
                } else {
                    printf("Invalid index!\n");
                }
                break;

            case 7:
                printf("Exiting.\n");
                arraylist_destroy(list);
                return 0;

            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}
