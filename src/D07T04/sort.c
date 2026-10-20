#include <stdio.h>

#define SIZE 10

int read_array(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        if (scanf("%d", &arr[i]) != 1) {
            return 0;
        }
    }
    return 1;
}

void sort_array(int *arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

void print_array(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", arr[i]);
    }
    printf("\n");
}

int main(void) {
    int arr[SIZE];

    if (!read_array(arr, SIZE)) {
        printf("n/a\n");
        return 0;
    }

    sort_array(arr, SIZE);
    print_array(arr, SIZE);

    return 0;
}
