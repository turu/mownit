#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <gsl/gsl_statistics.h>
#include <gsl/gsl_blas.h>


void mul(double ** a, double ** b, double ** res, int size){
    int i,j,k;

    for (i = 0; i < size; i++) {
        for ( j = 0; j < size; j++) {
            for (k = 0; k < size; k++) {
                res[i][j] += a[i][k] * b[k][i];
            }
        }
    }
}

void mulAfterRotate(double ** a, double ** b, double ** res, int size) {
    int i,j,k;

    for (i = 0; i < size; i++) {
        for ( j = 0; j < size; j++) {
            for (k = 0; k < size; k++) {
                res[i][j] += a[i][k] * b[i][k];
            }
        }
    }
}

void rotate(double ** a, int size) {
    int s = 0, i;
    double tmp;
    while (s < size - 1) {
        for (i = s + 1; i < size; i++) {
            tmp = a[s][i];
            a[s][i] = a[i][s];
            a[i][s] = tmp;
        }
        s++;
    }
}

void gen(double ** m, int size){
    int i, j;
    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            m[i][j] = ((double)rand())/(rand()+1);
        }
    }
}

void zeros(double ** m, int size) {
    int i;
    for (i = 0; i < size; i++) {
        memset(m[i], 0, size*sizeof(double));
    }
}

void printMatrix(double ** m, int size){
    int i, j;
    for (i = 0; i < size; i++) {
        printf("[ ");
        for (j = 0; j < size - 1; j++) {
            printf("%lf, ", m[i][j]);
        }
        printf("%lf ", m[i][j]);
        printf("]\n");
    }
}

double * flatten(double ** m, double * res, int size) {
    int i;
    for (i = 0; i < size; i++) {
        memcpy(res, m[i], size*sizeof(double));
        res += size;
    }
    return res;
}

int main(int argc, char *argv[]){
    int size = 10, iters = 10, i;
    if (argc == 3) {
        size = atoi(argv[1]);
        iters = atoi(argv[2]);
    }

    double ** a = (double**) malloc(size * sizeof(double*));
    double ** b = (double**) malloc(size * sizeof(double*));
    double ** c = (double**) malloc(size * sizeof(double*));
    for (i = 0; i < size; i++) {
        a[i] = (double*) malloc(size * sizeof(double));
        b[i] = (double*) malloc(size * sizeof(double));
        c[i] = (double*) malloc(size * sizeof(double));
    }

    gen(a, size);
    gen(b, size);
    zeros(c, size);

    //printf("a = \n");
    //printMatrix(a, size);
    //printf("\nb = \n");
    //printMatrix(b, size);
    //printf("\nc = \n");
    //printMatrix(c, size);

    double * flatA = (double*) malloc(size * size * sizeof(double));
    double * flatB = (double*) malloc(size * size * sizeof(double));
    double * flatC = (double*) malloc(size * size * sizeof(double));

    gsl_matrix_view A = gsl_matrix_view_array(flatten(a, flatA, size), size, size);
    gsl_matrix_view B = gsl_matrix_view_array(flatten(b, flatB, size), size, size);
    gsl_matrix_view C = gsl_matrix_view_array(flatten(c, flatC, size), size, size);

    double times[iters];
    struct timeval stop, start;
    int n;

    n = iters;
    while (n--) {
        gettimeofday(&start, NULL);
        mul(a, b, c, size);
        gettimeofday(&stop, NULL);
        times[n] = (double)stop.tv_usec+(double)stop.tv_sec*1000000 - (double)start.tv_usec-(double)start.tv_sec*1000000;
        //printf("zajela %.1f usec\n",times[l]);
    }
    printf("\nProste mnozenie srednio zajelo %f usec\n", gsl_stats_mean(times, 1, iters));
    printf("\tOdchylenie standardowe %f usec\n", gsl_stats_sd(times, 1, iters));

    rotate(b, size);
    //printf("\nb after rotation = \n");
    //printMatrix(b, size);

    n = iters;
    while (n--) {
        gettimeofday(&start, NULL);
        mulAfterRotate(a, b, c, size);
        gettimeofday(&stop, NULL);
        times[n] = (double)stop.tv_usec+(double)stop.tv_sec*1000000 - (double)start.tv_usec-(double)start.tv_sec*1000000;
        //printf("zajela %.1f usec\n",times[l]);
    }
    printf("\nUlepszone mnozenie srednio zajelo %f usec\n",gsl_stats_mean(times, 1, iters));
    printf("\tOdchylenie standardowe %f usec\n",gsl_stats_sd(times, 1, iters));

    n = iters;
    while (n--) {
        gettimeofday(&start, NULL);
        gsl_blas_dgemm (CblasNoTrans, CblasNoTrans,1.0, &(A.matrix), &(B.matrix), 0.0, &(C.matrix));
        gettimeofday(&stop, NULL);
        times[n] = (double)stop.tv_usec+(double)stop.tv_sec*1000000 - (double)start.tv_usec-(double)start.tv_sec*1000000;
        //printf("zajela %.1f usec\n",times[l]);
    }
    printf("\nBLAS mnozenie srednio zajelo %f usec\n",gsl_stats_mean(times, 1, iters));
    printf("\tOdchylenie standardowe %f usec\n",gsl_stats_sd(times, 1, iters));

    free(flatA);
    free(flatB);
    free(flatC);

    putchar('a');

    for (i = 0; i < size; i++) {
        free(a[i]);
        free(b[i]);
        free(c[i]);
    }
    putchar('b');
    free(a);
    free(b);
    free(c);
    putchar('c');

    return 0;
}
