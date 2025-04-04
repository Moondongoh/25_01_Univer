/*
오름차순은 과거 날짜에서 최근 날짜로 정렬하는 방식이고 --> 작은 것에서 큰 것으로 배열되도록 분류하는 것을 말함.
내림차순은 최근 날짜에서 과거 날짜 순서로 정렬하는 방식 --> 큰 것에서 작은 것으로 배열되도록 분류하는 것을 말함.
*/
#include <stdio.h>
#include <stdlib.h>

void bubbleSort(int arr[], int n, int ascending) {
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

int main() {
    int size, choice;

    printf("넣을 숫자의 개수: ");
    scanf("%d", &size);

    if (size <= 0) {
        printf("다시 시도하세요.\n");
        return 1;
    }

    int *arr = (int *)malloc(size * sizeof(int));
    if (arr == NULL) {
        printf("메모리 할당 실패\n");
        return 1;
    }

    printf("%d개의 번호 입력: ", size);
    for (int i = 0; i < size; i++) {
        scanf("%d", &arr[i]);
    }

    printf("원하는 기능을 번호로 선택하세요.: \n");
    printf("1. 오름차순\n");
    printf("2. 내림차순\n");
    printf("번호 입력 (1 혹은 2): ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            bubbleSort(arr, size, 1); 
            printf("배열이 정렬되었습니다. (오름차순):\n");
            break;
        case 2:
            bubbleSort(arr, size, 0); 
            printf("배열이 정렬되었습니다. (내림차순):\n");
            break;
        default:
            printf("다시 시도 하세요.\n");
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
