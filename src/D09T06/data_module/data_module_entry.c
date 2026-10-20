#include <stdio.h>
#include <stdlib.h>

#include "../data_libs/data_io.h"
#include "data_process.h"

int main() {
    double *data = NULL;
    int n = 0;
    int b = 1;

    if (scanf("%d", &n) != 1 || n <= 0) {
        b = 0;
    }

    if (b) {
        data = malloc((size_t)n * sizeof(double));
        if (data == NULL) {
            b = 0;
        }
        if (b) {
            input(data, n);
        }
    }
    if (!b) {
        printf("n/a");
    } else if (normalization(data, n)) {
        output(data, n);
    } else {
        printf("ERROR");
    }
    free(data);
    return 0;
}
