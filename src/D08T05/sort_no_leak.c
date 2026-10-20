#include <stdio.h>
#include <stdlib.h>

int read(int *a, int n);
void sort(int *a, int n);
void print(int *a, int n);

int main(void) {
    int n;

    if (scanf("%d", &n) != 1) {
        printf("n/a");
        return 0;
    }

    if (n <= 0) {
        printf("n/a");
        return 0;
    }

    int *a = malloc(n * sizeof(int));
    if (a == NULL) {
        printf("n/a");
        return 0;
    }

    if (!read(a, n)) {
        free(a);
        printf("n/a");
        return 0;
    }

    sort(a, n);
    print(a, n);
    free(a);

    return 0;
}

int read(int *a, int n) {
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &a[i]) != 1) {
            return 0;
        }
    }
    return 1;
}

void sort(int *a, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (a[j] > a[j + 1]) {
                int t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
            }
        }
    }
}

void print(int *a, int n) {
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            printf(" ");
        }
        printf("%d", a[i]);
    }
}
