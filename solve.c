#include "solve.h"
#include <math.h>

#define EPS 1e-15

int solve(int n, double *a, double *b, double *x);


int solve(int n, double *a, double *b, double *x) {
    for (int step = 0; step < n; step++) {  // поиск ведущего элемента в текущем столбце step
        
        int max_row = step;
        double max_val = fabs(a[step * n + step]);

        for (int i = step + 1; i < n; i++) {
            double cur_val = fabs(a[i * n + step]);
            if (cur_val > max_val) {
                max_val = cur_val;
                max_row = i;
            }
        }

        if (max_val < EPS) {
            return -1; // матрица вырождена
        }

     
        if (max_row != step) {        //перестановка текущей строки со строкой с ведущим элементом
            for (int j = step; j < n; j++) {
                double tmp = a[step * n + j];
                a[step * n + j] = a[max_row * n + j];
                a[max_row * n + j] = tmp;
            }
            double tmp_b = b[step];
            b[step] = b[max_row];
            b[max_row] = tmp_b;
        }

      
        double z = a[step * n + step];
        for (int j = step; j < n; j++) {      // нормируем ведущую строчку
            a[step * n + j] /= z;
        }
        b[step] /= z;

        // обнуление столбца step во всех остальных строках (и выше, и ниже)
        for (int i = 0; i < n; i++) {
            if (i != step) {
                double w = a[i * n + step];
                if (fabs(w) > 0.0) {
                    for (int j = step; j < n; j++) {
                        a[i * n + j] -= w * a[step * n + j];
                    }
                    b[i] -= w * b[step];
                }
            }
        }
    }

    for (int i = 0; i < n; i++) {   //вектор ответа хранится в x
        x[i] = b[i];
    }

    return 0;
}

