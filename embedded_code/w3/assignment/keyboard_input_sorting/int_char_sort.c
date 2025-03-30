#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Bubble Sort for Numbers
void bubbleSortNumbers(int arr[], int n, int ascending) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if ((ascending && arr[j] > arr[j + 1]) || (!ascending && arr[j] < arr[j + 1])) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Bubble Sort for Strings
void bubbleSortStrings(char arr[][100], int n, int ascending) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if ((ascending && strcmp(arr[j], arr[j + 1]) > 0) || (!ascending && strcmp(arr[j], arr[j + 1]) < 0)) {
                char temp[100];
                strcpy(temp, arr[j]);
                strcpy(arr[j], arr[j + 1]);
                strcpy(arr[j + 1], temp);
            }
        }
    }
}

int main() {
    int choice, size, order;

    printf("Choose data type to sort:\n");
    printf("1. Numbers\n");
    printf("2. Strings (English words)\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);

    printf("Enter the number of elements: ");
    scanf("%d", &size);

    if (size <= 0) {
        printf("Invalid size!\n");
        return 1;
    }

    printf("Choose sorting order:\n");
    printf("1. Ascending Order\n");
    printf("2. Descending Order\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &order);

    if (order != 1 && order != 2) {
        printf("Invalid sorting order!\n");
        return 1;
    }

    switch (choice) {
        case 1: { // Sorting Numbers
            int *arr = (int *)malloc(size * sizeof(int));
            if (arr == NULL) {
                printf("Memory allocation failed!\n");
                return 1;
            }

            printf("Enter %d numbers: ", size);
            for (int i = 0; i < size; i++) {
                scanf("%d", &arr[i]);
            }

            bubbleSortNumbers(arr, size, order == 1);
            printf("Sorted numbers (%s):\n", order == 1 ? "Ascending" : "Descending");
            for (int i = 0; i < size; i++) {
                printf("%d ", arr[i]);
            }
            printf("\n");

            free(arr);
            break;
        }
        case 2: { // Sorting Strings
            char arr[size][100];

            printf("Enter %d words: ", size);
            for (int i = 0; i < size; i++) {
                scanf("%s", arr[i]);
            }

            bubbleSortStrings(arr, size, order == 1);
            printf("Sorted words (%s):\n", order == 1 ? "Ascending" : "Descending");
            for (int i = 0; i < size; i++) {
                printf("%s ", arr[i]);
            }
            printf("\n");

            break;
        }
        default:
            printf("Invalid choice! Exiting...\n");
            return 1;
    }

    return 0;
}