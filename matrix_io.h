#ifndef MATRIX_IO_H
#define MATRIX_IO_H

#include <stdio.h>

double f(int k, int n, int i, int j);
void matr_form(double *a, int n, int k);
int matr_file(double *a, int n, const char *filename);
void init_rhs(const double *a, double *b, int n);
void print_matrix(const double *a, int l, int n, int m);

#endif
