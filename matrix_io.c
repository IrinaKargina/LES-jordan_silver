#include "matrix_io.h"
#include <math.h>
#include <stdlib.h>


double f(int k, int n, int i, int j);
void matr_form(double *a, int n, int k); //пишет матрицу по формуле
int matr_file(double *a, int n, const char *filename); //матрица из файла
void init_rhs(const double *a, double *b, int n);  //правая часть слу по матрице коэф
void print_matrix(const double *a, int l, int n, int m);

double f(int k, int n, int i, int j) {  //вычисляет каждый эл а(ij)
    switch (k) {
        case 1:
            return (double)(n - (i > j ? i : j) + 1); //если i>j, то i, иначе j = max(i,j)
        case 2:
            return (double)(i > j ? i : j); //double - преобр типа
        case 3:
            return (double)abs(i - j);
        case 4:
            return 1.0 / (double)(i + j - 1);
        default:
            return 0.0;
    }
}

void matr_form(double *a, int n, int k) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i * n + j] = f(k, n, i + 1, j + 1);
        }
    }
}

int matr_file(double *a, int n, const char *file) {
    FILE *fp = fopen(file, "r");
    if (!fp) {
        return -1;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (fscanf(fp, "%lf", &a[i * n + j]) != 1) {
                fclose(fp);
                return -2;
            }
        }
    }

    fclose(fp);
    return 0;
}

void init_rhs(const double *a, double *b, int n) {
    for (int i = 0; i < n; i++) {
        b[i] = 0.0;
        for (int j = 0; j < n; j += 2) {
            b[i] += a[i * n + j];
        }
    }
}

void print_matrix(const double *a, int l, int n, int m) {
    int am_rows = (l < m) ? l : m;
    int am_cols = (n < m) ? n : m;

    for (int i = 0; i < am_rows; i++) {
        for (int j = 0; j < am_cols; j++) {
            printf(" %10.3e", a[i * n + j]);
        }
        printf("\n");
    }
}
