#include <stdio.h>
#include <stdlib.h>

static void print_static(int a[100][100], int n, int m) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (j) putchar(' ');
            printf("%d", a[i][j]);
        }
        if (i != n - 1) putchar('\n');
    }
}

static void print_ptr(int **a, int n, int m) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (j) putchar(' ');
            printf("%d", a[i][j]);
        }
        if (i != n - 1) putchar('\n');
    }
}

static void print_flat(int *a, int n, int m) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (j) putchar(' ');
            printf("%d", a[(size_t)i * m + j]);
        }
        if (i != n - 1) putchar('\n');
    }
}

static int process_static(int n, int m) {
    if (n > 100 || m > 100) return 0;

    int a[100][100];

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (scanf("%d", &a[i][j]) != 1) return 0;
        }
    }

    print_static(a, n, m);
    return 1;
}

static int process_dyn_ptr(int n, int m) {
    int **a = malloc((size_t)n * sizeof(int *));
    if (!a) return 0;

    for (int i = 0; i < n; ++i) {
        a[i] = malloc((size_t)m * sizeof(int));
        if (!a[i]) {
            for (int k = 0; k < i; ++k) free(a[k]);
            free(a);
            return 0;
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (scanf("%d", &a[i][j]) != 1) {
                for (int k = 0; k < n; ++k) free(a[k]);
                free(a);
                return 0;
            }
        }
    }

    print_ptr(a, n, m);

    for (int i = 0; i < n; ++i) free(a[i]);
    free(a);
    return 1;
}

static int process_dyn_contig(int n, int m) {
    int **a = malloc((size_t)n * sizeof(int *));
    if (!a) return 0;

    int *data = malloc((size_t)n * (size_t)m * sizeof(int));
    if (!data) {
        free(a);
        return 0;
    }

    for (int i = 0; i < n; ++i) {
        a[i] = data + (size_t)i * m;
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (scanf("%d", &a[i][j]) != 1) {
                free(data);
                free(a);
                return 0;
            }
        }
    }

    print_ptr(a, n, m);

    free(data);
    free(a);
    return 1;
}

/* 4. Динамическое: одномерный массив с ручной индексацией */
static int process_dyn_flat(int n, int m) {
    int *a = malloc((size_t)n * (size_t)m * sizeof(int));
    if (!a) return 0;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (scanf("%d", &a[(size_t)i * m + j]) != 1) {
                free(a);
                return 0;
            }
        }
    }

    print_flat(a, n, m);
    free(a);
    return 1;
}

int main(void) {
    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("n/a");
        return 0;
    }

    if (choice < 1 || choice > 4) {
        printf("n/a");
        return 0;
    }

    int n, m;
    if (scanf("%d %d", &n, &m) != 2) {
        printf("n/a");
        return 0;
    }

    if (n <= 0 || m <= 0) {
        printf("n/a");
        return 0;
    }

    int ok = 0;

    switch (choice) {
        case 1:
            ok = process_static(n, m);
            break;
        case 2:
            ok = process_dyn_ptr(n, m);
            break;
        case 3:
            ok = process_dyn_contig(n, m);
            break;
        case 4:
            ok = process_dyn_flat(n, m);
            break;
    }

    if (!ok) {
        printf("n/a");
    }

    return 0;
}
