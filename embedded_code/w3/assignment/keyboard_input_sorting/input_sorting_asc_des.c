/*
오름차순은 과거 날짜에서 최근 날짜로 정렬하는 방식이고 --> 작은 것에서 큰 것으로 배열되도록 분류하는 것을 말함.
내림차순은 최근 날짜에서 과거 날짜 순서로 정렬하는 방식 --> 큰 것에서 작은 것으로 배열되도록 분류하는 것을 말함.
*/
#include <stdio.h>
#include <stdlib.h>

// Bubble Sort Function
void bubbleSort(int arr[], int n, int ascending) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if ((ascending && arr[j] > arr[j + 1]) || (!ascending && arr[j] < arr[j + 1])) {
                // Swap elements
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    int size, choice;

    printf("Enter the number of elements: ");
    scanf("%d", &size);

    if (size <= 0) {
        printf("Invalid size!\n");
        return 1;
    }

    int *arr = (int *)malloc(size * sizeof(int));
    if (arr == NULL) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    printf("Enter %d numbers: ", size);
    for (int i = 0; i < size; i++) {
        scanf("%d", &arr[i]);
    }

    printf("Choose sorting order:\n");
    printf("1. Ascending Order\n");
    printf("2. Descending Order\n");
    printf("Enter your choice (1 or 2): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            bubbleSort(arr, size, 1); // 오름차순 정렬
            printf("Sorted array (Ascending):\n");
            break;
        case 2:
            bubbleSort(arr, size, 0); // 내림차순 정렬
            printf("Sorted array (Descending):\n");
            break;
        default:
            printf("Invalid choice! Exiting...\n");
            free(arr);
            return 1;
    }

    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    free(arr);
    return 0;
}
