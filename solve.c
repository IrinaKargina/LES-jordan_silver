#include "solve.h"
#include <math.h>

#define EPS 1e-15

int solve(int n, double *a, double *b, double *x);


int solve(int n, double *a, double *b, double *x) {
    int step, i, j;
    int max_row;
    double max_val;
    double cur_val;
    double tmp;
    double tmp_b;
    double z;
    double w;
    
    for ( step = 0; step < n; step++) {  // поиск ведущего (max) элемента в текущем столбце step
        
         max_row = step;
         max_val = fabs(a[step * n + step]);

        for ( i = step + 1; i < n; i++) {
             cur_val = fabs(a[i * n + step]);
            if (cur_val > max_val) {
                max_val = cur_val;
                max_row = i;
            }
        }

        if (max_val < EPS) {
            return -1; // матрица вырождена
        }

     
        if (max_row != step) {        //перестановка текущей строки со строкой с ведущим элементом
            for ( j = step; j < n; j++) {
                 tmp = a[step * n + j];
                a[step * n + j] = a[max_row * n + j];
                a[max_row * n + j] = tmp;
            }
             tmp_b = b[step];
            b[step] = b[max_row];
            b[max_row] = tmp_b;
        }

      
         z = a[step * n + step];
        for ( j = step; j < n; j++) {      // нормируем ведущую строчку
            a[step * n + j] /= z;
        }
        b[step] /= z;

        // обнуление столбца step во всех остальных строках (и выше, и ниже)
        for ( i = 0; i < n; i++) {
            if (i != step) {
                 w = a[i * n + step];
                if (fabs(w) > 0.0) {
                    for ( j = step; j < n; j++) {
                        a[i * n + j] -= w * a[step * n + j];
                    }
                    b[i] -= w * b[step];
                }
            }
        }
    }

    for ( i = 0; i < n; i++) {   //вектор ответа хранится в x
        x[i] = b[i];
    }

    return 0;
}

