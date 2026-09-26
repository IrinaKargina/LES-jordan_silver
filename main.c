#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "matrix_io.h"
#include "solve.h"

//считаем ||Ax-b|| / ||b||
double calc_residual(const double *a, const double *b, const double *x, int n); 

//считаем ||x - x идеальный||
double calc_error(const double *x, int n);

double calc_residual(const double *a, const double *b, const double *x, int n) {
    double res_norm = 0.0;
    double b_norm = 0.0;

    for (int i = 0; i < n; i++) {
        double ax_i = 0.0;
        for (int j = 0; j < n; j++) {  //умножаем i-ю строку матрицы A на вектор x = (Ax)_i
            ax_i += a[i * n + j] * x[j];
        }
        double diff = ax_i - b[i];
        res_norm += diff * diff;
        b_norm += b[i] * b[i];
    }

    return (b_norm > 0.0) ? sqrt(res_norm) / sqrt(b_norm) : sqrt(res_norm);
}

double calc_error(const double *x, int n) {
    double err_sq = 0.0;
    for (int i = 0; i < n; i++) {
        double exact = (i % 2 == 0) ? 1.0 : 0.0;
        double diff = x[i] - exact;
        err_sq += diff * diff;
    }
    return sqrt(err_sq);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: %s n m k [filename]\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int k = atoi(argv[3]);

    if (n <= 0 || m < 0 || k < 0 || k > 4) {
        printf("Error: Invalid arguments\n");
        return 1;
    }

    if (k == 0 && argc < 5) {
        printf("Error: Filename required for k = 0\n");
        return 1;
    }
    
    if (argc == 5 && k!=0){
        printf("Error: file required for k=0\n");
        return 1;
    }

    double *a = (double *)malloc((size_t)n * n * sizeof(double));
    double *a_orig = (double *)malloc((size_t)n * n * sizeof(double));
    double *b = (double *)malloc((size_t)n * sizeof(double));
    double *b_orig = (double *)malloc((size_t)n * sizeof(double));
    double *x = (double *)malloc((size_t)n * sizeof(double));

    if (!a || !a_orig || !b || !b_orig || !x) {
        printf("Error: Memory allocation failed\n");
        free(a); free(a_orig); free(b); free(b_orig); free(x);
        return 2;
    }

    // Инициализация матрицы A
    if (k > 0) {
        matr_form(a, n, k);
    } else {
        int err = matr_file(a, n, argv[4]);
        if (err == -1) {
            printf("Error: Cannot open file '%s'\n", argv[4]);
            free(a); free(a_orig); free(b); free(b_orig); free(x);
            return 3;
        } else if (err == -2) {
            printf("Error: File '%s' corrupted or incomplete\n", argv[4]);
            free(a); free(a_orig); free(b); free(b_orig); free(x);
            return 4;
        }
    }

    // Заполнение правой части b
    init_rhs(a, b, n);

    for (int i = 0; i < n * n; i++) {
        a_orig[i] = a[i];
    }
    for (int i = 0; i < n; i++) {
        b_orig[i] = b[i];
    }

    printf("Initial Matrix A:\n");
    print_matrix(a, n, n, m);
    printf("Initial Right-Hand Side b:\n");
    print_matrix(b, 1, n, m);

    clock_t start_time = clock();
    int flag = solve(n, a, b, x);
    clock_t end_time = clock();

    if (flag != 0) {
        printf("Error: Matrix is singular\n");
        free(a); free(a_orig); free(b); free(b_orig); free(x);
        return 5;
    }

    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("\nSolution x:\n");
    print_matrix(x, 1, n, m);
    printf("\n");

    double residual = calc_residual(a_orig, b_orig, x, n);
    double error = calc_error(x, n);

    
    printf("Residual norm ||Ax - b|| / ||b|| : %10.3e\n", residual);
    printf("Error norm ||x - x_exact||       : %10.3e\n", error);
    printf("Elapsed time                     : %.2f seconds\n", elapsed_time);

    free(a);
    free(a_orig);
    free(b);
    free(b_orig);
    free(x);

    return 0;
}

