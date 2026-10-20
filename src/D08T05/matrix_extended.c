#include <stdio.h>
#include <stdlib.h>

static void print_matrix_static(int a[100][100], int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (j) putchar(' ');
            printf("%d", a[i][j]);
        }
        if (i != rows - 1) putchar('\n');
    }
}

static void print_matrix_row_pointers(int **a, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (j) putchar(' ');
            printf("%d", a[i][j]);
        }
        if (i != rows - 1) putchar('\n');
    }
}

static void print_matrix_flat(int *a, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (j) putchar(' ');
            printf("%d", a[(size_t)i * cols + j]);
        }
        if (i != rows - 1) putchar('\n');
    }
}

static void print_row_maxima_of_row_pointers(int **a, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        int max = a[i][0];
        for (int j = 1; j < cols; ++j) {
            if (a[i][j] > max) max = a[i][j];
        }
        if (i) putchar(' ');
        printf("%d", max);
    }
}

static void print_row_maxima_of_flat(int *a, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        int max = a[(size_t)i * cols];
        for (int j = 1; j < cols; ++j) {
            int value = a[(size_t)i * cols + j];
            if (value > max) max = value;
        }
        if (i) putchar(' ');
        printf("%d", max);
    }
}

static void print_column_minima_of_row_pointers(int **a, int rows, int cols) {
    for (int j = 0; j < cols; ++j) {
        int min = a[0][j];
        for (int i = 1; i < rows; ++i) {
            if (a[i][j] < min) min = a[i][j];
        }
        if (j) putchar(' ');
        printf("%d", min);
    }
}

static void print_column_minima_of_flat(int *a, int rows, int cols) {
    for (int j = 0; j < cols; ++j) {
        int min = a[j];
        for (int i = 1; i < rows; ++i) {
            int value = a[(size_t)i * cols + j];
            if (value < min) min = value;
        }
        if (j) putchar(' ');
        printf("%d", min);
    }
}

static int run_static_matrix(int rows, int cols) {
    if (rows > 100 || cols > 100) return 0;

    int a[100][100];

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (scanf("%d", &a[i][j]) != 1) return 0;
        }
    }

    print_matrix_static(a, rows, cols);

    putchar('\n');
    print_row_maxima_of_flat(&a[0][0], rows, cols);
    putchar('\n');
    print_column_minima_of_flat(&a[0][0], rows, cols);

    return 1;
}

static int run_matrix_with_row_pointers(int rows, int cols) {
    int **a = malloc((size_t)rows * sizeof(int *));
    if (!a) return 0;

    for (int i = 0; i < rows; ++i) {
        a[i] = malloc((size_t)cols * sizeof(int));
        if (!a[i]) {
            for (int k = 0; k < i; ++k) free(a[k]);
            free(a);
            return 0;
        }
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (scanf("%d", &a[i][j]) != 1) {
                for (int k = 0; k < rows; ++k) free(a[k]);
                free(a);
                return 0;
            }
        }
    }

    print_matrix_row_pointers(a, rows, cols);

    putchar('\n');
    print_row_maxima_of_row_pointers(a, rows, cols);
    putchar('\n');
    print_column_minima_of_row_pointers(a, rows, cols);

    for (int i = 0; i < rows; ++i) free(a[i]);
    free(a);
    return 1;
}

static int run_matrix_with_contiguous_block(int rows, int cols) {
    int **a = malloc((size_t)rows * sizeof(int *));
    if (!a) return 0;

    int *data = malloc((size_t)rows * (size_t)cols * sizeof(int));
    if (!data) {
        free(a);
        return 0;
    }

    for (int i = 0; i < rows; ++i) {
        a[i] = data + (size_t)i * cols;
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (scanf("%d", &a[i][j]) != 1) {
                free(data);
                free(a);
                return 0;
            }
        }
    }

    print_matrix_row_pointers(a, rows, cols);

    putchar('\n');
    print_row_maxima_of_flat(data, rows, cols);
    putchar('\n');
    print_column_minima_of_flat(data, rows, cols);

    free(data);
    free(a);
    return 1;
}

static int run_flat_matrix(int rows, int cols) {
    int *a = malloc((size_t)rows * (size_t)cols * sizeof(int));
    if (!a) return 0;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (scanf("%d", &a[(size_t)i * cols + j]) != 1) {
                free(a);
                return 0;
            }
        }
    }

    print_matrix_flat(a, rows, cols);

    putchar('\n');
    print_row_maxima_of_flat(a, rows, cols);
    putchar('\n');
    print_column_minima_of_flat(a, rows, cols);

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

    int rows, cols;
    if (scanf("%d %d", &rows, &cols) != 2) {
        printf("n/a");
        return 0;
    }

    if (rows <= 0 || cols <= 0) {
        printf("n/a");
        return 0;
    }

    int success = 0;

    switch (choice) {
        case 1:
            success = run_static_matrix(rows, cols);
            break;
        case 2:
            success = run_matrix_with_row_pointers(rows, cols);
            break;
        case 3:
            success = run_matrix_with_contiguous_block(rows, cols);
            break;
        case 4:
            success = run_flat_matrix(rows, cols);
            break;
    }

    if (!success) {
        printf("n/a");
    }

    return 0;
}
